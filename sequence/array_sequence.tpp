#include "array_sequence.hpp"

// ---------- Constructors and Destructor ----------

template <class T>
ArraySequence<T>::ArraySequence() : items() {}

template <class T>
ArraySequence<T>::ArraySequence(T *items, int count) : items(items, count) {}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T> &list) : items(list.items) {}

template <class T>
ArraySequence<T>::~ArraySequence() {}

// ---------- Sequence Interface Implementation ----------

template <class T>
T ArraySequence<T>::GetFirst() const { return items.Get(0); }

template <class T>
T ArraySequence<T>::GetLast() const { return items.Get(items.GetSize() - 1); }

template <class T>
T ArraySequence<T>::Get(int index) const { return items.Get(index); }

template <class T>
int ArraySequence<T>::GetLength() const { return items.GetSize(); }

// Caller is responsible for deleting the returned pointer
template <class T>
ArraySequence<T> *ArraySequence<T>::GetSubSequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= items.GetSize() || start_index > end_index) {
        throw std::out_of_range("Index out of range");
    }

    int new_size = end_index - start_index + 1;
    ArraySequence<T> *sub_sequence = new ArraySequence<T>();
    sub_sequence->items.Resize(new_size);
    
    for (int i = 0; i < new_size; i++) {
        sub_sequence->items.Set(i, items.Get(start_index + i));
    }
    return sub_sequence;
}

// Using instance 
template <class T>
ArraySequence<T> *ArraySequence<T>::Append(T item) {
    return Instance()->AppendInternal(item);
}

template <class T>
ArraySequence<T> *ArraySequence<T>::Prepend(T item) {
    return Instance()->PrependInternal(item);
}

template <class T>
ArraySequence<T> *ArraySequence<T>::InsertAt(T item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

// Returns new heap-allocated sequence
template <class T>
ArraySequence<T> *ArraySequence<T>::Concat(const Sequence<T> *list) const {
    int total_size = GetLength() + list->GetLength();

    // Create a temporary C-array to hold the concatenated items
    T *concat_items = new T[total_size];
    for (int i = 0; i < GetLength(); i++) {
        concat_items[i] = Get(i);
    }
    for (int i = 0; i < list->GetLength(); i++) {
        concat_items[GetLength() + i] = list->Get(i);
    }
    ArraySequence<T> *concat_sequence = new ArraySequence<T>(concat_items, total_size);
    delete[] concat_items;
    return concat_sequence;
}

// ---------- Internal Methods (modify in place) ----------

template <class T>
ArraySequence<T> *ArraySequence<T>::AppendInternal(T item) {
    items.Resize(items.GetSize() + 1);
    items.Set(items.GetSize() - 1, item);
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::PrependInternal(T item) {
    items.Resize(items.GetSize() + 1);
    for (int i = items.GetSize() - 1; i > 0; i--) {
        items.Set(i, items.Get(i - 1));
    }
    items.Set(0, item);
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::InsertAtInternal(T item, int index) {
    if (index < 0 || index > items.GetSize()) throw std::out_of_range("Index out of range");
    items.Resize(items.GetSize() + 1);
    for (int i = items.GetSize() - 1; i > index; i--) {
        items.Set(i, items.Get(i - 1));
    }
    items.Set(index, item);
    return this;
}

// ---------- Map, Where, Reduce ----------

template <class T>
template <class T2>
ArraySequence<T2> *ArraySequence<T>::Map(T2 (*func)(T)) const {
    ArraySequence<T2> *mapped_sequence = new ArraySequence<T2>();
    for (int i = 0; i < GetLength(); i++) {
        mapped_sequence->AppendInternal(func(Get(i)));
    }
    return mapped_sequence;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::Where(bool (*predicate)(T)) const {
    ArraySequence<T> *filtered_sequence = new ArraySequence<T>();
    for (int i = 0; i < GetLength(); i++) {
        if (predicate(Get(i))) filtered_sequence->Append(Get(i));
    }
    return filtered_sequence;
}

template <class T>
template <class T2>
T2 ArraySequence<T>::Reduce(T2 (*func)(T2, T), T2 initial) const {
    T2 result = initial;
    for (int i = 0; i < GetLength(); i++) {
        result = func(result, Get(i));
    }
    return result;
}

// ---------- Iterators ----------

template <class T>
ArraySequence<T>::ArraySequenceIterator::ArraySequenceIterator(PointerType ptr) : current(ptr) {}

template <class T>
void ArraySequence<T>::ArraySequenceIterator::Increment() {
    ++current;
}

template <class T>
void ArraySequence<T>::ArraySequenceIterator::Decrement() {
    --current;
}

template <class T>
T &ArraySequence<T>::ArraySequenceIterator::Dereference() const {
    return *current;
}

template <class T>
bool ArraySequence<T>::ArraySequenceIterator::Equals(const Iterator<T> &other) const {
    const ArraySequenceIterator *other_dereferenced = dynamic_cast<const ArraySequenceIterator*>(&other);
    return other_dereferenced && (current == other_dereferenced->current);
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator ArraySequence<T>::begin() {
    if (GetLength() == 0) return ArraySequenceIterator(nullptr);
    return ArraySequenceIterator(&items.Get(0));
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator ArraySequence<T>::end() {
    if (GetLength() == 0) return ArraySequenceIterator(nullptr);
    return ArraySequenceIterator(&items.Get(GetLength() - 1) + 1);
}

// ---------- Const Iterators ----------

template <class T>
ArraySequence<T>::ConstArraySequenceIterator::ConstArraySequenceIterator(PointerType ptr) : current(ptr) {}

template <class T>
void ArraySequence<T>::ConstArraySequenceIterator::Increment() { 
    ++current; 
}

template <class T>
void ArraySequence<T>::ConstArraySequenceIterator::Decrement() { 
    --current; 
}

template <class T>
const T &ArraySequence<T>::ConstArraySequenceIterator::Dereference() const { 
    return *current; 
}

template <class T>
bool ArraySequence<T>::ConstArraySequenceIterator::Equals(const ConstIterator<T>& other) const {
    const ConstArraySequenceIterator* otherDerived = dynamic_cast<const ConstArraySequenceIterator*>(&other);
    return otherDerived && current == otherDerived->current;
}

template <class T>
typename ArraySequence<T>::ConstArraySequenceIterator ArraySequence<T>::begin() const {
    if (GetLength() == 0) return ConstArraySequenceIterator(nullptr);
    return ConstArraySequenceIterator(&items.Get(0));
}

template <class T>
typename ArraySequence<T>::ConstArraySequenceIterator ArraySequence<T>::end() const {
    if (GetLength() == 0) return ConstArraySequenceIterator(nullptr);
    return ConstArraySequenceIterator(&items.Get(GetLength() - 1) + 1);
}

// ---------- Operators ----------

template <class T>
ArraySequence<T> &ArraySequence<T>::operator=(const ArraySequence<T> &other) {
    if (this != &other) {
        items = other.items;
    }
    return *this;
}

template <class T>
bool ArraySequence<T>::operator==(const ArraySequence<T> &other) const {
    if (GetLength() != other.GetLength()) return false;
    for (int i = 0; i < GetLength(); i++) {
        if (Get(i) != other.Get(i)) return false;
    }
    return true;
}

template <class T>
bool ArraySequence<T>::operator!=(const ArraySequence<T> &other) const {
    return !(*this == other);
}

template <class T>
T &ArraySequence<T>::operator[](int index) {
    return items.Get(index);    
}

template <class T>
const T &ArraySequence<T>::operator[](int index) const {
    return items.Get(index);
}

template <class T>
ArraySequence<T> ArraySequence<T>::operator+(const ArraySequence<T> &other) const {
    int total_size = GetLength() + other.GetLength();
    T *temp = new T[total_size];
    for (int i = 0; i < GetLength(); ++i)
        temp[i] = Get(i);
    for (int i = 0; i < other.GetLength(); ++i)
        temp[GetLength() + i] = other.Get(i);
    ArraySequence<T> result(temp, total_size);
    delete[] temp;
    return result;
}

template <class T>
ArraySequence<T> &ArraySequence<T>::operator+=(const T& value) {
    return *Append(value);
}

template <class T>
ArraySequence<T> &ArraySequence<T>::operator+=(const ArraySequence<T> &other) {
    *this = *Concat(&other);
    return *this;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const ArraySequence<T> &seq) {
    os << "[";
    for (int i = 0; i < seq.GetLength(); i++) {
        os << seq.Get(i);
        if (i < seq.GetLength() - 1) os << ", ";
    }
    os << "]";
    return os;
}

// ---------- Mutable and Immutable Versions ----------

template <class T>
MutableArraySequence<T>::MutableArraySequence() : ArraySequence<T>() {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence<T> &list) : ArraySequence<T>(list) {}

template <class T>
MutableArraySequence<T>::~MutableArraySequence() {}

template <class T>
ArraySequence<T> *MutableArraySequence<T>::Instance() {
    return this;
}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence() : ArraySequence<T>() {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T> &list) : ArraySequence<T>(list) {}

template <class T>
ImmutableArraySequence<T>::~ImmutableArraySequence() {}

template <class T>
ArraySequence<T> *ImmutableArraySequence<T>::Instance() {
    return new ImmutableArraySequence<T>(*this);
}

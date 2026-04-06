#include "array_sequence.h"

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

template <class T>
ArraySequence<T> *ArraySequence<T>::Concat(Sequence<T> *list) {
    return Instance()->ConcatInternal(list);
}

// ---------- Internal Methods ----------
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

template <class T>
ArraySequence<T> *ArraySequence<T>::ConcatInternal(Sequence<T> *list) {
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

template <class T>
ArraySequence<T> *ArraySequence<T>::MapInternal(T (*func)(T)) {
    ArraySequence<T> *mapped_sequence = new ArraySequence<T>();
    for (int i = 0; i < GetLength(); i++) {
        mapped_sequence->Append(func(Get(i)));
    }
    *this = *mapped_sequence;
    delete mapped_sequence;
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::WhereInternal(bool (*predicate)(T)) {
    ArraySequence<T> *filtered_sequence = new ArraySequence<T>();
    for (int i = 0; i < GetLength(); i++) {
        if (predicate(Get(i))) filtered_sequence->Append(Get(i));
    }
    *this = *filtered_sequence;
    delete filtered_sequence;
    return this;
}

template <class T>
T ArraySequence<T>::ReduceInternal(T (*func)(T, T), T initial) {
    T result = initial;
    for (int i = 0; i < GetLength(); i++) {
        result = func(result, Get(i));
    }
    return result;
}

// ---------- Map, Where, Reduce ----------
template <class T>
ArraySequence<T> *ArraySequence<T>::Map(T (*func)(T)) {
    return Instance()->MapInternal(func);
}

template <class T>
ArraySequence<T> *ArraySequence<T>::Where(bool (*predicate)(T)) {
    return Instance()->WhereInternal(predicate);
}

template <class T>
T ArraySequence<T>::Reduce(T (*func)(T, T), T initial) {
    return Instance()->ReduceInternal(func, initial);
}

// ---------- Iterators ----------
template <class T>
ArraySequence<T>::ArraySequenceIterator::ArraySequenceIterator(PointerType ptr) : current(ptr) {}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator &ArraySequence<T>::ArraySequenceIterator::operator++() {
    current++;
    return *this;
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator &ArraySequence<T>::ArraySequenceIterator::operator+=(int n) {
    current += n;
    return *this;
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator ArraySequence<T>::ArraySequenceIterator::operator++(int) {
    ArraySequenceIterator tmp = *this;
    current++;
    return tmp;
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator &ArraySequence<T>::ArraySequenceIterator::operator--() {
    current--;
    return *this;
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator &ArraySequence<T>::ArraySequenceIterator::operator-=(int n) {
    current -= n;
    return *this;
} 

template <class T>
typename ArraySequence<T>::ArraySequenceIterator ArraySequence<T>::ArraySequenceIterator::operator--(int) {
    ArraySequenceIterator tmp = *this;
    current--;
    return tmp;
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator::PointerType ArraySequence<T>::ArraySequenceIterator::operator->() {
    return current;
} 

template <class T>
typename ArraySequence<T>::ArraySequenceIterator::ReferenceType ArraySequence<T>::ArraySequenceIterator::operator*() {
    return *current;
} 

template <class T>
bool ArraySequence<T>::ArraySequenceIterator::operator==(const ArraySequenceIterator &other) const {
    return current == other.current;
}

template <class T>
bool ArraySequence<T>::ArraySequenceIterator::operator!=(const ArraySequenceIterator &other) const {
    return current != other.current;
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator ArraySequence<T>::begin() {
    return ArraySequenceIterator(this->items);
}

template <class T>
typename ArraySequence<T>::ArraySequenceIterator ArraySequence<T>::end() {
    return ArraySequenceIterator(this->items + this->GetLength());
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
ArraySequence<T> &ArraySequence<T>::operator=(std::initializer_list<T> init_list) {
    items.Resize(init_list.size());
    int index = 0;
    for (const T &item : init_list) {
        items.Set(index++, item);
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
ArraySequence<T> &ArraySequence<T>::operator+(const ArraySequence<T> &other) const {
    return *Concat(other);
}

template <class T>
ArraySequence<T> &ArraySequence<T>::operator+=(const T& value) {
    return *Append(value);
}

template <class T>
ArraySequence<T> &ArraySequence<T>::operator+=(const ArraySequence<T> &other) {
    return *ConcatInternal(other);
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

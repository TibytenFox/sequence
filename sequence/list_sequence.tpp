#include "list_sequence.h"

// ---------- Constructors and Destructor ----------
template <class T>
ListSequence<T>::ListSequence() : items() {}

template <class T>
ListSequence<T>::ListSequence(T *items, int count) : items(items, count) {}

template <class T>
ListSequence<T>::ListSequence(const ListSequence<T> &list) : items(list.items) {}

template <class T>
ListSequence<T>::~ListSequence() {}

// ---------- Sequence Interface Implementation ----------
template <class T>
T ListSequence<T>::GetFirst() const { return items.GetFirst(); }

template <class T>
T ListSequence<T>::GetLast() const { return items.GetLast(); }

template <class T>
T ListSequence<T>::Get(int index) const { return items.Get(index); }

template <class T>
int ListSequence<T>::GetLength() const { return items.GetLength(); }

template <class T>
ListSequence<T> *ListSequence<T>::GetSubSequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= items.GetLength() || start_index > end_index) {
        throw std::out_of_range("Index out of range");
    }

    // Create a temporary C-array to hold the subsequence items
    ListSequence<T> *sub_sequence = new ListSequence<T>();
    int new_size = end_index - start_index + 1;
    for (int i = 0; i < new_size; i++) {
        // Using sub_sequence->items.Append() directly to avoid using the public Append method which may not modify the current instance
        sub_sequence->items.Append(items.Get(start_index + i));
    }
    return sub_sequence;
}

template <class T>
ListSequence<T> *ListSequence<T>::Append(T item) {
    return Instance()->AppendInternal(item);
}

template <class T>
ListSequence<T> *ListSequence<T>::Prepend(T item) {
    return Instance()->PrependInternal(item);
}

template <class T> 
ListSequence<T> *ListSequence<T>::InsertAt(T item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

template <class T>
ListSequence<T> *ListSequence<T>::Concat(Sequence<T> *list) {
    return Instance()->ConcatInternal(list);
}

template <class T>
ListSequence<T> *ListSequence<T>::Map(T (*func)(T)) {
    return Instance()->MapInternal(func);
}

template <class T>
ListSequence<T> *ListSequence<T>::Where(bool (*predicate)(T)) {
    return Instance()->WhereInternal(predicate);
}  

template <class T>
T ListSequence<T>::Reduce(T (*func)(T, T), T initial) {
    return Instance()->ReduceInternal(func, initial);
}

// ---------- Internal Methods ----------
template <class T>
ListSequence<T> *ListSequence<T>::AppendInternal(T item) { 
    items.Append(item);
    return this;
 }

template <class T>
ListSequence<T> *ListSequence<T>::PrependInternal(T item) { 
    items.Prepend(item); 
    return this; 
}

template <class T>
ListSequence<T> *ListSequence<T>::InsertAtInternal(T item, int index) { 
    items.InsertAt(item, index); 
    return this; 
}

template <class T>
ListSequence<T> *ListSequence<T>::ConcatInternal(Sequence<T> *list) {
     // Create a temporary C-array to hold the subsequence items
    ListSequence<T> *new_sequence = new ListSequence<T>(*this);
    for (int i = 0; i < list->GetLength(); i++) {
        // Using sub_sequence->items.Append() directly to avoid using the public Append method which may not modify the current instance
        new_sequence->items.Append(list->Get(i));
    }
    return new_sequence;
}

template <class T>
ListSequence<T> *ListSequence<T>::MapInternal(T (*func)(T)) {
    ListSequence<T> *mapped_sequence = new ListSequence<T>();
    for (int i = 0; i < GetLength(); i++) {
        mapped_sequence->AppendInternal(func(Get(i)));
    }
    *this = *mapped_sequence;
    delete mapped_sequence;
    return this;
}

template <class T>
ListSequence<T> *ListSequence<T>::WhereInternal(bool (*predicate)(T)) {
    ListSequence<T> *filtered_sequence = new ListSequence<T>();
    for (int i = 0; i < GetLength(); i++) {
        if (predicate(Get(i))) filtered_sequence->AppendInternal(Get(i));
    }
    *this = *filtered_sequence;
    delete filtered_sequence;
    return this;
}

template <class T>
T ListSequence<T>::ReduceInternal(T (*func)(T, T), T initial) {
    T result = initial;
    for (int i = 0; i < GetLength(); i++) {
        result = func(result, Get(i));
    }
    return result;
}

// ---------- Iterators ----------
template <class T>
ListSequence<T>::ListSequenceIterator::ListSequenceIterator(NodePointerType ptr) : current(ptr) {}

template <class T>
typename ListSequence<T>::ListSequenceIterator &ListSequence<T>::ListSequenceIterator::operator++() {
    current = current->next;
    return *this;
}

template <class T>
typename ListSequence<T>::ListSequenceIterator &ListSequence<T>::ListSequenceIterator::operator+=(int n) {
    for (int i = 0; i < n; i++) {
        if (current == nullptr) break;
        current = current->next;
    }
    return *this;
}

template <class T>
typename ListSequence<T>::ListSequenceIterator ListSequence<T>::ListSequenceIterator::operator++(int) {
    ListSequenceIterator tmp = *this; 
    ++current; 
    return tmp;
}

template <class T>
typename ListSequence<T>::ListSequenceIterator &ListSequence<T>::ListSequenceIterator::operator--() {
    current = current->prev;
    return *this;
}

template <class T>
typename ListSequence<T>::ListSequenceIterator &ListSequence<T>::ListSequenceIterator::operator-=(int n) {
    for (int i = 0; i < n; i++) {
        if (current == nullptr) break;
        current = current->prev;
    }
    return *this;
}

template <class T>
typename ListSequence<T>::ListSequenceIterator ListSequence<T>::ListSequenceIterator::operator--(int) {
    ListSequenceIterator tmp = *this; 
    --current; 
    return tmp;
}

template <class T>
typename ListSequence<T>::ListSequenceIterator::PointerType ListSequence<T>::ListSequenceIterator::operator->() {
    return &(current->value);
}

template <class T>
typename ListSequence<T>::ListSequenceIterator::ReferenceType ListSequence<T>::ListSequenceIterator::operator*() {
    return current->value;
}

template <class T>
bool ListSequence<T>::ListSequenceIterator::operator==(const ListSequenceIterator &other) const {
    return current == other.current;
}

template <class T>
bool ListSequence<T>::ListSequenceIterator::operator!=(const ListSequenceIterator &other) const {
    return !(current == other.current);
}

template <class T>
typename ListSequence<T>::ListSequenceIterator ListSequence<T>::begin() {
    return ListSequenceIterator(items.GetHead());
}

template <class T>
typename ListSequence<T>::ListSequenceIterator ListSequence<T>::end() {
    return ListSequenceIterator(items.GetTail()->next);
}

// ---------- Operators ----------
template <class T>
ListSequence<T> &ListSequence<T>::operator=(const ListSequence<T> &other) {
    if (this != &other) {
        items = other.items;
    }
    return *this;
}

template <class T>
ListSequence<T> &ListSequence<T>::operator=(std::initializer_list<T> init_list) {
    if (&items != nullptr) delete &items;

    for (const T &item : init_list) {
       this->Append(item);
    }
    return *this;
}

template <class T>
bool ListSequence<T>::operator==(const ListSequence<T> &other) const {
    if (this->GetLength() != other.GetLength()) return false;

    ListSequenceIterator this_current = this->begin();
    ListSequenceIterator other_current = other.begin();

    while (this_current != this->end()) {
        if (*this_current != *other_current) return false;
        this_current++;
        other_current++;
    }

    return true;
}

template <class T>
bool ListSequence<T>::operator!=(const ListSequence<T> &other) const {
    return !(*this == other);
}

template <class T>
T &ListSequence<T>::operator[](int index) {
    ListSequenceIterator current = this->begin();
    current += index;
    return *current;
}

template <class T>
const T &ListSequence<T>::operator[](int index) const {
    ListSequenceIterator current = this->begin();
    current += index;
    return *current;
}

template <class T>
ListSequence<T> &ListSequence<T>::operator+(const ListSequence<T> &other) const {
    ListSequence<T> *result = new ListSequence<T>(*this);
    result->Concat(&other);
    return *result;
}

template <class T>
ListSequence<T> &ListSequence<T>::operator+=(const T &value) {
    this->Append(value);
    return *this;
}

template <class T>
ListSequence<T> &ListSequence<T>::operator+=(const ListSequence<T> &other) {
    this->Concat(&other);
    return *this;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const ListSequence<T> &seq) {
    os << '[';
    for (int i = 0; i < seq.GetLength(); i++) {
        os << seq.Get(i);
        if (i != seq.GetLength() - 1) os << ", ";
    }
    os << ']';
    return os;
}

// ---------- Mutable and Immutable Versions ----------
template <class T>
MutableListSequence<T>::MutableListSequence() : ListSequence<T>() {}

template <class T>
MutableListSequence<T>::MutableListSequence(T *items, int count) : ListSequence<T>(items, count) {}

template <class T>
MutableListSequence<T>::MutableListSequence(const MutableListSequence<T> &list) : ListSequence<T>(list) {}

template <class T>
MutableListSequence<T>::~MutableListSequence() {}

template <class T>
ListSequence<T> *MutableListSequence<T>::Instance() {
    return this;
}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence() : ListSequence<T>() {}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence(T *items, int count) : ListSequence<T>(items, count) {}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence(const ImmutableListSequence<T> &list) : ListSequence<T>(list) {}

template <class T>
ImmutableListSequence<T>::~ImmutableListSequence() {}

template <class T>
ListSequence<T> *ImmutableListSequence<T>::Instance() {
    return new ImmutableListSequence<T>(*this);
}
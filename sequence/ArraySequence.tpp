#include "ArraySequence.hpp"

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
const T &ArraySequence<T>::GetFirst() const { return this->items.Get(0); }

template <class T>
const T &ArraySequence<T>::GetLast() const { return this->items.Get(items.GetSize() - 1); }

template <class T>
const T &ArraySequence<T>::Get(int index) const { return this->items.Get(index); }

template <class T>
int ArraySequence<T>::GetLength() const { return this->items.GetSize(); }

// Caller is responsible for deleting the returned pointer
template <class T>
Sequence<T> *ArraySequence<T>::GetSubsequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= items.GetSize() || start_index > end_index) {
        throw IndexOutOfRange("GetSubSequence: Invalid indicies");
    }

    int new_size = end_index - start_index + 1;
    ISequenceBuilder<T> *builder = this->CreateBuilder();
    
    for (int i = 0; i < new_size; i++) {
        builder->Append(this->items.Get(start_index + i));
    }

    Sequence<T> *sub_sequence = builder->Build();
    delete builder;
    return sub_sequence;
}

// Using instance 
template <class T>
Sequence<T> *ArraySequence<T>::Append(T item) {
    return Instance()->AppendInternal(item);
}

template <class T>
Sequence<T> *ArraySequence<T>::Prepend(T item) {
    return Instance()->PrependInternal(item);
}

template <class T>
Sequence<T> *ArraySequence<T>::InsertAt(T item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

// Returns new heap-allocated sequence
template <class T>
Sequence<T> *ArraySequence<T>::Concat(const Sequence<T> *list) const {
    ISequenceBuilder<T> *builder = this->CreateBuilder();

    for (int i = 0; i < this->GetLength(); i++) {
        builder->Append(this->Get(i));
    }
    for (int i = 0; i < list->GetLength(); i++) {
       builder->Append(list->Get(i));
    }

    Sequence<T> *concat_sequence = builder->Build();
    delete builder;
    return concat_sequence;
}

// ---------- Internal Methods (modify in place) ----------

template <class T>
ArraySequence<T> *ArraySequence<T>::AppendInternal(T item) {
    this->items.Resize(this->items.GetSize() + 1);
    this->items.Set(this->items.GetSize() - 1, item);
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::PrependInternal(T item) {
    this->items.Resize(this->items.GetSize() + 1);
    for (int i = this->items.GetSize() - 1; i > 0; i--) {
        this->items.Set(i, this->items.Get(i - 1));
    }
    this->items.Set(0, item);
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::InsertAtInternal(T item, int index) {
    if (index < 0 || index > this->items.GetSize()) {
        throw IndexOutOfRange("InsertAt: invalid index");
    }
    this->items.Resize(this->items.GetSize() + 1);
    for (int i = this->items.GetSize() - 1; i > index; i--) {
        this->items.Set(i, items.Get(i - 1));
    }
    this->items.Set(index, item);
    return this;
}

// ---------- Iterators ----------

template <class T>
ArraySequence<T>::Enumerator::Enumerator(PointerType ptr) : current(ptr) {}

template <class T>
void ArraySequence<T>::Enumerator::Increment() {
    ++current;
}

template <class T>
void ArraySequence<T>::Enumerator::Decrement() {
    --current;
}

template <class T>
T &ArraySequence<T>::Enumerator::Dereference() const {
    return *current;
}

template <class T>
bool ArraySequence<T>::Enumerator::Equals(const IEnumerator<T> &other) const {
    const Enumerator *other_dereferenced = dynamic_cast<const Enumerator*>(&other);
    return other_dereferenced && (current == other_dereferenced->current);
}

template <class T>
typename ArraySequence<T>::Enumerator *ArraySequence<T>::begin() {
    if (GetLength() == 0) return new Enumerator(nullptr);
    return new Enumerator(&(this->items[0]));
}

template <class T>
typename ArraySequence<T>::Enumerator *ArraySequence<T>::end() {
    if (GetLength() == 0) return new Enumerator(nullptr);
    return new Enumerator(&(items[(GetLength() - 1) + 1]));
}

// ---------- Const Iterators ----------

template <class T>
ArraySequence<T>::ConstEnumerator::ConstEnumerator(PointerType ptr) : current(ptr) {}

template <class T>
void ArraySequence<T>::ConstEnumerator::Increment() { 
    ++current; 
}

template <class T>
void ArraySequence<T>::ConstEnumerator::Decrement() { 
    --current; 
}

template <class T>
const T &ArraySequence<T>::ConstEnumerator::Dereference() const { 
    return *current; 
}

template <class T>
bool ArraySequence<T>::ConstEnumerator::Equals(const IConstEnumerator<T>& other) const {
    const ConstEnumerator* otherDerived = dynamic_cast<const ConstEnumerator*>(&other);
    return otherDerived && (current == otherDerived->current);
}

template <class T>
typename ArraySequence<T>::ConstEnumerator *ArraySequence<T>::begin() const {
    if (GetLength() == 0) return new ConstEnumerator(nullptr);
    return new ConstEnumerator(&items.Get(0));
}

template <class T>
typename ArraySequence<T>::ConstEnumerator *ArraySequence<T>::end() const {
    if (GetLength() == 0) return new ConstEnumerator(nullptr);
    return new ConstEnumerator(&items.Get(GetLength() - 1) + 1);
}

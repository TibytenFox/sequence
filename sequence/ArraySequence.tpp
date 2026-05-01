#include "ArraySequence.hpp"

#define DEFAULT_CAPACIOTY 10

// ---------- Constructors and Destructor ----------

template <class T>
ArraySequence<T>::ArraySequence() : size(0), items(DEFAULT_CAPACIOTY) {}

template <class T>
ArraySequence<T>::ArraySequence(int count) : size(0), items(count) {}

template <class T>
ArraySequence<T>::ArraySequence(T *items, int count) : size(count), items(items, count) {}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T> &list) : size(list.GetLength()), items(list.items) {}

template <class T>
ArraySequence<T>::~ArraySequence() {}

// ---------- Sequence Interface Implementation ----------

template <class T>
const T &ArraySequence<T>::GetFirst() const { 
    if (size == 0) throw EmptyCollectionError("GetFirst(): Empty collection");
    return this->items.Get(0); 
}

template <class T>
const T &ArraySequence<T>::GetLast() const { 
    if (size == 0) throw EmptyCollectionError("GetFirst(): Empty collection");
    return this->items.Get(this->size - 1); 
}

template <class T>
const T &ArraySequence<T>::Get(int index) const { 
    if (index < 0 || index >= size) throw IndexOutOfRange("Get(): Index out of range");
    return this->items.Get(index); 
}

template <class T>
int ArraySequence<T>::GetLength() const { 
    return this->size; 
}

// Caller is responsible for deleting the returned pointer
template <class T>
Sequence<T> *ArraySequence<T>::GetSubsequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= this->GetLength() || start_index > end_index) {
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
Sequence<T> *ArraySequence<T>::Concat(const Sequence<T> &list) const {
    ISequenceBuilder<T> *builder = this->CreateBuilder();

    for (int i = 0; i < this->GetLength(); i++) {
        builder->Append(this->Get(i));
    }
    for (int i = 0; i < list.GetLength(); i++) {
       builder->Append(list.Get(i));
    }

    Sequence<T> *concat_sequence = builder->Build();
    delete builder;
    return concat_sequence;
}

// ---------- Iterators ----------

template <class T>
IEnumerator<T> *ArraySequence<T>::GetEnumerator() const {
    return new Enumerator(this);
}

// ---------- Internal Methods (modify in place) ----------

template <class T>
ArraySequence<T> *ArraySequence<T>::AppendInternal(T item) {
    if (this->items.GetSize() <= this->size) this->items.Resize(this->size + 1);
    this->size++;
    this->items.Set(this->size - 1, item);
    return this;
}

template <class T>
ArraySequence<T> *ArraySequence<T>::PrependInternal(T item) {
    if (this->items.GetSize() <= this->size) this->items.Resize(this->size + 1);
    this->size++;
    for (int i = this->size - 1; i > 0; i--) {
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
    if (this->items.GetSize() <= this->size) this->items.Resize(this->size + 1);
    this->size++;
    for (int i = this->size - 1; i > index; i--) {
        this->items.Set(i, this->items.Get(i - 1));
    }
    this->items.Set(index, item);
    return this;
}

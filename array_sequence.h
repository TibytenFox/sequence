#pragma once
#include "sequence.h"
#include "dynamic_array.h"

template <class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T> *items;
public:
    ArraySequence();
    ArraySequence(T *items, int count);
    ArraySequence(const ArraySequence<T> &list);
    ~ArraySequence();

    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;
    ArraySequence<T> *GetSubSequence(int start_index, int end_index) const;
    int GetLength() const;
    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    ArraySequence<T> *Concat(ArraySequence<T> *list);
};

#include "array_sequence.tpp"
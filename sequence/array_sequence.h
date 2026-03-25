#pragma once
#include "sequence.h"
#include "../dynamic_array.h"

template <class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T> items;
protected:
    ArraySequence<T> *AppendInternal(T item);
    ArraySequence<T> *PrependInternal(T item);
    ArraySequence<T> *InsertAtInternal(T item, int index);
    ArraySequence<T> *ConcatInternal(Sequence<T> *list);
public:
    ArraySequence();
    ArraySequence(T *items, int count);
    ArraySequence(const ArraySequence<T> &list);
    ~ArraySequence();

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;
    int GetLength() const override;
    ArraySequence<T> *GetSubSequence(int start_index, int end_index) const override;
    ArraySequence<T> *Append(T item) override;
    ArraySequence<T> *Prepend(T item) override;
    ArraySequence<T> *InsertAt(T item, int index) override;
    ArraySequence<T> *Concat(Sequence<T> *list) override;

    ArraySequence<T> &operator=(const ArraySequence<T> &other);

    virtual ArraySequence<T> *Instance() { return this; }
};

template <class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence();
    MutableArraySequence(T *items, int count);
    MutableArraySequence(const MutableArraySequence<T> &list);
    ~MutableArraySequence();

    virtual ArraySequence<T> *Instance() override;
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence();
    ImmutableArraySequence(T *items, int count);
    ImmutableArraySequence(const ImmutableArraySequence<T> &list);
    ~ImmutableArraySequence();

    virtual ArraySequence<T> *Instance() override;
};

#include "array_sequence.tpp"
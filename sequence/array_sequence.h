#pragma once
#include "sequence.h"
#include "../dynamic_array.h"

template <class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T> items;

    class ArraySequenceIterator {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using ReferenceType = ValueType&;

        ArraySequenceIterator(PointerType ptr);
        ArraySequenceIterator &operator++();
        ArraySequenceIterator &operator+=(int n);
        ArraySequenceIterator operator++(int);
        ArraySequenceIterator &operator--();
        ArraySequenceIterator &operator-=(int n);
        ArraySequenceIterator operator--(int);
        PointerType operator->();
        ReferenceType operator*();
        bool operator==(const ArraySequenceIterator &other) const;
        bool operator!=(const ArraySequenceIterator &other) const;
    private:
        PointerType current;
    };
protected:
    ArraySequence<T> *AppendInternal(T item);
    ArraySequence<T> *PrependInternal(T item);
    ArraySequence<T> *InsertAtInternal(T item, int index);
    ArraySequence<T> *ConcatInternal(Sequence<T> *list);
    ArraySequence<T> *MapInternal(T (*func)(T));
    ArraySequence<T> *WhereInternal(bool (*predicate)(T));
    T ReduceInternal(T (*func)(T, T), T initial);
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

    ArraySequence<T> *Map(T (*func)(T)) override;
    ArraySequence<T> *Where(bool (*predicate)(T)) override;
    T Reduce(T (*func)(T, T), T initial) override;

    ArraySequenceIterator begin();
    ArraySequenceIterator end();

    ArraySequence<T> &operator=(const ArraySequence<T> &other);
    ArraySequence<T> &operator=(std::initializer_list<T> init_list);
    bool operator==(const ArraySequence<T> &other) const;
    bool operator!=(const ArraySequence<T> &other) const;
    T &operator[](int index);
    const T &operator[](int index) const;
    ArraySequence<T> &operator+(const ArraySequence<T> &other) const;
    ArraySequence<T> &operator+=(const T &value);
    ArraySequence<T> &operator+=(const ArraySequence<T> &other);
    friend std::ostream &operator<<(std::ostream &os, const ArraySequence<T> &seq);

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
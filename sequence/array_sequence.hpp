#pragma once
#include "sequence.hpp"
#include "../dynamic_array.hpp"
#include "iterator.hpp"

template <class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T> items;    // Using dynamic array

    // Iterator for ArraySequence
    class ArraySequenceIterator : public Iterator<T> {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using ReferenceType = ValueType&;

        ArraySequenceIterator(PointerType ptr);
        
        void Increment() override;
        void Decrement() override;
        T &Dereference() const override;
        bool Equals(const Iterator<T> &other) const override;
    private:
        PointerType current;
    };

    // Const Iterator for ArraySequence
    class ConstArraySequenceIterator : public ConstIterator<T> {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using ReferenceType = ValueType&;

        ConstArraySequenceIterator(PointerType ptr);

        void Increment() override;
        void Decrement() override;
        const T &Dereference() const override;
        bool Equals(const ConstIterator<T> &other) const override;
    private:
        const T* current;
    };
public:
    // ---------- Constructors and Destructor ----------
    ArraySequence();
    ArraySequence(T *items, int count);
    ArraySequence(const ArraySequence<T> &list);
    ~ArraySequence();

    // ---------- Sequence overrides ----------
    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;
    int GetLength() const override;
    ArraySequence<T> *GetSubSequence(int start_index, int end_index) const override;
    ArraySequence<T> *Append(T item) override;
    ArraySequence<T> *Prepend(T item) override;
    ArraySequence<T> *InsertAt(T item, int index) override;
    ArraySequence<T> *Concat(const Sequence<T> *list) const override;

    // ---------- Map-Reduce ----------
    template <class T2>
    ArraySequence<T2> *Map(T2 (*func)(T)) const;
    ArraySequence<T> *Where(bool (*predicate)(T)) const override;
    template <class T2>
    T2 Reduce(T2 (*func)(T2, T), T2 initial) const;

    // ---------- Iterators ----------
    ArraySequenceIterator begin();
    ArraySequenceIterator end();
    ConstArraySequenceIterator begin() const;
    ConstArraySequenceIterator end() const;

    // ---------- Operators ----------
    ArraySequence<T> &operator=(const ArraySequence<T> &other);
    bool operator==(const ArraySequence<T> &other) const;
    bool operator!=(const ArraySequence<T> &other) const;
    T &operator[](int index);
    const T &operator[](int index) const;
    ArraySequence<T> operator+(const ArraySequence<T> &other) const;
    ArraySequence<T> &operator+=(const T &value);
    ArraySequence<T> &operator+=(const ArraySequence<T> &other);
    friend std::ostream &operator<<(std::ostream &os, const ArraySequence<T> &seq);

    /*
        Returns pointer to the object that should be used for modifying operations
        For MutableArraySequence returns this; for ImmutableArraySequence returns a copy
    */
    virtual ArraySequence<T> *Instance() { return this; }
protected:
    // Internal modifying methods that actually change the array.
    ArraySequence<T> *AppendInternal(T item);
    ArraySequence<T> *PrependInternal(T item);
    ArraySequence<T> *InsertAtInternal(T item, int index);
};

// ---------- Mutable version ----------
template <class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence();
    MutableArraySequence(T *items, int count);
    MutableArraySequence(const MutableArraySequence<T> &list);
    ~MutableArraySequence();

    virtual ArraySequence<T> *Instance() override; // Returns this
};

// ---------- Immutable version ----------
template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence();
    ImmutableArraySequence(T *items, int count);
    ImmutableArraySequence(const ImmutableArraySequence<T> &list);
    ~ImmutableArraySequence();

    virtual ArraySequence<T> *Instance() override; // Returns a new copy
};

#include "array_sequence.tpp"
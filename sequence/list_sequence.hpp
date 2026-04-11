#pragma once
#include "sequence.hpp"
#include "../linked_list.hpp"
#include "iterator.hpp"

template <class T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T> items;   // Linked list

    // Iterator for ListSequence
    class ListSequenceIterator : public Iterator<T> {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using NodePointerType = typename LinkedList<T>::Node*;
        using ReferenceType = ValueType&;

        ListSequenceIterator(NodePointerType ptr);

        void Increment() override;
        void Decrement() override;
        T &Dereference() const override;
        bool Equals(const Iterator<T> &other) const override;
    private:
        NodePointerType current;
    };

    // Const Iterator for ListSequence
    class ConstListSequenceIterator : public ConstIterator<T> {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using NodePointerType = typename LinkedList<T>::Node*;
        using ReferenceType = ValueType&;

        ConstListSequenceIterator(NodePointerType ptr);

        void Increment() override;
        void Decrement() override;
        const T &Dereference() const override;
        bool Equals(const ConstIterator<T> &other) const override;
    private:
        NodePointerType current;
    };
public:
    // ---------- Constructors and Destructor ----------
    ListSequence();
    ListSequence(T *items, int count);
    ListSequence(const ListSequence<T> &list);
    ~ListSequence();

    // ---------- Sequence overrides ----------
    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;
    int GetLength() const override;
    ListSequence<T> *GetSubSequence(int start_index, int end_index) const override;
    ListSequence<T> *Append(T item) override;
    ListSequence<T> *Prepend(T item) override;
    ListSequence<T> *InsertAt(T item, int index) override;
    ListSequence<T> *Concat(const Sequence<T> *list) const override;

    // ---------- Map-Reduce ----------
    template <class T2>
    ListSequence<T2> *Map(T2 (*func)(T)) const;
    ListSequence<T> *Where(bool (*predicate)(T)) const override;
    template <class T2>
    T2 Reduce(T2 (*func)(T2, T), T2 initial) const;

    // ---------- Iterators ----------
    ListSequenceIterator begin();
    ListSequenceIterator end();
    ConstListSequenceIterator begin() const;
    ConstListSequenceIterator end() const;

    // ---------- Operators ----------
    ListSequence<T> &operator=(const ListSequence<T> &other);
    bool operator==(const ListSequence<T> &other) const;
    bool operator!=(const ListSequence<T> &other) const;
    T &operator[](int index);
    const T &operator[](int index) const;
    ListSequence<T> operator+(const ListSequence<T> &other) const;
    ListSequence<T> &operator+=(const T &value);
    ListSequence<T> &operator+=(const ListSequence<T> &other);
    friend std::ostream &operator<<(std::ostream &os, const ListSequence<T> &seq);

    /*
        Returns pointer to the object that should be used for modifying operations
        For MutableArraySequence returns this; for ImmutableArraySequence returns a copy
    */
    virtual ListSequence<T> *Instance() { return this; }
protected:
    ListSequence<T> *AppendInternal(T item);
    ListSequence<T> *PrependInternal(T item);
    ListSequence<T> *InsertAtInternal(T item, int index);
};

// ---------- Mutable version ----------
template <class T>
class MutableListSequence : public ListSequence<T> {
public:
    MutableListSequence();
    MutableListSequence(T *items, int count);
    MutableListSequence(const MutableListSequence<T> &list);
    ~MutableListSequence();

    virtual ListSequence<T> *Instance() override; // Returns this
};

// ---------- Immutable version ----------
template <class T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence();
    ImmutableListSequence(T *items, int count);
    ImmutableListSequence(const ImmutableListSequence<T> &list);
    ~ImmutableListSequence();

    virtual ListSequence<T> *Instance() override; // Returncs a new copy
};

#include "list_sequence.tpp"
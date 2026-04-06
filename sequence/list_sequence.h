#pragma once
#include "sequence.h"
#include "../linked_list.h"

template <class T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T> items;

    class ListSequenceIterator {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using NodePointerType = Node<ValueType>*;
        using ReferenceType = ValueType&;

        ListSequenceIterator(NodePointerType ptr);
        ListSequenceIterator &operator++();
        ListSequenceIterator &operator+=(int n);
        ListSequenceIterator operator++(int);
        ListSequenceIterator &operator--();
        ListSequenceIterator &operator-=(int n);
        ListSequenceIterator operator--(int);
        PointerType operator->();
        ReferenceType operator*();
        bool operator==(const ListSequenceIterator &other) const;
        bool operator!=(const ListSequenceIterator &other) const;
    private:
        NodePointerType current;
    };
protected:
    ListSequence<T> *AppendInternal(T item);
    ListSequence<T> *PrependInternal(T item);
    ListSequence<T> *InsertAtInternal(T item, int index);
    ListSequence<T> *ConcatInternal(Sequence<T> *list);
    ListSequence<T> *MapInternal(T (*func)(T));
    ListSequence<T> *WhereInternal(bool (*predicate)(T));
    T ReduceInternal(T (*func)(T, T), T initial);
public:
    //TODO: use explicit constructors
    ListSequence();
    ListSequence(T *items, int count);
    ListSequence(const ListSequence<T> &list);
    ~ListSequence();

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;
    int GetLength() const override;
    ListSequence<T> *GetSubSequence(int start_index, int end_index) const override;
    ListSequence<T> *Append(T item) override;
    ListSequence<T> *Prepend(T item) override;
    ListSequence<T> *InsertAt(T item, int index) override;
    ListSequence<T> *Concat(Sequence<T> *list) override;

    ListSequence<T> *Map(T (*func)(T)) override;
    ListSequence<T> *Where(bool (*predicate)(T)) override;
    T Reduce(T (*func)(T, T), T initial) override;

    ListSequenceIterator begin();
    ListSequenceIterator end();

    ListSequence<T> &operator=(const ListSequence<T> &other);
    ListSequence<T> &operator=(std::initializer_list<T> init_list);
    bool operator==(const ListSequence<T> &other) const;
    bool operator!=(const ListSequence<T> &other) const;
    T &operator[](int index);
    const T &operator[](int index) const;
    ListSequence<T> &operator+(const ListSequence<T> &other) const;
    ListSequence<T> &operator+=(const T &value);
    ListSequence<T> &operator+=(const ListSequence<T> &other);
    friend std::ostream &operator<<(std::ostream &os, const ListSequence<T> &seq);

    virtual ListSequence<T> *Instance() { return this; }
};

template <class T>
class MutableListSequence : public ListSequence<T> {
public:
    MutableListSequence();
    MutableListSequence(T *items, int count);
    MutableListSequence(const MutableListSequence<T> &list);
    ~MutableListSequence();

    virtual ListSequence<T> *Instance() override;
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence();
    ImmutableListSequence(T *items, int count);
    ImmutableListSequence(const ImmutableListSequence<T> &list);
    ~ImmutableListSequence();

    virtual ListSequence<T> *Instance() override;
};

#include "list_sequence.tpp"
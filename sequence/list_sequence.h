#pragma once
#include "sequence.h"
#include "../linked_list.h"

template <class T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T> items;
protected:
    ListSequence<T> *AppendInternal(T item);
    ListSequence<T> *PrependInternal(T item);
    ListSequence<T> *InsertAtInternal(T item, int index);
    ListSequence<T> *ConcatInternal(Sequence<T> *list);
public:
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

    ListSequence<T> &operator=(const ListSequence<T> &other);

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
#pragma once
#include "sequence.h"
#include "linked_list.h"

template <class T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T> *items;
public:
    ListSequence();
    ListSequence(T *items, int count);
    ListSequence(const ListSequence<T> &list);
    ~ListSequence();

    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;
    ListSequence<T> *GetSubSequence(int start_index, int end_index) const;
    int GetLength() const;
    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    ListSequence<T> *Concat(ListSequence<T> *list);
};

#include "list_sequence.tpp"
#pragma once
#include "list_sequence.h"

template <class T>
ListSequence<T>::ListSequence() : items(new LinkedList<T>()) {}

template <class T>
ListSequence<T>::ListSequence(T *items, int count) : items(new LinkedList<T>(items, count)) {}

template <class T>
ListSequence<T>::ListSequence(const ListSequence<T> &list) : items(new LinkedList<T>(*list.items)) {}

template <class T>
ListSequence<T>::~ListSequence() {
    delete items;
}

template <class T>
T ListSequence<T>::GetFirst() const {
    return items->GetFirst();
}

template <class T>
T ListSequence<T>::GetLast() const {
    return items->GetLast();
}

template <class T>
T ListSequence<T>::Get(int index) const {
    return items->Get(index);
}

template <class T>
ListSequence<T> *ListSequence<T>::GetSubSequence(int start_index, int end_index) const {
    ListSequence<T> *sublist = new ListSequence<T>();
    sublist->items = items->GetSubList(start_index, end_index);
    return sublist;
}

template <class T>
int ListSequence<T>::GetLength() const {
    return items->GetLength();
}

template <class T>
void ListSequence<T>::Append(T item) {
    items->Append(item);
}

template <class T>
void ListSequence<T>::Prepend(T item) {
    items->Prepend(item);
}

template <class T>
void ListSequence<T>::InsertAt(T item, int index) {
    items->InsertAt(item, index);
}

template <class T>
ListSequence<T> *ListSequence<T>::Concat(ListSequence<T> *list) {
    ListSequence<T> *new_list = new ListSequence<T>(*this);
    new_list->items = items->Concat(list->items);
    return new_list;
}
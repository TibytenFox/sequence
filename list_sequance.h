#pragma once
#include "sequence.h"
#include "linked_list.h"

template <class T> 
class ListSequance : public Sequance<T> {
private:
    LinkedList<T> *items;
public:
    ListSequance() : items(new LinkedList<T>()) {}
    ListSequance(T *items, int count) : items(new LinkedList<T>(items, count)) {}
    ListSequance(const ListSequance<T> &list) : items(new LinkedList<T>(*list.items)) {}
    T GetFirst() const { return items->GetFirst(); }
    T GetLast() const { return items->GetLast(); }
    T Get(int index) const { return items->Get(index); }
    ListSequance<T> *GetSubSequance(int start_index, int end_index) {
        ListSequance<T> *sublist = new ListSequance<T>();
        sublist->items = items->GetSubList(start_index, end_index);
        return sublist;
    }
    int GetLength() const { return items->GetLength(); }
    void Append(T item) { items->Append(item); }
    void Prepend(T item) { items->Prepend(item); }
    void InsertAt(T item, int index) { items->InsertAt(item, index); } 
    ListSequance<T> *Concat(ListSequance<T> *list) {
        ListSequance<T> *new_list = new ListSequance<T>(*this);
        new_list->items = items->Concat(list->items);
        return new_list;
    }
    ~ListSequance() { delete items; }
};
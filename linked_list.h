#pragma once
#include <stdexcept>

template<class T>
struct Node {
    T value;
    struct Node<T> *next;
};

template<class T>
class LinkedList {
private:
    int size;
    Node<T> *first;
public:
    LinkedList();
    LinkedList(T *items, int count);
    LinkedList(const LinkedList<T> &list);
    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;
    LinkedList<T> *GetSubList(int start_index, int end_index) const;
    int GetLength() const;
    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    LinkedList<T> *Concat(LinkedList<T> *list);
    ~LinkedList();
};

#include "linked_list.tpp"
#pragma once

template <class T>
class LinkedList {
public:
    // ---------- Node structure ----------
    struct Node {
        T value;
        struct Node *next;
        struct Node *prev;
    };
public:
    // ---------- Constructors / Destructor ----------
    LinkedList();                              // Empty array
    LinkedList(T *items, int count);           // Copy from C-array
    LinkedList(const LinkedList<T> &list);     // Copy constructor
    ~LinkedList();                             // Destructor

    // ---------- Accessors ----------
    T GetFirst() const;                        // Get first element
    T GetLast() const;                         // Get last element
    T Get(int index) const;                    // Get element at index
    int GetLength() const;                     // Return current size
    LinkedList<T> *GetSubList(int start_index, int end_index) const; // Returns new heap-allocated list

    // ---------- Modifiers ----------
    void Append(T item);                       // Add element at the end
    void Prepend(T item);                      // Add element at the beginning
    void InsertAt(T item, int index);          // Insert at position
    LinkedList<T> *Concat(LinkedList<T> *list); // Returns new heap-allocated list

    // ---------- Iteration support ----------
    Node *GetHead() const;                     // Raw pointer to first node (nullptr if empty)
    Node *GetTail() const;                     // Raw pointer to last node (nullptr if empty)

    // ---------- Operators ----------
    LinkedList<T> &operator=(const LinkedList<T> &other);

private:
    int size;      // Number of nodes
    Node *head;    // First node (nullptr if empty)
    Node *tail;    // Last node (nullptr if empty)
};

#include "linked_list.tpp"
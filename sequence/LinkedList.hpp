#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "Exceptions.hpp"

template <class T>
class LinkedList {
public:
    // ---------- Node structure ----------
    struct Node {
        T value;
        struct Node *next;
        struct Node *prev;
    };
private:
    int size;      // Number of nodes
    Node *head;    // First node (nullptr if empty)
    Node *tail;    // Last node (nullptr if empty)
public:
    // ---------- Constructors / Destructor ----------
    LinkedList();                              // Empty array
    LinkedList(T *items, int count);           // Copy from C-array
    LinkedList(const LinkedList<T> &list);     // Copy constructor
    ~LinkedList();                             // Destructor

    // ---------- Accessors ----------
    const T &GetFirst() const;                        // Get first element
    const T &GetLast() const;                         // Get last element
    const T &Get(int index) const;                    // Get element at index
    int GetLength() const;                            // Return current size
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
};

#include "LinkedList.tpp"
#endif // LINKED_LIST_HPP
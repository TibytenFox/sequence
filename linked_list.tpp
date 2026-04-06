#include "linked_list.h"
#include <iostream>

// --------- Constructors and Destructor ----------
template <class T>
LinkedList<T>::LinkedList() : size(0), head(nullptr), tail(nullptr) {}

template <class T>
LinkedList<T>::LinkedList(T *items, int count) : LinkedList() {
    if (count < 0) throw std::invalid_argument("Count must be non-negative");
    for (int i = 0; i < count; i++) {
        Append(items[i]);
    }
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &list) : LinkedList() {
    Node<T> *current = list.head;
    while (current != nullptr) {
        Append(current->value);
        current = current->next;
    }
}

template <class T>
LinkedList<T>::~LinkedList() {
    Node<T> *current = head;
    while (current != nullptr) {
        Node<T> *next = current->next;
        delete current;
        current = next;
    }
}

// ---------- Getters ----------
template <class T>
T LinkedList<T>::GetFirst() const {
    if (head == nullptr) throw std::out_of_range("List is empty");
    return head->value;
}

template <class T>
T LinkedList<T>::GetLast() const {
    if (tail == nullptr) throw std::out_of_range("List is empty");
    return tail->value;
}

template <class T>
T LinkedList<T>::Get(int index) const {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
    Node<T> *current = head;
    for (int i = 0; i < index; i++) current = current->next;
    return current->value;
}

template <class T>
LinkedList<T> *LinkedList<T>::GetSubList(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= size || start_index > end_index) throw std::out_of_range("Invalid index range");
    if (head == nullptr) throw std::out_of_range("List is empty");

    // Create a new linked list for the sublist
    LinkedList<T> *sublist = new LinkedList<T>();
    Node<T> *current = head;
    for (int i = 0; i < start_index; i++) current = current->next;
    for (int i = start_index; i <= end_index; i++) {
        sublist->Append(current->value);
        current = current->next;
    }
    return sublist;
}

template <class T>
int LinkedList<T>::GetLength() const { return size; }

// ---------- Modifiers ----------
template <class T>
void LinkedList<T>::Append(T item) {
    Node<T> *new_node = new Node<T>{item, nullptr, tail};
    if (head == nullptr) {
        head = new_node;
        tail = new_node;
        size++;
        return;
    }
    tail->next = new_node;
    tail = new_node;
    size++;
}

template <class T>
void LinkedList<T>::Prepend(T item) {
    Node<T> *new_node = new Node<T>{item, head, nullptr};
    if (head == nullptr) {
        head = new_node;
        tail = new_node;
        size++;
        return;
    }
    head = new_node;
    size++;
}

template <class T>
void LinkedList<T>::InsertAt(T item, int index) {
    if (index < 0 || index > size) throw std::out_of_range("Index out of range");
    if (index == 0) { Prepend(item); return; }
    if (index == size) { Append(item); return; }

    Node<T> *current = head;
    for (int i = 0; i < index - 1; i++) current = current->next;
    Node<T> *new_node = new Node<T>{item, current->next, current};
    current->next = new_node;
    if (new_node->next) new_node->next->prev = new_node;
    size++;
}

template <class T>
LinkedList<T> *LinkedList<T>::Concat(LinkedList<T> *list) {
    LinkedList<T> *new_list = new LinkedList<T>(*this);
    Node<T> *current = list->head;
    while (current != nullptr) {
        new_list->Append(current->value);
        current = current->next;
    }
    return new_list;
}

template <class T>
Node<T> *LinkedList<T>::GetHead() const { return head; }

template <class T>
Node<T> *LinkedList<T>::GetTail() const { return tail; }

// ---------- Operators ----------
template <class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &other) {
    if (this != &other) {
        Node<T> *current = head;
        while (current != nullptr) {
            Node<T> *next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;

        current = other.head;
        while (current != nullptr) {
            Append(current->value);
            current = current->next;
        }
    }
    return *this;
}

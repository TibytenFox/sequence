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
    LinkedList() : size(0), first(nullptr) {}
    LinkedList(T *items, int count) : LinkedList() {
        for (int i = 0; i < count; i++) {
            Append(items[i]);
        }
    }
    LinkedList(const LinkedList<T> &list) : LinkedList() {
        Node<T> *current = list.first;
        while (current != nullptr) {
            Append(current->value);
            current = current->next;
        }
    }
    T GetFirst() const {
        if (first == nullptr) {
            throw std::out_of_range("List is empty");
        }
        return first->value;
    }
    T GetLast() const {
        if (first == nullptr) {
            throw std::out_of_range("List is empty");
        }

        Node<T> *current = first;
        while (current->next != nullptr) {
            current = current->next;
        }
        return current->value;
    }
    T Get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T> *current = first;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->value;
    }
    LinkedList<T> *GetSubList(int start_index, int end_index) {
        if (start_index < 0 || end_index >= size || start_index > end_index) {
            throw std::out_of_range("Invalid index range");
        }

        LinkedList<T> *sublist = new LinkedList<T>();
        Node<T> *current = first;
        for (int i = 0; i < start_index; i++) {
            current = current->next;
        }

        for (int i = start_index; i <= end_index; i++) {
            sublist->Append(current->value);
            current = current->next;
        }
        return sublist;
    }
    int GetLength() const { return size; }
    void Append(T item) {
        Node<T> *new_node = new Node<T>{item, nullptr};
        
        if (first == nullptr) {
            first = new_node;
            return;
        }

        Node<T> *current = first;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new_node;
        size++;
    }
    void Prepend(T item) {
        Node<T> *new_node = new Node<T>{item, first};
        first = new_node;
        size++; 
    }
    void InsertAt(T item, int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        
        Node<T> *current = first;
        for (int i = 0; i < index - 1; i++) current = current->next;
        Node<T> *new_node = new Node<T>{item, current->next};
        current->next = new_node;
        size++;
    }
    LinkedList<T> *Concat(LinkedList<T> *list) {
        LinkedList<T> *new_list = new LinkedList<T>(*this);
        Node<T> *current = list->first;
        while (current != nullptr) {
            new_list->Append(current->value);
            current = current->next;
        }
        return new_list;
    }
    ~LinkedList() {
        Node<T> *current = first;
        while (current != nullptr) {
            Node<T> *next = current->next;
            delete current;
            current = next;
        }
    }
};
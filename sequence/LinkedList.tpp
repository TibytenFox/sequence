#include "LinkedList.hpp"

// --------- Constructors and Destructor ----------

template <class T>
LinkedList<T>::LinkedList() : size(0), head(nullptr), tail(nullptr) {}

template <class T>
LinkedList<T>::LinkedList(T *items, int count) : LinkedList() {
    if (count < 0) throw IndexOutOfRange("LinkedList(): Count must be non-negative");
    for (int i = 0; i < count; i++) {
        Append(items[i]);
    }
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &list) : LinkedList() {
    Node *current = list.head;
    while (current != nullptr) {
        Append(current->value);
        current = current->next;
    }
}

template <class T>
LinkedList<T>::LinkedList(LinkedList<T> &&other) noexcept : size(other.size), head(other.head), tail(other.tail) {
    other.size = 0;
    other.head = nullptr;
    other.tail = nullptr;
}

template <class T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList<T> &&other) noexcept {
    if (this != &other) {
        Node *current = head;
        while (current != nullptr) {
            Node *next = current->next;
            delete current;
            current = next;
        }
        head = other.head;
        tail = other.tail;
        size = other.size;
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }
    return *this;
}

template <class T>
LinkedList<T>::~LinkedList() {
    Node *current = head;
    while (current != nullptr) {
        Node *next = current->next;
        delete current;
        current = next;
    }
}

// ---------- Getters ----------

template <class T>
const T &LinkedList<T>::GetFirst() const {
    if (head == nullptr) throw EmptyCollectionError("GetFirst(): List is empty");
    return head->value;
}

template <class T>
const T &LinkedList<T>::GetLast() const {
    if (tail == nullptr) throw EmptyCollectionError("GetLast(): List is empty");
    return tail->value;
}

template <class T>
const T &LinkedList<T>::Get(int index) const {
    if (index < 0 || index >= size) throw IndexOutOfRange("Get(): Index out of range");
    Node *current = head;
    for (int i = 0; i < index; i++) current = current->next;
    return current->value;
}

// Caller is responsible for deleting the returned list
template <class T>
LinkedList<T> *LinkedList<T>::GetSubList(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= size || start_index > end_index) throw IndexOutOfRange("GetSubList(): index range");
    if (head == nullptr) throw EmptyCollectionError("GetSubList(): List is empty");

    // Create a new linked list for the sublist
    LinkedList<T> *sublist = new LinkedList<T>();
    Node *current = head;
    for (int i = 0; i < start_index; i++) current = current->next;
    for (int i = start_index; i <= end_index; i++) {
        sublist->Append(current->value);
        current = current->next;
    }
    return sublist;
}

template <class T>
int LinkedList<T>::GetLength() const { return size; }

template <class T>
IEnumerator<T> *LinkedList<T>::GetEnumerator() const {
    return new Enumerator(this->head);
}

// ---------- Modifiers ----------

template <class T>
void LinkedList<T>::Append(T item) {
    Node *new_node = new Node{item, nullptr, tail};
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
    Node *new_node = new Node{item, head, nullptr};
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
    if (index < 0 || index > size) throw IndexOutOfRange("InsertAt(): Index out of range");
    if (index == 0) { Prepend(item); return; }
    if (index == size) { Append(item); return; }

    Node *current = head;
    for (int i = 0; i < index - 1; i++) current = current->next;
    Node *new_node = new Node{item, current->next, current};
    current->next = new_node;
    if (new_node->next) new_node->next->prev = new_node;
    size++;
}


template <class T>
T LinkedList<T>::RemoveAt(int index) {
    if (size == 0) if (size == 0) throw EmptyCollectionError("RemoveAt(): Empty collection");
    if (index < 0 || index >= size) {
        throw IndexOutOfRange("RemoveAt: index out of range");
    }

    Node *current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }

    T removedValue = current->value;

    if (current->prev != nullptr) {
        current->prev->next = current->next;
    } else {
        head = current->next;
    }

    if (current->next != nullptr) {
        current->next->prev = current->prev;
    } else {
        tail = current->prev;
    }

    delete current;
    --size;

    return removedValue;
}


template <class T>
LinkedList<T> *LinkedList<T>::Concat(const LinkedList<T> &list) {
    LinkedList<T> *new_list = new LinkedList<T>(*this);
    Node *current = list.head;
    while (current != nullptr) {
        new_list->Append(current->value);
        current = current->next;
    }
    return new_list;
}

template <class T>
typename LinkedList<T>::Node *LinkedList<T>::GetHead() const { return head; }

template <class T>
typename LinkedList<T>::Node *LinkedList<T>::GetTail() const { return tail; }

// ---------- Operators ----------

template <class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &other) {
    if (this != &other) {
        Node *current = head;
        while (current != nullptr) {
            Node *next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
        
        // Copy other list
        current = other.head;
        while (current != nullptr) {
            Append(current->value);
            current = current->next;
        }
    }
    return *this;
}

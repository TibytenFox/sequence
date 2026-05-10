#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "Exceptions.hpp"
#include "IEnumerable.hpp"

template <class T>
class LinkedList : IEnumerable<T> {
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
    class Enumerator : public IEnumerator<T> {
    private:
        const Node *head;
        const Node *current;
        bool started;
    public:
        explicit Enumerator(const Node *ptr) : head(ptr), current(nullptr), started(false) {}

        bool MoveNext() override {
            if (!this->started) {
                this->started = true;
                this->current = head;
            } else if (this->current != nullptr) {
                this->current = this->current->next;
            }
            return this->current != nullptr;
        }

        const T &GetCurrent() const override {
            if (current == nullptr) {
                throw IndexOutOfRange("Enumerator: out of bounds");
            }
            return current->value;
        }
    };

    // ---------- Constructors / Destructor ----------
    LinkedList();                              // Empty array
    LinkedList(T *items, int count);           // Copy from C-array
    LinkedList(const LinkedList<T> &list);     // Copy constructor
    LinkedList(LinkedList<T> &&other) noexcept; // Move constructor
    ~LinkedList();                             // Destructor

    // ---------- Accessors ----------
    const T &GetFirst() const;                        // Get first element
    const T &GetLast() const;                         // Get last element
    const T &Get(int index) const;                    // Get element at index
    int GetLength() const;                            // Return current size
    LinkedList<T> *GetSubList(int start_index, int end_index) const; // Returns new heap-allocated list
    virtual IEnumerator<T> *GetEnumerator() const override;

    // ---------- Modifiers ----------
    void Append(T item);                       // Add element at the end
    void Prepend(T item);                      // Add element at the beginning
    void InsertAt(T item, int index);          // Insert at position
    T RemoveAt(int index);                  // Remove at position
    LinkedList<T> *Concat(const LinkedList<T> &list); // Returns new heap-allocated list

    // ---------- Iteration support ----------
    Node *GetHead() const;                     // Raw pointer to first node (nullptr if empty)
    Node *GetTail() const;                     // Raw pointer to last node (nullptr if empty)

    // ---------- Operators ----------
    LinkedList<T> &operator=(const LinkedList<T> &other);
    LinkedList<T> &operator=(LinkedList<T> &&other) noexcept;
};

#include "LinkedList.tpp"
#endif // LINKED_LIST_HPP
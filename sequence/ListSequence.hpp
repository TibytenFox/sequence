#ifndef LIST_SEQUENCE_HPP
#define LIST_SEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> items;   // Linked list

public:
    // ---------- Constructors and Destructor ----------
    ListSequence();
    ListSequence(T *items, int count);
    ListSequence(const ListSequence<T> &list);
    ListSequence(ListSequence<T> &&other) noexcept = default;
    ListSequence<T> &operator=(ListSequence<T> &&other) noexcept = default;
    ~ListSequence();

    // ---------- Sequence overrides ----------
    virtual const T &GetFirst() const override;
    virtual const T &GetLast() const override;
    virtual const T &Get(int index) const override;
    virtual int GetLength() const override;
    virtual Sequence<T> *GetSubsequence(int start_index, int end_index) const override;
    virtual Sequence<T> *Append(T item) override;
    virtual Sequence<T> *Prepend(T item) override;
    virtual Sequence<T> *InsertAt(T item, int index) override;
    virtual Sequence<T> *Concat(const Sequence<T> *list) const override;

    // ---------- Iterators ----------
    virtual IEnumerator<T> *GetEnumerator() const override;

protected:
    virtual ListSequence<T> *Instance() = 0;
    ListSequence<T> *AppendInternal(T item);
    ListSequence<T> *PrependInternal(T item);
    ListSequence<T> *InsertAtInternal(T item, int index);
};

#include "ListSequence.tpp"
#endif // LIST_SEQUENCE_HPP
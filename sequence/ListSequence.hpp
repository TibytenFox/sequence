#ifndef LIST_SEQUENCE_HPP
#define LIST_SEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> items;   // Linked list

    // Iterator for ListSequence
    class Enumerator : public IEnumerator<T> {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using NodePointerType = typename LinkedList<T>::Node*;
        using ReferenceType = ValueType&;

        Enumerator(NodePointerType ptr);

        void Increment() override;
        void Decrement() override;
        T &Dereference() const override;
        bool Equals(const IEnumerator<T> &other) const override;
    private:
        NodePointerType current;
    };

    // Const Iterator for ListSequence
    class ConstEnumerator : public IConstEnumerator<T> {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using NodePointerType = typename LinkedList<T>::Node*;
        using ReferenceType = ValueType&;

        ConstEnumerator(NodePointerType ptr);

        void Increment() override;
        void Decrement() override;
        const T &Dereference() const override;
        bool Equals(const IConstEnumerator<T> &other) const override;
    private:
        NodePointerType current;
    };
public:
    // ---------- Constructors and Destructor ----------
    ListSequence();
    ListSequence(T *items, int count);
    ListSequence(const ListSequence<T> &list);
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

    // ---------- Map-Reduce ----------
    // template <class T2>
    // ListSequence<T2> *Map(T2 (*func)(T)) const;

    // ListSequence<T> *Where(bool (*predicate)(T)) const override;
    
    // template <class T2>
    // T2 Reduce(T2 (*func)(T2, T), T2 initial) const;

    // ---------- Iterators ----------
    Enumerator *begin() override;
    Enumerator *end() override ;
    ConstEnumerator *begin() const override;
    ConstEnumerator *end() const override;

protected:
    virtual ListSequence<T> *Instance() = 0;
    ListSequence<T> *AppendInternal(T item);
    ListSequence<T> *PrependInternal(T item);
    ListSequence<T> *InsertAtInternal(T item, int index);
};

#include "ListSequence.tpp"
#endif // LIST_SEQUENCE_HPP
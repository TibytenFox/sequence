#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> items;    // Using dynamic array

    // Iterator for ArraySequence
    class Enumerator : public IEnumerator<T> {
    public:
        using ValueType = T;
        using PointerType = ValueType*;
        using ReferenceType = ValueType&;

        Enumerator(PointerType ptr);
        
        void Increment() override;
        void Decrement() override;
        T &Dereference() const override;
        bool Equals(const IEnumerator<T> &other) const override;
    private:
        PointerType current;
    };

    // Const Iterator for ArraySequence
    class ConstEnumerator : public IConstEnumerator<T> {
    public:
        using ValueType = T;
        using PointerType = const ValueType*;
        using ReferenceType = ValueType&;

        ConstEnumerator(PointerType ptr);

        void Increment() override;
        void Decrement() override;
        const T &Dereference() const override;
        bool Equals(const IConstEnumerator<T> &other) const override;
    private:
        const T* current;
    };
public:
    // ---------- Constructors and Destructor ----------
    ArraySequence();
    ArraySequence(T *items, int count);
    ArraySequence(const ArraySequence<T> &list);
    ~ArraySequence();

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
    Enumerator *begin() override;
    Enumerator *end() override;
    ConstEnumerator *begin() const override;
    ConstEnumerator *end() const override;

protected:
    virtual ArraySequence<T> *Instance() = 0;
    ArraySequence<T> *AppendInternal(T item);
    ArraySequence<T> *PrependInternal(T item);
    ArraySequence<T> *InsertAtInternal(T item, int index);
};

#include "ArraySequence.tpp"
#endif // ARRAY_SEQUENCE_HPP
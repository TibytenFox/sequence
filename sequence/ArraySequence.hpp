#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> items;    // Using dynamic array

public:
    // ---------- Constructors and Destructor ----------
    ArraySequence();
    ArraySequence(T *items, int count);
    ArraySequence(const ArraySequence<T> &list);
    ArraySequence(ArraySequence<T> &&other) noexcept = default;
    ArraySequence<T> &operator=(ArraySequence<T> &&other) noexcept = default;
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
    virtual IEnumerator<T> *GetEnumerator() const override;

protected:
    virtual ArraySequence<T> *Instance() = 0;
    ArraySequence<T> *AppendInternal(T item);
    ArraySequence<T> *PrependInternal(T item);
    ArraySequence<T> *InsertAtInternal(T item, int index);
};

#include "ArraySequence.tpp"
#endif // ARRAY_SEQUENCE_HPP
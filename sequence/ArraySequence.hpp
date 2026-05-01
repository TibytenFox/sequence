#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    int size; // items.size - capacity (items.size >= size)
    DynamicArray<T> items;    // Using dynamic array

public:
    class Enumerator : public IEnumerator<T> {
    private:
        const ArraySequence<T> *seq;
        int current_index;
    public:
        explicit Enumerator(const ArraySequence<T> *ptr) : seq(ptr), current_index(-1) {}

        bool MoveNext() override {
            if (this->current_index + 1 < this->seq->GetLength()) {
                this->current_index++;
                return true;
            }
            return false;
        }

        const T &GetCurrent() const override {
            if (this->current_index < 0 || this->current_index >= this->seq->GetLength()) {
                throw IndexOutOfRange("Enumerator: out of bounds");
            }
            return this->seq->Get(current_index);
        }
    };

    // ---------- Constructors and Destructor ----------
    ArraySequence();
    explicit ArraySequence(int count);
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
    virtual Sequence<T> *Concat(const Sequence<T> &list) const override;

    int GetCapacity() const;

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
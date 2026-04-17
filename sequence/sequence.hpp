#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "Exceptions.hpp"
#include "ICollection.hpp"
#include "IEnumerable.hpp"
#include "ISequenceBuilder.hpp"

template <class T>
class Sequence : public ICollection<T>, public IEnumerable<T> {
public:
    // ---------- Builder ----------
    virtual ISequenceBuilder<T> *CreateBuilder() const = 0;

    // ---------- Virtual constructors (ICollection) ----------
    virtual Sequence<T> *CreateEmpty() const override = 0;
    virtual Sequence<T> *Clone() const override = 0;

    // ---------- Virtual getters (ICollection) ----------
    virtual const T &Get(int index) const override = 0;
    virtual int GetLength() const override = 0;

    // ---------- Modifying operations (may return new object or modify in place) ----------
    virtual Sequence<T> *Append(T item) = 0;
    virtual Sequence<T> *Prepend(T item) = 0;
    virtual Sequence<T> *InsertAt(T item, int index) = 0;

    // ---------- Getters ----------
    virtual const T &GetFirst() const;
    virtual const T &GetLast() const;
    virtual Sequence<T> *GetSubsequence(int start_index, int end_ndex) const = 0;

    // ---------- Operations ----------
    virtual Sequence<T> *Concat(const Sequence<T> *list) const = 0;

    // ---------- Map-Reduce operations ----------
    template <class T2>
    Sequence<T2> *Map(T2 (*func)(T)) const;

    virtual Sequence<T> *Where(bool (*predicate)(T)) const;

    template <class T2>
    T2 Reduce(T2 (*func)(T2, T), T initial) const;

    // ---------- Operators ----------
    virtual const T &operator[](int index) const = 0;

    virtual ~Sequence() = default;
};

#include "Sequence.tpp"
#endif // SEQUENCE_HPP
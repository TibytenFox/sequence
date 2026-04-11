#pragma once

template <class T>
class Sequence {
public:
    // ---------- Basic access ----------
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;

    // ---------- Subsequence and concatenation ----------
    virtual Sequence<T> *GetSubSequence(int start_index, int end_index) const = 0;
    virtual Sequence<T> *Concat(const Sequence<T> *list) const = 0;

    // ---------- Modifying operations (may return new object or modify in place) ----------
    virtual Sequence<T> *Append(T item) = 0;
    virtual Sequence<T> *Prepend(T item) = 0;
    virtual Sequence<T> *InsertAt(T item, int index) = 0;

    // ---------- Map-Reduce operations ----------
    template <class T2>
    Sequence<T> *Map(T2 (*func)(T)) const {}; // Not virtual; implemented in derived classes
    virtual Sequence<T> *Where(bool (*predicate)(T)) const = 0;
    template <class T2>
    T2 Reduce(T2 (*func)(T2, T), T initial) const {};

    virtual ~Sequence() = default;
};

#pragma once

template <class T>
class Iterator {
public:
    virtual ~Iterator() = default;

    virtual void Increment() = 0; // ++it
    virtual void Decrement() = 0; // --it;
    virtual T &Dereference() const = 0; // *it
    virtual bool Equals(const Iterator<T> &other) const = 0; // it == other

    // Operator overloads (call virtual methods)
    Iterator<T> &operator++() {
        Increment();
        return *this;
    }
    Iterator<T> &operator--() {
        Decrement();
        return *this;
    }
    T &operator*() const {
        return Dereference();
    }
    T *operator->() const {
        return &Dereference();
    }
    bool operator==(const Iterator<T> &other) const {
        return Equals(other);
    }
    bool operator!=(const Iterator<T> &other) const {
        return !Equals(other);
    }
};

template <class T>
class ConstIterator {
public:
    virtual ~ConstIterator() = default;

    virtual void Increment() = 0; // ++it
    virtual void Decrement() = 0; // --it;
    virtual const T &Dereference() const = 0; // *it
    virtual bool Equals(const ConstIterator<T> &other) const = 0; // it == other

    // Operator overloads (call virtual methods)
    ConstIterator<T> &operator++() {
        Increment();
        return *this;
    }
    ConstIterator<T> &operator--() {
        Decrement();
        return *this;
    }
    const T &operator*() const {
        return Dereference();
    }
    const T *operator->() const {
        return &Dereference();
    }
    bool operator==(const ConstIterator<T> &other) const {
        return Equals(other);
    }
    bool operator!=(const ConstIterator<T> &other) const {
        return !Equals(other);
    }
};
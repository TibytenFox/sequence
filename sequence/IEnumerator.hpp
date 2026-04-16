#ifndef IENUMERATOR_HPP
#define IENUMERATOR_HPP

// TODO: переписать к виду MoveNext()

template <class T>
class IEnumerator {
public:
    virtual ~IEnumerator() = default;

    virtual void Increment() = 0; // ++it
    virtual void Decrement() = 0; // --it;
    virtual T &Dereference() const = 0; // *it
    virtual bool Equals(const IEnumerator<T> &other) const = 0; // it == other

    // Operator overloads (call virtual methods)
    IEnumerator<T> &operator++() {
        Increment();
        return *this;
    }
    IEnumerator<T> &operator--() {
        Decrement();
        return *this;
    }
    T &operator*() const {
        return Dereference();
    }
    T *operator->() const {
        return &Dereference();
    }
    bool operator==(const IEnumerator<T> &other) const {
        return Equals(other);
    }
    bool operator!=(const IEnumerator<T> &other) const {
        return !Equals(other);
    }
};

template <class T>
class IConstEnumerator {
public:
    virtual ~IConstEnumerator() = default;

    virtual void Increment() = 0; // ++it
    virtual void Decrement() = 0; // --it;
    virtual const T &Dereference() const = 0; // *it
    virtual bool Equals(const IConstEnumerator<T> &other) const = 0; // it == other

    // Operator overloads (call virtual methods)
    IConstEnumerator<T> &operator++() {
        Increment();
        return *this;
    }
    IConstEnumerator<T> &operator--() {
        Decrement();
        return *this;
    }
    const T &operator*() const {
        return Dereference();
    }
    const T *operator->() const {
        return &Dereference();
    }
    bool operator==(const IConstEnumerator<T> &other) const {
        return Equals(other);
    }
    bool operator!=(const IConstEnumerator<T> &other) const {
        return !Equals(other);
    }
};

#endif // IENUMERATOR_HPP
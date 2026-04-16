#ifndef IMMUTABLE_LIST_SEQUENCE_HPP
#define IMMUTABLE_LIST_SEQUENCE_HPP

#include "ListSequence.hpp"

template <class T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    virtual ListSequence<T> *Instance() override { 
        return new ImmutableListSequence<T>(*this); 
    }
public:
    class Builder : public ISequenceBuilder<T> {
    private:
        ImmutableListSequence<T> *seq;
    public:
        Builder() {
            seq = new ImmutableListSequence();
        }
        
        virtual ~Builder() {
            if (seq) {
                delete seq;
            }
        }

        virtual ISequenceBuilder<T> *Append(const T &item) override {
            seq->AppendInternal(item);
            return this;
        }

        virtual Sequence<T> *Build() override {
            Sequence<T> *res = seq;
            seq = nullptr;
            return res;
        }
    };

    virtual ISequenceBuilder<T> *CreateBuilder() const override {
        return new Builder();
    }

    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(T *items, int count) : ListSequence<T>(items, count) {}
    ImmutableListSequence(const ListSequence<T> &list) : ListSequence<T>(list) {}
    ~ImmutableListSequence() {}

    ImmutableListSequence<T> &operator=(const ImmutableListSequence<T> &other) {
        if (this != &other) {
            this->items = other.items;
        }
        return *this;
    }

    bool operator==(const ImmutableListSequence<T> &other) const {
        if (this->GetLength() != other.GetLength()) return false;
        for (int i = 0; i < this->GetLength(); ++i) {
            if (this->Get(i) != other.Get(i)) return false;
        }
        return true;
    }

    bool operator!=(const ImmutableListSequence<T> &other) const {
        return !(*this == other);
    }

    T &operator[](int index) {
        if (index < 0 || index >= this->GetLength()) throw IndexOutOfRange("Operator[]: Index out of range");
        typename ListSequence<T>::Enumerator *current = this->begin();
        for (int i = 0; i < index; ++i) {
            current->Increment();
        }
        T &value = current->Dereference();
        delete current;
        return value;
    }

    const T &operator[](int index) const {
        if (index < 0 || index >= this->GetLength()) throw IndexOutOfRange("Operator[]: Index out of range");
        typename ListSequence<T>::ConstEnumerator *current = this->begin();
        for (int i = 0; i < index; ++i) {
            current->Increment();
        }
        const T &value = current->Dereference();
        delete current;
        return value;
    }

    ImmutableListSequence<T> operator+(const ImmutableListSequence<T> &other) const {
        ImmutableListSequence<T> result(*this);
        for (int i = 0; i < other.GetLength(); ++i) {
            result.AppendInternal(other.Get(i));
        }
        return result;
    }

    ImmutableListSequence<T> &operator+=(const T &value) {
        ImmutableListSequence<T> result(*this);
        result.AppendInternal(value);
        *this = result;
        return *this;
    }

    ImmutableListSequence<T> &operator+=(const ImmutableListSequence<T> &other) {
        ImmutableListSequence<T> result(*this);
        for (int i = 0; i < other.GetLength(); ++i) {
            result.AppendInternal(other.Get(i));
        }
        *this = result;
        return *this;
    }

    virtual Sequence<T> *CreateEmpty() const override { 
        return new ImmutableListSequence<T>(); 
    }

    virtual Sequence<T> *Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};

#endif // IMMUTABLE_LIST_SEQUENCE_HPP
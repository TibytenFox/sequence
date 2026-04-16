#ifndef IMMUTABLE_ARRAY_SEQUENCE_HPP
#define IMMUTABLE_ARRAY_SEQUENCE_HPP

#include "ArraySequence.hpp"

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    virtual ArraySequence<T> *Instance() override { 
        return new ImmutableArraySequence<T>(*this); 
    }
public:
    class Builder : public ISequenceBuilder<T> {
    private:
        ImmutableArraySequence<T> *seq;
    public:
        Builder() {
            seq = new ImmutableArraySequence();
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

    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence(const ArraySequence<T> &list) : ArraySequence<T>(list) {}
    ~ImmutableArraySequence() {}

    ImmutableArraySequence<T> &operator=(const ImmutableArraySequence<T> &other) {
        if (this != &other) {
            this->items = other.items;
        }
        return *this;
    }

    bool operator==(const ImmutableArraySequence<T> &other) const {
        if (this->GetLength() != other.GetLength()) return false;
        for (int i = 0; i < this->GetLength(); ++i) {
            if (this->Get(i) != other.Get(i)) return false;
        }
        return true;
    }

    bool operator!=(const ImmutableArraySequence<T> &other) const {
        return !(*this == other);
    }

    T &operator[](int index) {
        return this->items[index];
    }

    const T &operator[](int index) const {
        return this->items.Get(index);
    }

    ImmutableArraySequence<T> operator+(const ImmutableArraySequence<T> &other) const {
        ImmutableArraySequence<T> result(*this);
        for (int i = 0; i < other.GetLength(); ++i) {
            result.AppendInternal(other.Get(i));
        }
        return result;
    }

    ImmutableArraySequence<T> &operator+=(const T &value) {
        ImmutableArraySequence<T> result(*this);
        result.AppendInternal(value);
        *this = result;
        return *this;
    }

    ImmutableArraySequence<T> &operator+=(const ImmutableArraySequence<T> &other) {
        ImmutableArraySequence<T> result(*this);
        for (int i = 0; i < other.GetLength(); ++i) {
            result.AppendInternal(other.Get(i));
        }
        *this = result;
        return *this;
    }

    virtual Sequence<T> *CreateEmpty() const override { 
        return new ImmutableArraySequence<T>(); 
    }

    virtual Sequence<T> *Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};

#endif // IMMUTABLE_ARRAY_SEQUENCE_HPP
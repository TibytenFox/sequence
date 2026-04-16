#ifndef MUTALBE_LIST_SEQUENCE_HPP
#define MUTALBE_LIST_SEQUENCE_HPP

#include "ListSequence.hpp"

template <class T>
class MutableListSequence : public ListSequence<T> {
protected:
    virtual ListSequence<T> *Instance() override { 
        return this; 
    }
public:
    class Builder : public ISequenceBuilder<T> {
    private:
        MutableListSequence<T> *seq;
    public:
        Builder() {
            seq = new MutableListSequence();
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
    
    MutableListSequence() : ListSequence<T>() {}
    MutableListSequence(T *items, int count) : ListSequence<T>(items, count) {}
    MutableListSequence(const ListSequence<T> &list) : ListSequence<T>(list) {}
    ~MutableListSequence() {}

    MutableListSequence<T> &operator=(const MutableListSequence<T> &other) {
        if (this != &other) {
            this->items = other.items;
        }
        return *this;
    }

    bool operator==(const MutableListSequence<T> &other) const {
        if (this->GetLength() != other.GetLength()) return false;
        for (int i = 0; i < this->GetLength(); ++i) {
            if (this->Get(i) != other.Get(i)) return false;
        }
        return true;
    }

    bool operator!=(const MutableListSequence<T> &other) const {
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

    MutableListSequence<T> operator+(const MutableListSequence<T> &other) const {
        MutableListSequence<T> result(*this);
        for (int i = 0; i < other.GetLength(); ++i) {
            result.Append(other.Get(i));
        }
        return result;
    }

    MutableListSequence<T> &operator+=(const T &value) {
        this->Append(value);
        return *this;
    }

    MutableListSequence<T> &operator+=(const MutableListSequence<T> &other) {
        for (int i = 0; i < other.GetLength(); ++i) {
            this->Append(other.Get(i));
        }
        return *this;
    }

    virtual Sequence<T> *CreateEmpty() const override { 
        return new MutableListSequence<T>(); 
    }

    virtual Sequence<T> *Clone() const override {
        return new MutableListSequence<T>(*this);
    }
};

#endif // MUTALBE_LIST_SEQUENCE_HPP
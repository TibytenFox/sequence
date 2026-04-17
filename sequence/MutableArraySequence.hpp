#ifndef MUTALBE_ARRAY_SEQUENCE_HPP
#define MUTALBE_ARRAY_SEQUENCE_HPP

#include "ArraySequence.hpp"

template <class T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    virtual ArraySequence<T> *Instance() override { 
        return this; 
    }
public:
    class Builder : public ISequenceBuilder<T> {
    private:
        MutableArraySequence<T> *seq;
    public:
        Builder() {
            seq = new MutableArraySequence();
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

    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {}
    MutableArraySequence(const ArraySequence<T> &list) : ArraySequence<T> (list) {}
    ~MutableArraySequence() {}

    MutableArraySequence<T> &operator=(const MutableArraySequence<T> &other);
    bool operator==(const MutableArraySequence<T> &other) const;
    bool operator!=(const MutableArraySequence<T> &other) const;
    T &operator[](int index);
    const T &operator[](int index) const;
    MutableArraySequence<T> operator+(const MutableArraySequence<T> &other) const;
    MutableArraySequence<T> &operator+=(const T &value);
    MutableArraySequence<T> &operator+=(const MutableArraySequence<T> &other);

    virtual Sequence<T> *CreateEmpty() const override { 
        return new MutableArraySequence<T>(); 
    }

    virtual Sequence<T> *Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
};

#include "MutableArraySequence.tpp"
#endif // MUTALBE_ARRAY_SEQUENCE_HPP
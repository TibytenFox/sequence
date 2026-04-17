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

    MutableListSequence<T> &operator=(const MutableListSequence<T> &other);
    bool operator==(const MutableListSequence<T> &other) const;
    bool operator!=(const MutableListSequence<T> &other) const;
    T &operator[](int index);
    const T &operator[](int index) const;
    MutableListSequence<T> operator+(const MutableListSequence<T> &other) const;
    MutableListSequence<T> &operator+=(const T &value);
    MutableListSequence<T> &operator+=(const MutableListSequence<T> &other);

    virtual Sequence<T> *CreateEmpty() const override { 
        return new MutableListSequence<T>(); 
    }

    virtual Sequence<T> *Clone() const override {
        return new MutableListSequence<T>(*this);
    }
};

#include "MutableListSequence.tpp"
#endif // MUTALBE_LIST_SEQUENCE_HPP
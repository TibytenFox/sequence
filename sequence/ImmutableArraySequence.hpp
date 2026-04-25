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
            this->seq = new ImmutableArraySequence();
        }
        
        virtual ~Builder() {
            if (this->seq) {
                delete this->seq;
            }
        }

        virtual ISequenceBuilder<T> *Append(const T &item) override {
            this->seq->AppendInternal(item);
            return this;
        }

        virtual Sequence<T> *Build() override {
            Sequence<T> *res = seq;
            this->seq = nullptr;
            return res;
        }
    };

    virtual ISequenceBuilder<T> *CreateBuilder() const override {
        return new Builder();
    }

    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence(const ArraySequence<T> &other) : ArraySequence<T>(other) {}
    ImmutableArraySequence(const ImmutableArraySequence<T> &other) : ArraySequence<T>(other) {}
    ImmutableArraySequence(ImmutableArraySequence<T> &&other) noexcept = default;
    ~ImmutableArraySequence() {}

    ImmutableArraySequence<T> &operator=(const ImmutableArraySequence<T> &other);
    ImmutableArraySequence<T> &operator=(ImmutableArraySequence<T> &&other) noexcept = default;
    bool operator==(const ImmutableArraySequence<T> &other) const;
    bool operator!=(const ImmutableArraySequence<T> &other) const;
    const T &operator[](int index) const override;
    ImmutableArraySequence<T> operator+(const ImmutableArraySequence<T> &other) const;
    ImmutableArraySequence<T> &operator+=(const T &value);
    ImmutableArraySequence<T> &operator+=(const ImmutableArraySequence<T> &value);

    virtual Sequence<T> *CreateEmpty() const override {
        return new ImmutableArraySequence<T>();
    }

    virtual Sequence<T> *Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};

#include "ImmutableArraySequence.tpp"
#endif // IMMUTABLE_ARRAY_SEQUENCE_HPP
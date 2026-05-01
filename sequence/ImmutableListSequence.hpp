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
            delete seq;
        }

        virtual ISequenceBuilder<T> *Append(const T &item) override {
            if (seq) seq->AppendInternal(item);
            return this;
        }

        virtual ImmutableListSequence<T> *Build() override {
            ImmutableListSequence<T> *res = seq;
            seq = nullptr;
            return res;
        }
    };

    virtual ISequenceBuilder<T> *CreateBuilder() const override {
        return new Builder();
    }

    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(T *items, int count) : ListSequence<T>(items, count) {}
    ImmutableListSequence(const ListSequence<T> &other) : ListSequence<T>(other) {}
    ImmutableListSequence(const ImmutableListSequence<T> &other) : ListSequence<T>(other) {}
    ImmutableListSequence(ImmutableListSequence<T> &&other) noexcept = default;
    ~ImmutableListSequence() {}

    ImmutableListSequence<T> &operator=(const ImmutableListSequence<T> &other);
    ImmutableListSequence<T> &operator=(ImmutableListSequence<T> &&other) noexcept = default;
    bool operator==(const ImmutableListSequence<T> &other) const;
    bool operator!=(const ImmutableListSequence<T> &other) const;
    const T &operator[](int index) const override;
    ImmutableListSequence<T> operator+(const ImmutableListSequence<T> &other) const;
    ImmutableListSequence<T> &operator+=(const T &value);
    ImmutableListSequence<T> &operator+=(const ImmutableListSequence<T> &other);

    virtual Sequence<T> *CreateEmpty() const override { 
        return new ImmutableListSequence<T>(); 
    }

    virtual Sequence<T> *Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};

#include "ImmutableListSequence.tpp"
#endif // IMMUTABLE_LIST_SEQUENCE_HPP
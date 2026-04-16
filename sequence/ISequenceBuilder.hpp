#ifndef ISEQUENCE_BUILDER_HPP
#define ISEQUENCE_BUILDER_HPP

template <class T>
class Sequence;

template <class T>
class ISequenceBuilder {
public:
    virtual ~ISequenceBuilder() {}

    virtual ISequenceBuilder<T> *Append(const T& item) = 0;
    virtual Sequence<T> *Build() = 0;
};

#endif // ISEQUENCE_BUILDER_HPP
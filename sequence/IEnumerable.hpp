#ifndef IENUMERABLE_HPP
#define IENUMERABLE_HPP

#include "IEnumerator.hpp"

template <class T>
class IEnumerable {
public:
    virtual ~IEnumerable() {}

    virtual IEnumerator<T> *begin() = 0;
    virtual IEnumerator<T> *end() = 0;
    virtual IConstEnumerator<T> *begin() const = 0;
    virtual IConstEnumerator<T> *end() const = 0;
};

#endif // IENUMERABLE_HPP
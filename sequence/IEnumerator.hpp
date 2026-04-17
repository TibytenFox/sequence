#ifndef IENUMERATOR_HPP
#define IENUMERATOR_HPP

template <class T>
class IEnumerator {
public:
    virtual ~IEnumerator() = default;

    virtual bool MoveNext() = 0;
    virtual const T &GetCurrent() const = 0;
};

#endif // IENUMERATOR_HPP
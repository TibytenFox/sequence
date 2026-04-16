#ifndef ICOLLECTION_HPP
#define ICOLLECTION_HPP

template <class T>
class ICollection {
public:
    virtual ~ICollection() {}

    virtual const T &Get(int index) const = 0;
    virtual int GetLength() const = 0;

    virtual ICollection<T> *CreateEmpty() const = 0;
    virtual ICollection<T> *Clone() const = 0;
};

#endif // ICOLLECTION_HPP

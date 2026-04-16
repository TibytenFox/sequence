#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

class Exception {
protected:
    const char *message;

public:
    explicit Exception(const char *msg) : message(msg) {}
    virtual ~Exception() {}

    virtual const char *GetMessage() const {
        return message;
    }
};

class IndexOutOfRange : public Exception {
public:
    explicit IndexOutOfRange(const char *msg = "Index out of range") : Exception(msg) {}
};

class EmptyCollectionError : public Exception {
public:
    explicit EmptyCollectionError(const char *msg = "Collection is empty") : Exception(msg) {}
};

#endif // EXCEPTIONS_HPP
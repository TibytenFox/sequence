#include "Sequence.hpp"

// Map-Reduce

template <class T>
template <class T2>
Sequence<T2> *Sequence<T>::Map(T2 (*func)(T)) const {
    ISequenceBuilder<T> *builder = this->CreateBuilder();
    IConstEnumerator<T> *en = this->begin();
    IConstEnumerator<T> *end = this->end();

    for (; *en != *end; ++(*en)) {
        builder->Append(func(**en));
    }

    Sequence<T> *result = builder->Build();
    delete builder;
    delete en;
    delete end;
    return result;
}

template <class T>
Sequence<T> *Sequence<T>::Where(bool (*predicate)(T)) const {
    ISequenceBuilder<T> *builder = this->CreateBuilder();
    IConstEnumerator<T> *en = this->begin();
    IConstEnumerator<T> *end = this->end();

    for (; *en != *end; ++(*en)) {
        if (predicate(**en)) {
            builder->Append(**en);
        }
    }

    Sequence<T> *result = builder->Build();
    delete builder;
    delete en;
    delete end;
    return result;
}

template <class T>
template <class T2>
T2 Sequence<T>::Reduce(T2 (*func)(T2, T), T initial) const {
    T2 accum = initial;
    IConstEnumerator<T> *en = this->begin();
    IConstEnumerator<T> *end = this->end();

    for (; *en != *end; ++(*en)) {
        accum = func(accum, **en);
    }

    delete en;
    delete end;
    return accum;
}

// Getters

template <class T>
const T &Sequence<T>::GetFirst() const {
    if (this->GetLength() == 0) {
        throw EmptyCollectionError("GetFirst(): Sequence is empty");
    }
    return this->Get(0);
}

template <class T>
const T &Sequence<T>::GetLast() const {
    if (this->GetLength() == 0) {
        throw EmptyCollectionError("GetLast(): Sequence is empty");
    }
    return this->Get(this->GetLength() - 1);
}

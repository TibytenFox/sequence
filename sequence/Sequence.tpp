#include "Sequence.hpp"

// Map-Reduce

template <class T>
template <class T2>
Sequence<T2> *Sequence<T>::Map(T2 (*func)(T)) const {
    ISequenceBuilder<T> *builder = this->CreateBuilder();
    IEnumerator<T> *en = this->GetEnumerator();

        while (en->MoveNext()) {
        builder->Append(func(en->GetCurrent()));
    }

    Sequence<T> *result = builder->Build();
    delete builder;
    delete en;
    return result;
}

template <class T>
Sequence<T> *Sequence<T>::Where(bool (*predicate)(T)) const {
    ISequenceBuilder<T> *builder = this->CreateBuilder();
    IEnumerator<T> *en = this->GetEnumerator();

        while (en->MoveNext()) {
        if (predicate(en->GetCurrent())) {
            builder->Append(en->GetCurrent());
        }
    }

    Sequence<T> *result = builder->Build();
    delete builder;
    delete en;
    return result;
}

template <class T>
template <class T2>
T2 Sequence<T>::Reduce(T2 (*func)(T2, T), T initial) const {
    T2 accum = initial;
    IEnumerator<T> *en = this->GetEnumerator();

    while (en->MoveNext()) {
        accum = func(accum, en->GetCurrent());
    }

    delete en;
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

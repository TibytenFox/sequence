#ifndef SEQUENCE_OUTPUT_HPP
#define SEQUENCE_OUTPUT_HPP

#include <ostream>
#include "Sequence.hpp"

template <class T>
std::ostream &operator<< (std::ostream &os, const Sequence<T> &seq) {
    os << '[';
    bool is_first = true;

    IEnumerator<T> *en = seq.GetEnumerator();

    while (en->MoveNext()) {
        if (!is_first) {
            os << ", ";
        }
        os << en->GetCurrent();
        is_first = false;
    }
    os << ']';
    delete en;
    return os;
}

#endif // SEQUENCE_OUTPUT_HPP
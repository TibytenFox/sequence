#ifndef SEQUENCE_OUTPUT_HPP
#define SEQUENCE_OUTPUT_HPP

#include <ostream>
#include "Sequence.hpp"

template <class T>
std::ostream &operator<< (std::ostream &os, const Sequence<T> &seq) {
    os << '[';
    bool is_first = true;

    IConstEnumerator<T> *en = seq.begin();
    IConstEnumerator<T> *end = seq.end();


    for (; *en != *end; ++(*end)) {
        if (!is_first) {
            os << ', ';
        }
        os << **en;
        is_first = false;
    }
    os << ']';
    return os;
}

#endif // SEQUENCE_OUTPUT_HPP
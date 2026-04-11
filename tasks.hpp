// tasks.hpp
#pragma once

#include <cmath>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "sequence/array_sequence.hpp"
#include "sequence/list_sequence.hpp"

// -------------------- П-1 --------------------
template <typename T>
struct MinMaxAvg {
    T min;
    T max;
    double avg;
};

template <typename T>
MinMaxAvg<T> ComputeMinMaxAvg(const Sequence<T> &seq) {
    if (seq.GetLength() == 0) {
        throw std::logic_error("Sequence is empty");
    }

    struct Accum {
        T min;
        T max;
        double sum;
        int count;
    };

    auto reducer = [](Accum acc, T x) -> Accum {
        if (x < acc.min) acc.min = x;
        if (x > acc.max) acc.max = x;
        acc.sum += x;
        acc.count++;
        return acc;
    };

    Accum init = { seq.Get(0), seq.Get(0), 0.0, 0 };
    Accum result = seq.Reduce<Accum>(reducer, init);
    return { result.min, result.max, result.sum / result.count };
}

// -------------------- П-2 ------------------------
template <typename T>
T ComputeMedian(const Sequence<T> &seq) {
    int n = seq.GetLength();
    if (n == 0) throw std::logic_error("Sequence is empty");

    std::vector<T> elements;
    elements.reserve(n);
    for (int i = 0; i < n; ++i) {
        elements.push_back(seq.Get(i));
    }
    std::sort(elements.begin(), elements.end());
    return elements[n / 2];
}

// -------------------- П-3 ------------------------
template <typename T>
long long CountInversions(const Sequence<T> &seq) {
    struct Accum {
        MutableArraySequence<T> prev;   // все предыдущие элементы
        long long inv_count;
    };

    auto reducer = [](Accum acc, T x) -> Accum {
        // считаем, сколько предыдущих элементов больше x
        for (const T& y : acc.prev) {
            if (y > x) ++acc.inv_count;
        }
        acc.prev.Append(x);
        return acc;
    };

    Accum init = { MutableArraySequence<T>(), 0 };
    Accum result = seq.Reduce<Accum>(reducer, init);
    return result.inv_count;
}

// -------------------- П-4 ------------------------
template <typename T>
ArraySequence<ArraySequence<T>> *ComputePrecedingSmaller(const Sequence<T> &seq) {
    struct Accum {
        MutableArraySequence<T> prev;                                 // предыдущие элементы
        MutableArraySequence<MutableArraySequence<T>> results;        // списки меньших для каждого элемента
    };

    auto reducer = [](Accum acc, T x) -> Accum {
        MutableArraySequence<T> smaller;
        for (const T& y : acc.prev) {
            if (y < x) smaller.Append(y);
        }
        acc.results.Append(smaller);
        acc.prev.Append(x);
        return acc;
    };

    Accum init = { MutableArraySequence<T>(), MutableArraySequence<MutableArraySequence<T>>() };
    Accum result = seq.Reduce<Accum>(reducer, init);
    return &result.results;
}

// -------------------- П-5 ------------------------
template <typename T>
ArraySequence<ArraySequence<T>> *GetAllPrefixes(const Sequence<T> &seq) {
    struct Accum {
        ArraySequence<T> current;                     // текущий префикс
        ArraySequence<ArraySequence<T>> prefixes;     // все префиксы
    };

    auto reducer = [](Accum acc, T x) -> Accum {
        // создаём новый префикс = старый + x
        ArraySequence<T> new_prefix = acc.current;
        new_prefix.Append(x);
        // сохраняем копию в результат
        acc.prefixes.Append(new_prefix);
        // обновляем текущий префикс
        acc.current = std::move(new_prefix);
        return acc;
    };

    Accum init = { ArraySequence<T>(), ArraySequence<ArraySequence<T>>() };
    Accum result = seq.Reduce<Accum>(reducer, init);
    return new ArraySequence<ArraySequence<T>>(result.prefixes);
}

template <typename T>
ArraySequence<ArraySequence<T>> *GetAllSuffixes(const Sequence<T> &seq) {
    int n = seq.GetLength();
    if (n == 0) return new ArraySequence<ArraySequence<T>>();

    // Теперь строим суффиксы, идя с конца (второй проход по индексам)
    auto *suffixes = new ArraySequence<ArraySequence<T>>();
    for (int start = n - 1; start >= 0; --start) {
        ArraySequence<T> suffix;
        for (int i = start; i < n; ++i) suffix.Append(seq[i]);
        suffixes->Append(suffix);
    }
    return suffixes;
}

// -------------------- П-6 ------------------------
template <typename T>
ArraySequence<double> *ComputeSlidingAverage(const Sequence<T> &seq) {
    int n = seq.GetLength();
    if (n < 3) return new ArraySequence<double>();  // пустой результат

    // 1-й проход: копируем в вектор
    std::vector<T> data;
    data.reserve(n);
    for (int i = 0; i < n; ++i) data.push_back(seq.Get(i));

    // 2-й проход: вычисляем средние
    auto* result = new ArraySequence<double>();
    for (int i = 1; i < n - 1; ++i) {
        double avg = (data[i-1] + data[i] + data[i+1]) / 3.0;
        result->Append(avg);
    }
    return result;
}

// -------------------- П-7 ------------------------
template <typename T>
ArraySequence<double> *ComputeRMSDeviation(const Sequence<T> &seq) {
    int n = seq.GetLength();
    if (n == 0) return new ArraySequence<double>();

    // 1-й проход: сумма квадратов
    struct SumSqAcc { double sum_sq; int count; };
    auto sum_sq_reducer = [](SumSqAcc acc, T x) -> SumSqAcc {
        acc.sum_sq += static_cast<double>(x) * x;
        acc.count++;
        return acc;
    };
    SumSqAcc init = { 0.0, 0 };
    SumSqAcc result = seq.Reduce<SumSqAcc>(sum_sq_reducer, init);
    double mean_sq = result.sum_sq / result.count;   // σ²

    // 2-й проход: вычисление sqrt(mean_sq - a_i²)
    auto* output = new ArraySequence<double>();
    for (int i = 0; i < n; ++i) {
        double a = static_cast<double>(seq.Get(i));
        double inside = mean_sq - a * a;
        if (inside < 0) inside = 0;   // защита от численных погрешностей
        output->Append(std::sqrt(inside));
    }
    return output;
}

// -------------------- П-8 ------------------------
template <typename T>
ArraySequence<T> *ComputeMirrorSum(const Sequence<T>& seq) {
    int n = seq.GetLength();
    auto *result = new ArraySequence<T>();
    for (int i = 0; i < n / 2; ++i) {
        T sum = seq.Get(i) + seq.Get(n - 1 - i);
        result->Append(sum);
    }
    if (n % 2 == 1) {
        // средний элемент складывается сам с собой
        result->Append(seq.Get(n / 2) + seq.Get(n / 2));
    }
    return result;
}
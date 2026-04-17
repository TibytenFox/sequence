#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Sequence.hpp"
#include "MutableArraySequence.hpp"

class SequenceUtilities {
public:
	template <class T1, class T2>
	struct Pair {
		T1 first;
		T2 second;

		Pair(): first(), second() {}
		Pair(T1 f, T2 s): first(f), second(s) {}

		bool operator==(const Pair &other) const {
			return (first == other.first) && (second == other.second);
		}
	};

	// TODO: разобраться, зачем нужен static здесь

	template <class T>
	static Sequence<Sequence<T>*> *Split(const Sequence<T> &seq, const T &element) {
		Sequence<Sequence<T>*> *result = new MutableArraySequence<Sequence<T>*>();
		Sequence<T> *current = new MutableArraySequence<T>();

		for (int i = 0; i < seq.GetLength(); i++) {
			if (seq[i] == element) {
				result->Append(current);
				current = new MutableArraySequence<T>();
			} else {
				current->Append(seq[i]);
			}
		}
		result->Append(current);
		return result;
	}

	template <class T1, class T2>
	static Sequence<Pair<T1, T2>> *Zip(const Sequence<T1> &seq1, const Sequence<T2> &seq2) {
		Sequence<Pair<T1, T2>> *result = new MutableArraySequence<Pair<T1, T2>>();

		int length = (seq1.GetLength() < seq2.GetLength()) ? seq1.GetLength() : seq2.GetLength();

		for (int i = 0; i < length; i++) {
			result->Append(Pair<T1, T2>(seq1[i], seq2[i]));
		}

		return result;
	}

	template <class T1, class T2>
	static Pair<Sequence<T1>*, Sequence<T2>*> Unzip(const Sequence<Pair<T1, T2>> &seq) {
		Sequence<T1> *seq1 = new MutableArraySequence<T1>();
		Sequence<T2> *seq2 = new MutableArraySequence<T2>();

		for (int i = 0; i < seq.GetLength(); i++) {
			seq1->Append(seq[i].first);
			seq2->Append(seq[i].second);
		}

		return Pair<Sequence<T1>*, Sequence<T2>*>(seq1, seq2);
	}

};

#endif // UTILITIES_HPP
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Sequence.hpp"
#include "MutableArraySequence.hpp"

namespace SequenceUtilities {
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

	template <class T>
	static Sequence<Sequence<T>*> *Split(const Sequence<T> &seq, const T &element) {
		Sequence<Sequence<T>*> *result = new MutableArraySequence<Sequence<T>*>();
		Sequence<T> *current = new MutableArraySequence<T>();
		IEnumerator<T> *en = seq.GetEnumerator();

		while (en->MoveNext()) {
			if (en->GetCurrent() == element) {
				result->Append(current);
				current = new MutableArraySequence<T>();
			} else {
				current->Append(en->GetCurrent());
			}
		}
		result->Append(current);

		delete en;
		return result;
	}

	template <class T1, class T2>
	static Sequence<Pair<T1, T2>> *Zip(const Sequence<T1> &seq1, const Sequence<T2> &seq2) {
		Sequence<Pair<T1, T2>> *result = new MutableArraySequence<Pair<T1, T2>>();
		IEnumerator<T1> *en1 = seq1.GetEnumerator();
		IEnumerator<T2> *en2 = seq2.GetEnumerator();

		while (en1->MoveNext() && en2->MoveNext()) {
			result->Append(Pair<T1, T2>(en1->GetCurrent(), en2->GetCurrent()));
		}

		return result;
	}

	template <class T1, class T2>
	static Pair<Sequence<T1>*, Sequence<T2>*> Unzip(const Sequence<Pair<T1, T2>> &seq) {
		Sequence<T1> *seq1 = new MutableArraySequence<T1>();
		Sequence<T2> *seq2 = new MutableArraySequence<T2>();
		IEnumerator<Pair<T1, T2>> *en = seq.GetEnumerator();

		while (en->MoveNext()) {
			seq1->Append(en->GetCurrent().first);
			seq2->Append(en->GetCurrent().second);
		}

		return Pair<Sequence<T1>*, Sequence<T2>*>(seq1, seq2);
	}

};

#endif // UTILITIES_HPP
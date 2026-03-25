#include <iostream>
#include "./sequence/array_sequence.h"
#include "./sequence/list_sequence.h"

template <typename Seq>
void printSequence(const Seq& seq, const std::string& name) {
    std::cout << name << " (length " << seq.GetLength() << "): ";
    for (int i = 0; i < seq.GetLength(); ++i) {
        std::cout << seq.Get(i) << " ";
    }
    std::cout << std::endl;
}

int main() {
    // ---------- Test ArraySequence ----------
    std::cout << "=== ArraySequence<int> ===" << std::endl;

    // Create from C-array using mutable version
    int initArr[] = {10, 20, 30};
    MutableArraySequence<int> arrSeq(initArr, 3);
    printSequence(arrSeq, "Initial");

    std::cout << "First: " << arrSeq.GetFirst() << ", Last: " << arrSeq.GetLast() << std::endl;

    // Modify (mutable, modifies in place)
    arrSeq.Append(40);
    arrSeq.Prepend(5);
    arrSeq.InsertAt(25, 2);  // insert 25 at index 2
    printSequence(arrSeq, "After Append(40), Prepend(5), InsertAt(25,2)");

    // Get subsequence (returns a new sequence)
    ArraySequence<int>* subArr = arrSeq.GetSubSequence(1, 4);
    printSequence(*subArr, "SubSequence [1..4]");
    delete subArr;

    // Concat with another mutable sequence
    int otherArr[] = {100, 200};
    MutableArraySequence<int> otherSeq(otherArr, 2);
    ArraySequence<int>* concatArr = arrSeq.Concat(&otherSeq);
    printSequence(*concatArr, "Concatenation with [100,200]");
    delete concatArr;

    std::cout << std::endl;

    // ---------- Test ListSequence ----------
    std::cout << "=== ListSequence<int> ===" << std::endl;

    // Create from C-array using mutable version
    int initList[] = {1, 2, 3, 4};
    MutableListSequence<int> listSeq(initList, 4);
    printSequence(listSeq, "Initial");

    std::cout << "First: " << listSeq.GetFirst() << ", Last: " << listSeq.GetLast() << std::endl;

    // Modify
    listSeq.Append(5);
    listSeq.Prepend(0);
    listSeq.InsertAt(99, 3);  // insert 99 at index 3
    printSequence(listSeq, "After Append(5), Prepend(0), InsertAt(99,3)");

    // Get subsequence
    ListSequence<int>* subList = listSeq.GetSubSequence(2, 5);
    printSequence(*subList, "SubSequence [2..5]");
    delete subList;

    // Concat
    int otherList[] = {10, 20};
    MutableListSequence<int> otherListSeq(otherList, 2);
    ListSequence<int>* concatList = listSeq.Concat(&otherListSeq);
    printSequence(*concatList, "Concatenation with [10,20]");
    delete concatList;

    return 0;
}
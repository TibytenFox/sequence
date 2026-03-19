#include <iostream>
#include "array_sequance.h"
#include "list_sequance.h"

template <typename Seq>
void printSequence(const Seq& seq, const std::string& name) {
    std::cout << name << " (length " << seq.GetLength() << "): ";
    for (int i = 0; i < seq.GetLength(); ++i) {
        std::cout << seq.Get(i) << " ";
    }
    std::cout << std::endl;
}

int main() {
    // ---------- Test ArraySequance ----------
    std::cout << "=== ArraySequance<int> ===" << std::endl;

    // Create from C-array
    int initArr[] = {10, 20, 30};
    ArraySequance<int> arrSeq(initArr, 3);
    printSequence(arrSeq, "Initial");

    std::cout << "First: " << arrSeq.GetFirst() << ", Last: " << arrSeq.GetLast() << std::endl;

    // Modify
    arrSeq.Append(40);
    arrSeq.Prepend(5);
    arrSeq.InsertAt(25, 2);  // insert 25 at index 2
    printSequence(arrSeq, "After Append(40), Prepend(5), InsertAt(25,2)");

    // Get subsequence
    ArraySequance<int>* subArr = arrSeq.GetSubSequance(1, 4);
    printSequence(*subArr, "SubSequence [1..4]");
    delete subArr;

    // Concat
    int otherArr[] = {100, 200};
    ArraySequance<int> otherSeq(otherArr, 2);
    ArraySequance<int>* concatArr = arrSeq.Concat(&otherSeq);
    printSequence(*concatArr, "Concatenation with [100,200]");
    delete concatArr;

    std::cout << std::endl;

    // ---------- Test ListSequance ----------
    std::cout << "=== ListSequance<int> ===" << std::endl;

    // Create from C-array
    int initList[] = {1, 2, 3, 4};
    ListSequance<int> listSeq(initList, 4);
    printSequence(listSeq, "Initial");

    std::cout << "First: " << listSeq.GetFirst() << ", Last: " << listSeq.GetLast() << std::endl;

    // Modify
    listSeq.Append(5);
    listSeq.Prepend(0);
    listSeq.InsertAt(99, 3);  // insert 99 at index 3
    printSequence(listSeq, "After Append(5), Prepend(0), InsertAt(99,3)");

    // Get subsequence
    ListSequance<int>* subList = listSeq.GetSubSequance(2, 5);
    printSequence(*subList, "SubSequence [2..5]");
    delete subList;

    // Concat
    int otherList[] = {10, 20};
    ListSequance<int> otherListSeq(otherList, 2);
    ListSequance<int>* concatList = listSeq.Concat(&otherListSeq);
    printSequence(*concatList, "Concatenation with [10,20]");
    delete concatList;

    return 0;
}
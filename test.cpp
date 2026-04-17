#include <iostream>
#include "sequence/MutableArraySequence.hpp"
#include "sequence/MutableListSequence.hpp"
#include "sequence/ImmutableArraySequence.hpp"
#include "sequence/ImmutableListSequence.hpp"
#include "sequence/Exceptions.hpp"
#include "sequence/Utilities.hpp"

#define TEST(name) void name(); \
    struct Register_##name { Register_##name() { register_test(name); std::cout << #name << '\n'; } } reg_##name; \
    void name()

static int tests_passed = 0;
static int tests_failed = 0;

void register_test(void (*f)()) {
    try {
        f();
        ++tests_passed;
        std::cout << "[PASS]\t";
    } catch (const Exception &e) {
        ++tests_failed;
        std::cout << "[FAIL] " << e.GetMessage() << "\t";
    } catch (...) {
        ++tests_failed;
        std::cout << "[FAIL] Unknown exception\t";
    }
}

#define ASSERT_EQ(x, y) \
    if ((x) != (y)) throw RunTimeError("ASSERT_EQ failed: " #x " != " #y)

#define ASSERT_THROWS(expr, exc) \
    do { \
        bool caught = false; \
        try { expr; } \
        catch (const exc&) { caught = true; } \
        catch (...) {} \
        if (!caught) throw RunTimeError("ASSERT_THROWS failed: " #expr); \
    } while(0)

#define ASSERT_NO_THROW(expr) \
    do { \
        try { expr; } \
        catch (...) { throw RunTimeError("ASSERT_NO_THROW failed: " #expr); } \
    } while(0)

// -------------------- Tests for ArraySequence --------------------

TEST(ArraySequence_DefaultConstructor) {
    MutableArraySequence<int> seq;
    ASSERT_EQ(seq.GetLength(), 0);
    ASSERT_THROWS(seq.GetFirst(), EmptyCollectionError);
    ASSERT_THROWS(seq.GetLast(), EmptyCollectionError);
}

TEST(ArraySequence_ConstructorFromArray) {
    int arr[] = {1, 2, 3};
    MutableArraySequence<int> seq(arr, 3);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.GetFirst(), 1);
    ASSERT_EQ(seq.GetLast(), 3);
    ASSERT_EQ(seq.Get(1), 2);
}

TEST(ArraySequence_CopyConstructor) {
    int arr[] = {5, 6, 7};
    MutableArraySequence<int> seq1(arr, 3);
    MutableArraySequence<int> seq2(seq1);
    ASSERT_EQ(seq2.GetLength(), 3);
    ASSERT_EQ(seq2.Get(0), 5);
    ASSERT_EQ(seq2.Get(2), 7);
}

TEST(ArraySequence_Append) {
    MutableArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    ASSERT_EQ(seq.GetLength(), 2);
    ASSERT_EQ(seq.Get(0), 10);
    ASSERT_EQ(seq.Get(1), 20);
    ASSERT_EQ(seq.GetLast(), 20);
}

TEST(ArraySequence_Prepend) {
    MutableArraySequence<int> seq;
    seq.Append(30);
    seq.Prepend(10);
    seq.Prepend(5);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.GetFirst(), 5);
    ASSERT_EQ(seq.Get(1), 10);
    ASSERT_EQ(seq.Get(2), 30);
}

TEST(ArraySequence_InsertAt) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(3);
    seq.InsertAt(2, 1);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.Get(1), 2);
    seq.InsertAt(0, 0);    
    ASSERT_EQ(seq.GetFirst(), 0);
    seq.InsertAt(4, seq.GetLength()); 
    ASSERT_EQ(seq.GetLast(), 4);
    ASSERT_THROWS(seq.InsertAt(99, 10), IndexOutOfRange);
}

TEST(ArraySequence_GetSubsequence) {
    int arr[] = {10, 20, 30, 40, 50};
    MutableArraySequence<int> seq(arr, 5);
    Sequence<int> *sub = seq.GetSubsequence(1, 3);
    ASSERT_EQ(sub->GetLength(), 3);
    ASSERT_EQ(sub->Get(0), 20);
    ASSERT_EQ(sub->Get(2), 40);
    delete sub;

    ASSERT_THROWS(seq.GetSubsequence(2, 1), IndexOutOfRange);
    ASSERT_THROWS(seq.GetSubsequence(-1, 2), IndexOutOfRange);
    ASSERT_THROWS(seq.GetSubsequence(2, 10), IndexOutOfRange);
}

TEST(ArraySequence_Concat) {
    int a[] = {1, 2};
    int b[] = {3, 4};
    MutableArraySequence<int> seqA(a, 2);
    MutableArraySequence<int> seqB(b, 2);
    Sequence<int> *concat = seqA.Concat(&seqB);
    ASSERT_EQ(concat->GetLength(), 4);
    ASSERT_EQ(concat->Get(0), 1);
    ASSERT_EQ(concat->Get(2), 3);
    ASSERT_EQ(concat->Get(3), 4);
    delete concat;
}

TEST(ArraySequence_Map) {
    MutableArraySequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3);
    Sequence<int> *squares = seq.Map<int>([](int x) { return x * x; });
    ASSERT_EQ(squares->GetLength(), 3);
    ASSERT_EQ(squares->Get(0), 1);
    ASSERT_EQ(squares->Get(1), 4);
    ASSERT_EQ(squares->Get(2), 9);
    delete squares;
}

TEST(ArraySequence_Where) {
    MutableArraySequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3); seq.Append(4);
    Sequence<int> *evens = seq.Where([](int x) { return x % 2 == 0; });
    ASSERT_EQ(evens->GetLength(), 2);
    ASSERT_EQ(evens->Get(0), 2);
    ASSERT_EQ(evens->Get(1), 4);
    delete evens;
}

TEST(ArraySequence_Reduce) {
    MutableArraySequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3); seq.Append(4);
    int sum = seq.Reduce<int>([](int acc, int x) { return acc + x; }, 0);
    ASSERT_EQ(sum, 10);
    int product = seq.Reduce<int>([](int acc, int x) { return acc * x; }, 1);
    ASSERT_EQ(product, 24);
}

TEST(ArraySequence_EqualityOperator) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {1, 2, 3};
    int arr3[] = {1, 2, 4};
    MutableArraySequence<int> seq1(arr1, 3);
    MutableArraySequence<int> seq2(arr2, 3);
    MutableArraySequence<int> seq3(arr3, 3);
    ASSERT_EQ(seq1 == seq2, true);
    ASSERT_EQ(seq1 == seq3, false);
    ASSERT_EQ(seq1 != seq3, true);
}

TEST(ArraySequence_SubscriptOperator) {
    MutableArraySequence<int> seq;
    seq.Append(10); seq.Append(20); seq.Append(30);
    ASSERT_EQ(seq[0], 10);
    ASSERT_EQ(seq[2], 30);
    seq[1] = 99;
    ASSERT_EQ(seq[1], 99);
    const MutableArraySequence<int> &constSeq = seq;
    ASSERT_EQ(constSeq[1], 99);
    ASSERT_THROWS(seq[5], IndexOutOfRange);
}

TEST(ArraySequence_Iterator) {
    MutableArraySequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3);
    int sum = 0;
    for (int i = 0; i < seq.GetLength(); i++) sum += seq[i];
    ASSERT_EQ(sum, 6);

    auto it = seq.GetEnumerator();
    it->MoveNext();
    ASSERT_EQ(it->GetCurrent(), 1);
    it->MoveNext();
    ASSERT_EQ(it->GetCurrent(), 2);
    delete it;
}

TEST(ArraySequence_OperatorPlus) {
    int a[] = {1, 2};
    int b[] = {3, 4};
    MutableArraySequence<int> seqA(a, 2);
    MutableArraySequence<int> seqB(b, 2);
    MutableArraySequence<int> result = seqA + seqB;
    ASSERT_EQ(result.GetLength(), 4);
    ASSERT_EQ(result.Get(0), 1);
    ASSERT_EQ(result.Get(2), 3);
}

TEST(ArraySequence_OperatorPlusEqual) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq += 2;
    ASSERT_EQ(seq.GetLength(), 2);
    ASSERT_EQ(seq.Get(1), 2);
    int c[] = {3, 4};
    MutableArraySequence<int> other(c, 2);
    seq += other;
    ASSERT_EQ(seq.GetLength(), 4);
    ASSERT_EQ(seq.Get(2), 3);
}

// -------------------- Tests for ListSequence --------------------

TEST(ListSequence_DefaultConstructor) {
    MutableListSequence<int> seq;
    ASSERT_EQ(seq.GetLength(), 0);
    ASSERT_THROWS(seq.GetFirst(), EmptyCollectionError);
}

TEST(ListSequence_ConstructorFromArray) {
    int arr[] = {1, 2, 3};
    MutableListSequence<int> seq(arr, 3);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.GetFirst(), 1);
    ASSERT_EQ(seq.GetLast(), 3);
    ASSERT_EQ(seq.Get(1), 2);
}

TEST(ListSequence_CopyConstructor) {
    int arr[] = {5, 6, 7};
    MutableListSequence<int> seq1(arr, 3);
    MutableListSequence<int> seq2(seq1);
    ASSERT_EQ(seq2.GetLength(), 3);
    ASSERT_EQ(seq2.Get(0), 5);
    ASSERT_EQ(seq2.Get(2), 7);
}

TEST(ListSequence_Append) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    ASSERT_EQ(seq.GetLength(), 2);
    ASSERT_EQ(seq.Get(0), 10);
    ASSERT_EQ(seq.Get(1), 20);
}

TEST(ListSequence_Prepend) {
    MutableListSequence<int> seq;
    seq.Append(30);
    seq.Prepend(10);
    seq.Prepend(5);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.GetFirst(), 5);
    ASSERT_EQ(seq.Get(1), 10);
    ASSERT_EQ(seq.Get(2), 30);
}

TEST(ListSequence_InsertAt) {
    MutableListSequence<int> seq;
    seq.Append(1); seq.Append(3);
    seq.InsertAt(2, 1);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.Get(1), 2);
    seq.InsertAt(0, 0);
    ASSERT_EQ(seq.GetFirst(), 0);
    seq.InsertAt(4, seq.GetLength());
    ASSERT_EQ(seq.GetLast(), 4);
    ASSERT_THROWS(seq.InsertAt(99, 10), IndexOutOfRange);
}

TEST(ListSequence_GetSubsequence) {
    int arr[] = {10, 20, 30, 40, 50};
    MutableListSequence<int> seq(arr, 5);
    Sequence<int> *sub = seq.GetSubsequence(1, 3);
    ASSERT_EQ(sub->GetLength(), 3);
    ASSERT_EQ(sub->Get(0), 20);
    ASSERT_EQ(sub->Get(2), 40);
    delete sub;
}

TEST(ListSequence_Concat) {
    int a[] = {1, 2};
    int b[] = {3, 4};
    MutableListSequence<int> seqA(a, 2);
    MutableListSequence<int> seqB(b, 2);
    Sequence<int> *concat = seqA.Concat(&seqB);
    ASSERT_EQ(concat->GetLength(), 4);
    ASSERT_EQ(concat->Get(0), 1);
    ASSERT_EQ(concat->Get(2), 3);
    delete concat;
}

TEST(ListSequence_Map) {
    MutableListSequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3);
    Sequence<int> *squares = seq.Map<int>([](int x) { return x * x; });
    ASSERT_EQ(squares->GetLength(), 3);
    ASSERT_EQ(squares->Get(0), 1);
    ASSERT_EQ(squares->Get(1), 4);
    ASSERT_EQ(squares->Get(2), 9);
    delete squares;
}

TEST(ListSequence_Where) {
    MutableListSequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3); seq.Append(4);
    Sequence<int> *evens = seq.Where([](int x) { return x % 2 == 0; });
    ASSERT_EQ(evens->GetLength(), 2);
    ASSERT_EQ(evens->Get(0), 2);
    ASSERT_EQ(evens->Get(1), 4);
    delete evens;
}

TEST(ListSequence_Reduce) {
    MutableListSequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3); seq.Append(4);
    int sum = seq.Reduce<int>([](int acc, int x) { return acc + x; }, 0);
    ASSERT_EQ(sum, 10);
}

TEST(ListSequence_EqualityOperator) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {1, 2, 3};
    int arr3[] = {1, 2, 4};
    MutableListSequence<int> seq1(arr1, 3);
    MutableListSequence<int> seq2(arr2, 3);
    MutableListSequence<int> seq3(arr3, 3);
    ASSERT_EQ(seq1 == seq2, true);
    ASSERT_EQ(seq1 == seq3, false);
}

TEST(ListSequence_SubscriptOperator) {
    MutableListSequence<int> seq;
    seq.Append(10); seq.Append(20); seq.Append(30);
    ASSERT_EQ(seq[0], 10);
    ASSERT_EQ(seq[2], 30);
    seq[1] = 99;
    ASSERT_EQ(seq[1], 99);
    const MutableListSequence<int>& constSeq = seq;
    ASSERT_EQ(constSeq[1], 99);
    ASSERT_THROWS(seq[5], IndexOutOfRange);
}

TEST(ListSequence_Iterator) {
    MutableListSequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3);
    int sum = 0;
    IEnumerator<int> *en = seq.GetEnumerator();
    while (en->MoveNext()) sum += en->GetCurrent();
    delete en;
    ASSERT_EQ(sum, 6);

    auto it = seq.GetEnumerator();
    it->MoveNext();
    ASSERT_EQ(it->GetCurrent(), 1);
    it->MoveNext();
    ASSERT_EQ(it->GetCurrent(), 2);
    delete it;
}

TEST(ListSequence_OperatorPlus) {
    int a[] = {1, 2};
    int b[] = {3, 4};
    MutableListSequence<int> seqA(a, 2);
    MutableListSequence<int> seqB(b, 2);
    MutableListSequence<int> result = seqA + seqB;
    ASSERT_EQ(result.GetLength(), 4);
    ASSERT_EQ(result.Get(0), 1);
    ASSERT_EQ(result.Get(2), 3);
}

TEST(ListSequence_OperatorPlusEqual) {
    MutableListSequence<int> seq;
    seq.Append(1);
    seq += 2;
    ASSERT_EQ(seq.GetLength(), 2);
    ASSERT_EQ(seq.Get(1), 2);
    int c[] = {3, 4};
    MutableListSequence<int> other(c, 2);
    seq += other;
    ASSERT_EQ(seq.GetLength(), 4);
    ASSERT_EQ(seq.Get(2), 3);
}

// -------------------- Tests for Utilities: Split (ArraySequence) --------------------

TEST(ArraySequence_SplitBasic) {
    int arr[] = {1, 0, 2, 0, 3};
    MutableArraySequence<int> seq(arr, 5);
    Sequence<Sequence<int>*> *result = SequenceUtilities::Split(seq, 0);
    ASSERT_EQ(result->GetLength(), 3);
    ASSERT_EQ(result->Get(0)->GetLength(), 1);
    ASSERT_EQ(result->Get(0)->Get(0), 1);
    ASSERT_EQ(result->Get(1)->GetLength(), 1);
    ASSERT_EQ(result->Get(1)->Get(0), 2);
    ASSERT_EQ(result->Get(2)->GetLength(), 1);
    ASSERT_EQ(result->Get(2)->Get(0), 3);
    delete result;
}

TEST(ArraySequence_SplitConsecutiveDelimiters) {
    int arr[] = {1, 0, 0, 2};
    MutableArraySequence<int> seq(arr, 4);
    Sequence<Sequence<int>*> *result = SequenceUtilities::Split(seq, 0);
    ASSERT_EQ(result->GetLength(), 3);
    ASSERT_EQ(result->Get(0)->GetLength(), 1);
    ASSERT_EQ(result->Get(0)->Get(0), 1);
    ASSERT_EQ(result->Get(1)->GetLength(), 0);
    ASSERT_EQ(result->Get(2)->GetLength(), 1);
    ASSERT_EQ(result->Get(2)->Get(0), 2);
    delete result;
}

TEST(ArraySequence_SplitNoDelimiter) {
    int arr[] = {1, 2, 3};
    MutableArraySequence<int> seq(arr, 3);
    Sequence<Sequence<int>*> *result = SequenceUtilities::Split(seq, 0);
    ASSERT_EQ(result->GetLength(), 1);
    ASSERT_EQ(result->Get(0)->GetLength(), 3);
    ASSERT_EQ(result->Get(0)->Get(0), 1);
    ASSERT_EQ(result->Get(0)->Get(2), 3);
    delete result;
}

TEST(ArraySequence_SplitAllDelimiters) {
    int arr[] = {0, 0, 0};
    MutableArraySequence<int> seq(arr, 3);
    Sequence<Sequence<int>*> *result = SequenceUtilities::Split(seq, 0);
    ASSERT_EQ(result->GetLength(), 4);
    ASSERT_EQ(result->Get(0)->GetLength(), 0);
    ASSERT_EQ(result->Get(1)->GetLength(), 0);
    ASSERT_EQ(result->Get(2)->GetLength(), 0);
    ASSERT_EQ(result->Get(3)->GetLength(), 0);
    delete result;
}

// -------------------- Tests for Utilities: Zip (ArraySequence) --------------------

TEST(ArraySequence_ZipBasic) {
    int a[] = {1, 2, 3};
    int b[] = {10, 20, 30};
    MutableArraySequence<int> seq1(a, 3);
    MutableArraySequence<int> seq2(b, 3);
    Sequence<SequenceUtilities::Pair<int, int>> *result = SequenceUtilities::Zip(seq1, seq2);
    ASSERT_EQ(result->GetLength(), 3);
    ASSERT_EQ(result->Get(0).first, 1);
    ASSERT_EQ(result->Get(0).second, 10);
    ASSERT_EQ(result->Get(1).first, 2);
    ASSERT_EQ(result->Get(1).second, 20);
    ASSERT_EQ(result->Get(2).first, 3);
    ASSERT_EQ(result->Get(2).second, 30);
    delete result;
}

TEST(ArraySequence_ZipDifferentLengths) {
    int a[] = {1, 2, 3, 4, 5};
    int b[] = {10, 20};
    MutableArraySequence<int> seq1(a, 5);
    MutableArraySequence<int> seq2(b, 2);
    Sequence<SequenceUtilities::Pair<int, int>> *result = SequenceUtilities::Zip(seq1, seq2);
    ASSERT_EQ(result->GetLength(), 2);
    ASSERT_EQ(result->Get(0).first, 1);
    ASSERT_EQ(result->Get(0).second, 10);
    ASSERT_EQ(result->Get(1).first, 2);
    ASSERT_EQ(result->Get(1).second, 20);
    delete result;
}

TEST(ArraySequence_ZipEmptySequence) {
    MutableArraySequence<int> seq1;
    MutableArraySequence<int> seq2;
    Sequence<SequenceUtilities::Pair<int, int>> *result = SequenceUtilities::Zip(seq1, seq2);
    ASSERT_EQ(result->GetLength(), 0);
    delete result;
}

// -------------------- Tests for Utilities: Unzip (ArraySequence) --------------------

TEST(ArraySequence_UnzipBasic) {
    MutableArraySequence<SequenceUtilities::Pair<int, int>> *pairs = 
        new MutableArraySequence<SequenceUtilities::Pair<int, int>>();
    pairs->Append(SequenceUtilities::Pair<int, int>(1, 10));
    pairs->Append(SequenceUtilities::Pair<int, int>(2, 20));
    pairs->Append(SequenceUtilities::Pair<int, int>(3, 30));
    
    auto result = SequenceUtilities::Unzip(*pairs);
    ASSERT_EQ(result.first->GetLength(), 3);
    ASSERT_EQ(result.second->GetLength(), 3);
    ASSERT_EQ(result.first->Get(0), 1);
    ASSERT_EQ(result.first->Get(1), 2);
    ASSERT_EQ(result.first->Get(2), 3);
    ASSERT_EQ(result.second->Get(0), 10);
    ASSERT_EQ(result.second->Get(1), 20);
    ASSERT_EQ(result.second->Get(2), 30);
    
    delete result.first;
    delete result.second;
    delete pairs;
}

TEST(ArraySequence_UnzipSinglePair) {
    MutableArraySequence<SequenceUtilities::Pair<int, int>> *pairs = 
        new MutableArraySequence<SequenceUtilities::Pair<int, int>>();
    pairs->Append(SequenceUtilities::Pair<int, int>(5, 50));
    
    auto result = SequenceUtilities::Unzip(*pairs);
    ASSERT_EQ(result.first->GetLength(), 1);
    ASSERT_EQ(result.second->GetLength(), 1);
    ASSERT_EQ(result.first->Get(0), 5);
    ASSERT_EQ(result.second->Get(0), 50);
    
    delete result.first;
    delete result.second;
    delete pairs;
}

TEST(ArraySequence_UnzipEmpty) {
    MutableArraySequence<SequenceUtilities::Pair<int, int>> *pairs = 
        new MutableArraySequence<SequenceUtilities::Pair<int, int>>();
    
    auto result = SequenceUtilities::Unzip(*pairs);
    ASSERT_EQ(result.first->GetLength(), 0);
    ASSERT_EQ(result.second->GetLength(), 0);
    
    delete result.first;
    delete result.second;
    delete pairs;
}

// -------------------- Tests for Utilities: Split (ListSequence) --------------------

TEST(ListSequence_SplitBasic) {
    int arr[] = {1, 0, 2, 0, 3};
    MutableListSequence<int> seq(arr, 5);
    Sequence<Sequence<int>*> *result = SequenceUtilities::Split(seq, 0);
    ASSERT_EQ(result->GetLength(), 3);
    ASSERT_EQ(result->Get(0)->GetLength(), 1);
    ASSERT_EQ(result->Get(0)->Get(0), 1);
    ASSERT_EQ(result->Get(1)->GetLength(), 1);
    ASSERT_EQ(result->Get(1)->Get(0), 2);
    ASSERT_EQ(result->Get(2)->GetLength(), 1);
    ASSERT_EQ(result->Get(2)->Get(0), 3);
    delete result;
}

TEST(ListSequence_SplitConsecutiveDelimiters) {
    int arr[] = {1, 5, 5, 2};
    MutableListSequence<int> seq(arr, 4);
    Sequence<Sequence<int>*> *result = SequenceUtilities::Split(seq, 5);
    ASSERT_EQ(result->GetLength(), 3);
    ASSERT_EQ(result->Get(0)->GetLength(), 1);
    ASSERT_EQ(result->Get(0)->Get(0), 1);
    ASSERT_EQ(result->Get(1)->GetLength(), 0);
    ASSERT_EQ(result->Get(2)->GetLength(), 1);
    ASSERT_EQ(result->Get(2)->Get(0), 2);
    delete result;
}

TEST(ListSequence_SplitNoDelimiter) {
    int arr[] = {1, 2, 3};
    MutableListSequence<int> seq(arr, 3);
    Sequence<Sequence<int>*> *result = SequenceUtilities::Split(seq, 0);
    ASSERT_EQ(result->GetLength(), 1);
    ASSERT_EQ(result->Get(0)->GetLength(), 3);
    ASSERT_EQ(result->Get(0)->Get(0), 1);
    ASSERT_EQ(result->Get(0)->Get(2), 3);
    delete result;
}

// -------------------- Tests for Utilities: Zip (ListSequence) --------------------

TEST(ListSequence_ZipBasic) {
    int a[] = {1, 2, 3};
    int b[] = {10, 20, 30};
    MutableListSequence<int> seq1(a, 3);
    MutableListSequence<int> seq2(b, 3);
    Sequence<SequenceUtilities::Pair<int, int>> *result = SequenceUtilities::Zip(seq1, seq2);
    ASSERT_EQ(result->GetLength(), 3);
    ASSERT_EQ(result->Get(0).first, 1);
    ASSERT_EQ(result->Get(0).second, 10);
    ASSERT_EQ(result->Get(1).first, 2);
    ASSERT_EQ(result->Get(1).second, 20);
    ASSERT_EQ(result->Get(2).first, 3);
    ASSERT_EQ(result->Get(2).second, 30);
    delete result;
}

TEST(ListSequence_ZipDifferentLengths) {
    int a[] = {1, 2, 3, 4};
    int b[] = {10, 20};
    MutableListSequence<int> seq1(a, 4);
    MutableListSequence<int> seq2(b, 2);
    Sequence<SequenceUtilities::Pair<int, int>> *result = SequenceUtilities::Zip(seq1, seq2);
    ASSERT_EQ(result->GetLength(), 2);
    ASSERT_EQ(result->Get(0).first, 1);
    ASSERT_EQ(result->Get(0).second, 10);
    ASSERT_EQ(result->Get(1).first, 2);
    ASSERT_EQ(result->Get(1).second, 20);
    delete result;
}

// -------------------- Tests for Utilities: Unzip (ListSequence) --------------------

TEST(ListSequence_UnzipBasic) {
    MutableListSequence<SequenceUtilities::Pair<int, int>> *pairs = 
        new MutableListSequence<SequenceUtilities::Pair<int, int>>();
    pairs->Append(SequenceUtilities::Pair<int, int>(1, 10));
    pairs->Append(SequenceUtilities::Pair<int, int>(2, 20));
    pairs->Append(SequenceUtilities::Pair<int, int>(3, 30));
    
    auto result = SequenceUtilities::Unzip(*pairs);
    ASSERT_EQ(result.first->GetLength(), 3);
    ASSERT_EQ(result.second->GetLength(), 3);
    ASSERT_EQ(result.first->Get(0), 1);
    ASSERT_EQ(result.first->Get(1), 2);
    ASSERT_EQ(result.first->Get(2), 3);
    ASSERT_EQ(result.second->Get(0), 10);
    ASSERT_EQ(result.second->Get(1), 20);
    ASSERT_EQ(result.second->Get(2), 30);
    
    delete result.first;
    delete result.second;
    delete pairs;
}

TEST(ListSequence_UnzipSinglePair) {
    MutableListSequence<SequenceUtilities::Pair<int, int>> *pairs = 
        new MutableListSequence<SequenceUtilities::Pair<int, int>>();
    pairs->Append(SequenceUtilities::Pair<int, int>(7, 70));
    
    auto result = SequenceUtilities::Unzip(*pairs);
    ASSERT_EQ(result.first->GetLength(), 1);
    ASSERT_EQ(result.second->GetLength(), 1);
    ASSERT_EQ(result.first->Get(0), 7);
    ASSERT_EQ(result.second->Get(0), 70);
    
    delete result.first;
    delete result.second;
    delete pairs;
}

// -------------------- Tests for ImmutableArraySequence --------------------

TEST(ImmutableArraySequence_DefaultConstructor) {
    ImmutableArraySequence<int> seq;
    ASSERT_EQ(seq.GetLength(), 0);
    ASSERT_THROWS(seq.GetFirst(), EmptyCollectionError);
    ASSERT_THROWS(seq.GetLast(), EmptyCollectionError);
}

TEST(ImmutableArraySequence_ConstructorFromArray) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.GetFirst(), 1);
    ASSERT_EQ(seq.GetLast(), 3);
    ASSERT_EQ(seq.Get(1), 2);
}

TEST(ImmutableArraySequence_CopyConstructor) {
    int arr[] = {5, 6, 7};
    ImmutableArraySequence<int> seq1(arr, 3);
    ImmutableArraySequence<int> seq2(seq1);
    ASSERT_EQ(seq2.GetLength(), 3);
    ASSERT_EQ(seq2.Get(0), 5);
    ASSERT_EQ(seq2.Get(2), 7);
}

TEST(ImmutableArraySequence_GetSubsequence) {
    int arr[] = {10, 20, 30, 40, 50};
    ImmutableArraySequence<int> seq(arr, 5);
    Sequence<int> *sub = seq.GetSubsequence(1, 3);
    ASSERT_EQ(sub->GetLength(), 3);
    ASSERT_EQ(sub->Get(0), 20);
    ASSERT_EQ(sub->Get(2), 40);
    delete sub;
}

TEST(ImmutableArraySequence_Concat) {
    int a[] = {1, 2};
    int b[] = {3, 4};
    ImmutableArraySequence<int> seqA(a, 2);
    ImmutableArraySequence<int> seqB(b, 2);
    Sequence<int> *concat = seqA.Concat(&seqB);
    ASSERT_EQ(concat->GetLength(), 4);
    ASSERT_EQ(concat->Get(0), 1);
    ASSERT_EQ(concat->Get(2), 3);
    ASSERT_EQ(concat->Get(3), 4);
    delete concat;
}

TEST(ImmutableArraySequence_Map) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    Sequence<int> *squares = seq.Map<int>([](int x) { return x * x; });
    ASSERT_EQ(squares->GetLength(), 3);
    ASSERT_EQ(squares->Get(0), 1);
    ASSERT_EQ(squares->Get(1), 4);
    ASSERT_EQ(squares->Get(2), 9);
    delete squares;
}

TEST(ImmutableArraySequence_Where) {
    int arr[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> seq(arr, 4);
    Sequence<int> *evens = seq.Where([](int x) { return x % 2 == 0; });
    ASSERT_EQ(evens->GetLength(), 2);
    ASSERT_EQ(evens->Get(0), 2);
    ASSERT_EQ(evens->Get(1), 4);
    delete evens;
}

TEST(ImmutableArraySequence_Reduce) {
    int arr[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> seq(arr, 4);
    int sum = seq.Reduce<int>([](int acc, int x) { return acc + x; }, 0);
    ASSERT_EQ(sum, 10);
}

TEST(ImmutableArraySequence_EqualityOperator) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {1, 2, 3};
    int arr3[] = {1, 2, 4};
    ImmutableArraySequence<int> seq1(arr1, 3);
    ImmutableArraySequence<int> seq2(arr2, 3);
    ImmutableArraySequence<int> seq3(arr3, 3);
    ASSERT_EQ(seq1 == seq2, true);
    ASSERT_EQ(seq1 == seq3, false);
}

TEST(ImmutableArraySequence_SubscriptOperator) {
    int arr[] = {10, 20, 30};
    ImmutableArraySequence<int> seq(arr, 3);
    ASSERT_EQ(seq[0], 10);
    ASSERT_EQ(seq[2], 30);
    const ImmutableArraySequence<int> &constSeq = seq;
    ASSERT_EQ(constSeq[1], 20);
    ASSERT_THROWS(seq[5], IndexOutOfRange);
}

TEST(ImmutableArraySequence_Iterator) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    int sum = 0;
    auto it = seq.GetEnumerator();
    while (it->MoveNext()) sum += it->GetCurrent();
    delete it;
    ASSERT_EQ(sum, 6);
}

// -------------------- Tests for ImmutableListSequence --------------------

TEST(ImmutableListSequence_DefaultConstructor) {
    ImmutableListSequence<int> seq;
    ASSERT_EQ(seq.GetLength(), 0);
    ASSERT_THROWS(seq.GetFirst(), EmptyCollectionError);
}

TEST(ImmutableListSequence_ConstructorFromArray) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> seq(arr, 3);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.GetFirst(), 1);
    ASSERT_EQ(seq.GetLast(), 3);
    ASSERT_EQ(seq.Get(1), 2);
}

TEST(ImmutableListSequence_CopyConstructor) {
    int arr[] = {5, 6, 7};
    ImmutableListSequence<int> seq1(arr, 3);
    ImmutableListSequence<int> seq2(seq1);
    ASSERT_EQ(seq2.GetLength(), 3);
    ASSERT_EQ(seq2.Get(0), 5);
    ASSERT_EQ(seq2.Get(2), 7);
}

TEST(ImmutableListSequence_GetSubsequence) {
    int arr[] = {10, 20, 30, 40, 50};
    ImmutableListSequence<int> seq(arr, 5);
    Sequence<int> *sub = seq.GetSubsequence(1, 3);
    ASSERT_EQ(sub->GetLength(), 3);
    ASSERT_EQ(sub->Get(0), 20);
    ASSERT_EQ(sub->Get(2), 40);
    delete sub;
}

TEST(ImmutableListSequence_Concat) {
    int a[] = {1, 2};
    int b[] = {3, 4};
    ImmutableListSequence<int> seqA(a, 2);
    ImmutableListSequence<int> seqB(b, 2);
    Sequence<int> *concat = seqA.Concat(&seqB);
    ASSERT_EQ(concat->GetLength(), 4);
    ASSERT_EQ(concat->Get(0), 1);
    ASSERT_EQ(concat->Get(2), 3);
    ASSERT_EQ(concat->Get(3), 4);
    delete concat;
}

TEST(ImmutableListSequence_Map) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> seq(arr, 3);
    Sequence<int> *squares = seq.Map<int>([](int x) { return x * x; });
    ASSERT_EQ(squares->GetLength(), 3);
    ASSERT_EQ(squares->Get(0), 1);
    ASSERT_EQ(squares->Get(1), 4);
    ASSERT_EQ(squares->Get(2), 9);
    delete squares;
}

TEST(ImmutableListSequence_Where) {
    int arr[] = {1, 2, 3, 4};
    ImmutableListSequence<int> seq(arr, 4);
    Sequence<int> *evens = seq.Where([](int x) { return x % 2 == 0; });
    ASSERT_EQ(evens->GetLength(), 2);
    ASSERT_EQ(evens->Get(0), 2);
    ASSERT_EQ(evens->Get(1), 4);
    delete evens;
}

TEST(ImmutableListSequence_Reduce) {
    int arr[] = {1, 2, 3, 4};
    ImmutableListSequence<int> seq(arr, 4);
    int sum = seq.Reduce<int>([](int acc, int x) { return acc + x; }, 0);
    ASSERT_EQ(sum, 10);
}

TEST(ImmutableListSequence_EqualityOperator) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {1, 2, 3};
    int arr3[] = {1, 2, 4};
    ImmutableListSequence<int> seq1(arr1, 3);
    ImmutableListSequence<int> seq2(arr2, 3);
    ImmutableListSequence<int> seq3(arr3, 3);
    ASSERT_EQ(seq1 == seq2, true);
    ASSERT_EQ(seq1 == seq3, false);
}

TEST(ImmutableListSequence_SubscriptOperator) {
    int arr[] = {10, 20, 30};
    ImmutableListSequence<int> seq(arr, 3);
    ASSERT_EQ(seq[0], 10);
    ASSERT_EQ(seq[2], 30);
    const ImmutableListSequence<int> &constSeq = seq;
    ASSERT_EQ(constSeq[1], 20);
    ASSERT_THROWS(seq[5], IndexOutOfRange);
}

TEST(ImmutableListSequence_Iterator) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> seq(arr, 3);
    int sum = 0;
    auto it = seq.GetEnumerator();
    while (it->MoveNext()) sum += it->GetCurrent();
    delete it;
    ASSERT_EQ(sum, 6);
}

// -------------------- MAIN --------------------
int main() {
    std::cout << "=== Running tests for ArraySequence and ListSequence ===\n";
    std::cout << "\nResults: passed " << tests_passed << ", failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}
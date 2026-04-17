#include <iostream>
#include "sequence/MutableArraySequence.hpp"
#include "sequence/MutableListSequence.hpp"
#include "sequence/Exceptions.hpp"
#include "sequence/SequenceOutput.hpp"

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

    auto it = seq.begin();
    ASSERT_EQ(**it, 1);
    ++(*it);
    ASSERT_EQ(**it, 2);
    --(*it);
    ASSERT_EQ(**it, 1);
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
    IEnumerator<int> *en = seq.begin();
    IEnumerator<int> *end = seq.end();
    for (; (*en) != (*end); ++(*en)) sum += **en;
    delete en;
    delete end;
    ASSERT_EQ(sum, 6);

    auto it = seq.begin();
    ASSERT_EQ(**it, 1);
    ++(*it);
    ASSERT_EQ(**it, 2);
    --(*it);
    ASSERT_EQ(**it, 1);
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

// -------------------- MAIN --------------------

int main() {
    std::cout << "=== Running tests for ArraySequence and ListSequence ===\n";
    std::cout << "\nResults: passed " << tests_passed << ", failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}
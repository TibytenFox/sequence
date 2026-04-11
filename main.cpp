#include <iostream>
#include <string>
#include <limits>
#include "sequence/array_sequence.hpp"
#include "sequence/list_sequence.hpp"

// Helper: print a sequence
template <class Seq>
void printSequence(const Seq &seq, const std::string &name = "Sequence") {
    std::cout << name << " (length " << seq.GetLength() << "): ";
    for (int i = 0; i < seq.GetLength(); ++i)
        std::cout << seq.Get(i) << " ";
    std::cout << std::endl;
}

// Predicate for Where: keep numbers > threshold
bool greaterThanFive(int x) { return x > 5; }

// Example function for Map: square
int square(int x) { return x * x; }

// Example reducer: sum
int sumReducer(int acc, int x) { return acc + x; }

// Helper to read an integer from stdin with validation
int readInt(const std::string &prompt) {
    int val;
    std::cout << prompt;
    std::cin >> val;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an integer: ";
        std::cin >> val;
    }
    return val;
}

// Helper to read a sequence from user input
template <class Seq>
Seq *createSequenceFromUser() {
    int n = readInt("How many elements? ");
    int *arr = new int[n];
    std::cout << "Enter " << n << " integers: ";
    for (int i = 0; i < n; ++i)
        std::cin >> arr[i];
    Seq *seq = new Seq(arr, n);
    delete[] arr;
    return seq;
}

// Menu for array sequence
void arraySequenceMenu() {
    MutableArraySequence<int> *seq = nullptr;
    std::cout << "\n--- MutableArraySequence ---\n";
    int choice = 0;
    while (choice != 10) {
        std::cout << "\n1. Create new sequence\n";
        std::cout << "2. Append\n3. Prepend\n4. Insert at index\n";
        std::cout << "5. Get element at index\n6. Print\n";
        std::cout << "7. Get subsequence\n8. Concat with another sequence\n";
        std::cout << "9. Where (filter >5) and Reduce (sum)\n10. Exit\n";
        choice = readInt("Your choice: ");

        switch (choice) {
            case 1: {
                if (seq) delete seq;
                seq = createSequenceFromUser<MutableArraySequence<int>>();
                printSequence(*seq, "Current sequence");
                break;
            }
            case 2: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int val = readInt("Value to append: ");
                seq->Append(val);
                printSequence(*seq, "After append");
                break;
            }
            case 3: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int val = readInt("Value to prepend: ");
                seq->Prepend(val);
                printSequence(*seq, "After prepend");
                break;
            }
            case 4: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int idx = readInt("Index: ");
                int val = readInt("Value: ");
                try {
                    seq->InsertAt(val, idx);
                    printSequence(*seq, "After insert");
                } catch (const std::exception &e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 5: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int idx = readInt("Index: ");
                try {
                    std::cout << "Element at index " << idx << " = " << seq->Get(idx) << std::endl;
                } catch (const std::exception &e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 6: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                printSequence(*seq, "Sequence");
                break;
            }
            case 7: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int start = readInt("Start index: ");
                int end = readInt("End index: ");
                try {
                    ArraySequence<int>* sub = seq->GetSubSequence(start, end);
                    printSequence(*sub, "Subsequence");
                    delete sub;
                } catch (const std::exception &e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 8: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                std::cout << "Create another sequence to concatenate:\n";
                MutableArraySequence<int> *other = createSequenceFromUser<MutableArraySequence<int>>();
                ArraySequence<int> *concat = seq->Concat(other);
                printSequence(*concat, "Concatenated sequence");
                delete concat;
                delete other;
                break;
            }
            case 9: {
                if (!seq) {std::cout << "No sequence. Create one first.\n"; break; }
                // Where
                ArraySequence<int> *filtered = seq->Where(greaterThanFive);
                printSequence(*filtered, "Filtered (elements >5)");
                delete filtered;
                // Reduce (sum)
                int sum = seq->Reduce(sumReducer, 0);
                std::cout << "Sum of all elements: " << sum << std::endl;
                break;
            }
            case 10:
                std::cout << "Exiting array sequence menu.\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
    delete seq;
}

// Menu for list sequence (identical operations)
void listSequenceMenu() {
    MutableListSequence<int> *seq = nullptr;
    std::cout << "\n--- MutableListSequence ---\n";
    int choice = 0;
    while (choice != 10) {
        std::cout << "\n1. Create new sequence\n";
        std::cout << "2. Append\n3. Prepend\n4. Insert at index\n";
        std::cout << "5. Get element at index\n6. Print\n";
        std::cout << "7. Get subsequence\n8. Concat with another sequence\n";
        std::cout << "9. Where (filter >5) and Reduce (sum)\n10. Exit\n";
        choice = readInt("Your choice: ");

        switch (choice) {
            case 1: {
                if (seq) delete seq;
                seq = createSequenceFromUser<MutableListSequence<int>>();
                printSequence(*seq, "Current sequence");
                break;
            }
            case 2: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int val = readInt("Value to append: ");
                seq->Append(val);
                printSequence(*seq, "After append");
                break;
            }
            case 3: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int val = readInt("Value to prepend: ");
                seq->Prepend(val);
                printSequence(*seq, "After prepend");
                break;
            }
            case 4: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int idx = readInt("Index: ");
                int val = readInt("Value: ");
                try {
                    seq->InsertAt(val, idx);
                    printSequence(*seq, "After insert");
                } catch (const std::exception &e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 5: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int idx = readInt("Index: ");
                try {
                    std::cout << "Element at index " << idx << " = " << seq->Get(idx) << std::endl;
                } catch (const std::exception &e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 6: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                printSequence(*seq, "Sequence");
                break;
            }
            case 7: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                int start = readInt("Start index: ");
                int end = readInt("End index: ");
                try {
                    ListSequence<int>* sub = seq->GetSubSequence(start, end);
                    printSequence(*sub, "Subsequence");
                    delete sub;
                } catch (const std::exception &e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 8: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                std::cout << "Create another sequence to concatenate:\n";
                MutableListSequence<int> *other = createSequenceFromUser<MutableListSequence<int>>();
                ListSequence<int> *concat = seq->Concat(other);
                printSequence(*concat, "Concatenated sequence");
                delete concat;
                delete other;
                break;
            }
            case 9: {
                if (!seq) { std::cout << "No sequence. Create one first.\n"; break; }
                ListSequence<int> *filtered = seq->Where(greaterThanFive);
                printSequence(*filtered, "Filtered (elements >5)");
                delete filtered;
                int sum = seq->Reduce(sumReducer, 0);
                std::cout << "Sum of all elements: " << sum << std::endl;
                break;
            }
            case 10:
                std::cout << "Exiting list sequence menu.\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
    delete seq;
}

int main() {
    std::cout << "=== Interactive Sequence Tester ===\n";
    while (true) {
        std::cout << "\nSelect sequence type:\n";
        std::cout << "1. MutableArraySequence\n2. MutableListSequence\n3. Exit\n";
        int type = readInt("Choice: ");
        if (type == 1)
            arraySequenceMenu();
        else if (type == 2)
            listSequenceMenu();
        else if (type == 3)
            break;
        else
            std::cout << "Invalid choice.\n";
    }
    std::cout << "Goodbye!\n";
    return 0;
}
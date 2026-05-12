#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "Exceptions.hpp"
#include "IEnumerable.hpp"

template <class T>
class DynamicArray : IEnumerable<T> {
private:
    T *data;     // Raw pointer to heap-allocated array
    int size;    // Number of elements
public:
    class Enumerator : public IEnumerator<T> {
    private:
        const DynamicArray<T> *array;
        int current_index;
    public:
        explicit Enumerator(const DynamicArray<T> *ptr) : array(ptr), current_index(-1) {}

        bool MoveNext() override {
            if (this->current_index + 1 < this->array->GetSize()) {
                this->current_index++;
                return true;
            }
            return false;
        }

        const T &GetCurrent() const override {
            if (this->current_index < 0 || this->current_index >= this->array->GetSize()) {
                throw IndexOutOfRange("Enumerator: out of bounds");
            }
            return this->array->data[this->current_index];
        }
    };

    // ---------- Constructors / Destructor ----------
    DynamicArray();                              // Empty array
    explicit DynamicArray(int s);                // Array of size s (default-initialized)
    DynamicArray(T *items, int count);           // Copy from C-array
    DynamicArray(const DynamicArray &other);     // Copy constructor
    DynamicArray(DynamicArray &&other) noexcept; // Move constructor
    ~DynamicArray();                             // Destructor

    // ---------- Accessors ----------
    const T &Get(int index) const;                   // Get element at index
    int GetSize() const;                             // Return current size
    void Set(int index, T value);                    // Assign value at index
    void Resize(int new_size);                       // Change size (preserve existing data)
    virtual IEnumerator<T>* GetEnumerator() const override;  //

    // ---------- Operators ----------
    DynamicArray<T> &operator=(const DynamicArray<T> &other);
    DynamicArray<T> &operator=(DynamicArray<T> &&other) noexcept;
    T &operator[](int index);
    const T &operator[](int index) const;

};

#include "DynamicArray.tpp"
#endif // DYNAMIC_ARRAY_HPP
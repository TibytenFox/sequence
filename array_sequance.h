#pragma once 
#include "sequence.h"
#include "dynamic_array.h"

template <class T>
class ArraySequance : public Sequance<T> {
private:
    DynamicArray<T> *items;
public:
    ArraySequance() : items(new DynamicArray<T>(0)) {}
    ArraySequance(T *items, int count) : items(new DynamicArray<T>(items, count)) {}
    ArraySequance(const ArraySequance<T> &list) : items(new DynamicArray<T>(*list.items)) {}
    T GetFirst() const { return items->Get(0); }
    T GetLast() const { return items->Get(items->GetSize() - 1); }
    T Get(int index) const { return items->Get(index); }
    ArraySequance<T> *GetSubSequance(int start_index, int end_index) {
        if (start_index < 0 || end_index >= items->GetSize() || start_index > end_index) {
            throw std::out_of_range("Index out of range");
        }

        ArraySequance<T> *sublist = new ArraySequance<T>();
        for (int i = start_index; i <= end_index; i++) {
            sublist->Append(items->Get(i));
        }
        return sublist;
    }
    int GetLength() const { return items->GetSize(); }
    void Append(T item) {
        items->Resize(items->GetSize() + 1);
        items->Set(items->GetSize() - 1, item);
    }
    void Prepend(T item) {
        items->Resize(items->GetSize() + 1);
        for (int i = items->GetSize() - 1; i > 0; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(0, item);
    }
    void InsertAt(T item, int index) {  
        if (index < 0 || index > items->GetSize()) {
            throw std::out_of_range("Index out of range");
        }

        items->Resize(items->GetSize() + 1);
        for (int i = items->GetSize() - 1; i > index; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
    }
    ArraySequance<T> *Concat(ArraySequance<T> *list) {
        ArraySequance<T> *new_list = new ArraySequance<T>(*this);
        for (int i = 0; i < list->GetLength(); i++) {
            new_list->Append(list->Get(i));
        }
        return new_list;
    }
    ~ArraySequance() { delete items; }
};
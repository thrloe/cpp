#include <iostream>
#include <utility>
#include <cstdlib> 
#include <stdexcept>

using namespace std;

class vector2 {
private:
    pair<int, int>* b;
    int size;
    int capacity;

    void resize() {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        pair<int, int>* new_b = (pair<int, int>*) realloc(b, capacity * sizeof(pair<int, int>));
        if (new_b == nullptr) {
            throw std::runtime_error("Memory allocation failed during resize");
        }
        b = new_b;
    }

public:
    vector2() : b(nullptr), size(0), capacity(0) {}

    void append(pair<int, int> X) {
        if (size >= capacity) {
            resize(); 
        }
        b[size] = X;
        size++;
    }

    pair<int, int> getAT(int i) const {
        if (i < 0 || i >= size) {
            throw std::out_of_range("nuh oh");
        }
        return b[i];
    }

    int getSize() const {
        return size;
    }
};

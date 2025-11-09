#include "Sorting.h"
#include <algorithm>

void sortComplex(std::vector<Complex>& arr) {
    std::sort(arr.begin(), arr.end(), [](const Complex& a, const Complex& b) {
        return a.abs() < b.abs();
    });
}

#include <iostream>
#include <vector>
#include "Complex/Complex.h"
#include "Sorting/Sorting.h"

int main() {
    std::vector<Complex> numbers = {
        Complex(3, 4),
        Complex(1, 1),
        Complex(0, -2),
        Complex(-2, 2)
    };

    std::cout << "Before sorting:\n";
    for (const auto& c : numbers) {
        c.print();
        std::cout << " ";
    }
    std::cout << "\n";

    sortComplex(numbers);

    std::cout << "After sorting\n";
    for (const auto& c : numbers) {
        c.print();
        std::cout << " ";
    }
    std::cout << "\n";

    return 0;
}

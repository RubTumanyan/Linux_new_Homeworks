#include "Complex.h"

Complex::Complex(double r, double i) : real(r), imag(i) {}

double Complex::getReal() const { return real; }
double Complex::getImag() const { return imag; }

Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::multiplyByConstant(double c) const {
    return Complex(real * c, imag * c);
}

double Complex::abs() const {
    return std::sqrt(real*real + imag*imag);
}

void Complex::print() const {
    std::cout << real;
    if (imag >= 0) std::cout << "+";
    std::cout << imag << "i";
}

~
~

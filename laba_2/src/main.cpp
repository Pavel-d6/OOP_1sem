#include <iostream>
#include "Polynomial.hpp"

int main() {
    double c1[] = {1.0, 2.0};        
    double c2[] = {0.0, 1.0, 1.0};  
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);

    Polynomial s(a);
    s.add(b);                        
    s.print(std::cout);
    std::cout << "\nP(2) = " << s.evaluate(2.0) << "\n";    

    Polynomial m(a);
    m.mul(b);                       
    m.print(std::cout);
    std::cout << "\n";

    std::cout << "derivative: ";
    m.derivative().print(std::cout);
    std::cout << "\nintegral: ";
    m.integral().print(std::cout);
    std::cout << "\n";
    return 0;
}
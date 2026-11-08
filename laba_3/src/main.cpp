#include <iostream>
#include <utility>

#include "PointSet.hpp"

int main() {
    PointSet a;
    std::cout << "add (1, 1): " << std::boolalpha << a.add(Point2D(1, 1)) << '\n';
    std::cout << "add (1, 1) ещё раз: " << a.add(Point2D(1, 1)) << '\n';
    a += Point2D(2, 2);
    a += Point2D(3, 3);
    std::cout << "A = " << a << ", size = " << a.size() << '\n';

    std::cout << "contains (2, 2): " << a.contains(Point2D(2, 2)) << '\n';
    std::cout << "a(Point2D(9, 9)): " << a(Point2D(9, 9)) << '\n';

    std::cout << "remove (2, 2): " << a.remove(Point2D(2, 2)) << '\n';
    std::cout << "remove (2, 2) ещё раз: " << a.remove(Point2D(2, 2)) << '\n';
    std::cout << "A после remove = " << a << '\n';

    PointSet b;
    b += Point2D(3, 3);
    b += Point2D(4, 4);
    b += Point2D(5, 5);
    std::cout << "\nA = " << a << '\n';
    std::cout << "B = " << b << '\n';
    std::cout << "A | B = " << (a | b) << '\n';
    std::cout << "A & B = " << (a & b) << '\n';
    std::cout << "A - B = " << (a - b) << '\n';

    PointSet c;
    c += Point2D(3, 3);
    c += Point2D(1, 1);
    std::cout << "\nC = " << c << '\n';
    std::cout << "A == C: " << (a == c) << ", A != B: " << (a != b) << '\n';

    PointSet copy(a);
    copy += Point2D(7, 7);
    std::cout << "\nкопия = " << copy << ", оригинал = " << a << '\n';

    PointSet moved(std::move(copy));
    std::cout << "после move: moved = " << moved << ", copy = " << copy
              << ", copy.empty() = " << copy.empty() << '\n';

    return 0;
}
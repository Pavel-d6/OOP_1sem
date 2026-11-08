#include "mainClass.hpp"
#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

namespace {
constexpr double G = 6.674e-11;
}

CelestialBody::CelestialBody(double M, double R) {
    if (!(M > 0) || !(R > 0))
        throw std::invalid_argument("Масса и радиус должны быть больше 0");
    M_ = M;
    R_ = R;
}

double CelestialBody::metric() const {
    return G * M_ / (R_ * R_);
}

std::ostream& operator<<(std::ostream& os, const CelestialBody& b) {
    b.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, CelestialBody& b) {
    b.read(is);
    return is;
}

bool operator==(const CelestialBody& a, const CelestialBody& b) {
    return std::abs(a.metric() - b.metric()) < EPS;
}
#include<iostream>
#include <cmath>
#include "moon.hpp"
#include <stdexcept>

Moon::Moon() : CelestialBody(1.0, 1.0 ), T_orbit_(1), T_rot_(1){}

Moon::Moon(double M, double R, double T_orbit, double T_rot) : CelestialBody(M, R) {
    if (!(T_orbit > 0) || !(T_rot > 0))
        throw std::invalid_argument("Периоды должны быть больше 0");
    T_orbit_ = T_orbit;
    T_rot_ = T_rot;
}

bool Moon::tidallyLocked() const {
    return std::abs(T_orbit_ - T_rot_) < EPS * T_orbit_;
}

void Moon::print(std::ostream& os) const {
    os << "Moon: M=" << M_ << " kg R=" << R_ << " m T_orbit=" << T_orbit_
       << " s T_rot=" << T_rot_ << " s";
}

void Moon::read(std::istream& is) {
    double M, R, T_orbit, T_rot;
    if (!(is >> M >> R >> T_orbit >> T_rot))
        return;                                  
    if (!(M > 0) || !(R > 0) || !(T_orbit > 0) || !(T_rot > 0)) {
        is.setstate(std::ios::failbit);    
        return;      
    }
    M_ = M; R_ = R; T_orbit_ = T_orbit; T_rot_ = T_rot;
}
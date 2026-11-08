#include "planet.hpp"
#include <cmath>
#include <stdexcept>


Planet::Planet(): CelestialBody(1.0, 1.0), T_(1) {}
Planet::Planet(double M, double R, double T) : CelestialBody(M,R), T_(T){}

void Planet::read(std::istream& is){
    double M, R, T;
    if (!(is>>M>>R>>T)) return;
    if (!((M>0)&&(R>0))){
        is.setstate(std::ios::failbit);    
        return;   
    }
    M_ = M; R_ = R; T_ = T;
}

void Planet::print(std::ostream& os) const{
    os << "Planet: M=" << M_ << " kg R=" << R_ << " m T=" << T_ << " K";
}

bool Planet::isHabitable() const {
    const double g = metric();
    return std::abs(T_ - 290.0) < 60.0 && g >= 5.0 && g <= 15.0;
}
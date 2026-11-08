#include<iostream>
#include "star.hpp"

Star::Star() : CelestialBody(1.0, 1.0), T_(1) {}
Star::Star(double M, double R, double T) : CelestialBody(M,R), T_(T){}

void Star::read(std::istream& is){
    double M, R, T;
    if (!(is>>M>>R>>T)) return;
    if (!((M>0)&&(R>0))){
        is.setstate(std::ios::failbit);    
        return;   
    }
    M_ = M; R_ = R; T_ = T;
}

void Star::print(std::ostream& os) const {
    os << "Star: M=" << M_ << " kg R=" << R_ << " m T=" << T_ << " K";
}

char Star::stellarClass() const {
    if (T_ >= 30000.0) return 'O';
    if (T_ >= 10000.0) return 'B';
    if (T_ >= 7500.0)  return 'A';
    if (T_ >= 6000.0)  return 'F';
    if (T_ >= 5200.0)  return 'G';
    if (T_ >= 3700.0)  return 'K';
    return 'M';
}
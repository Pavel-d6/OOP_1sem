#pragma once
#include "mainClass.hpp"

class Star : public CelestialBody {
    double T_;

public:
    Star();
    Star(double M, double R, double T);
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    char stellarClass() const;
};
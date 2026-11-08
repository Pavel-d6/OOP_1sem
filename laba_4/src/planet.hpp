#pragma once
#include<iostream>
#include "mainClass.hpp"

class Planet: public CelestialBody{
    private:
        double T_;
    public:
    Planet();
    Planet(double M, double R, double T);
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    bool isHabitable() const;
};
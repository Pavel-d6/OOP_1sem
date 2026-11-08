#pragma once
#include<iostream>
#include "mainClass.hpp"


class Moon : public CelestialBody{
    private:
        double T_orbit_, T_rot_;

    public:
        Moon();
        Moon(double M, double R, double T_orbit, double T_rot);
        void print(std::ostream& os) const override;
        void read(std::istream& is) override;
        bool tidallyLocked() const;

};
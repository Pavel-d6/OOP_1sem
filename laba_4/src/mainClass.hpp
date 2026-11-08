#pragma once
#include <iostream>
#include <ostream>

class CelestialBody{
    protected:
        double M_, R_;
    
    public:
    CelestialBody(double M, double R);
    virtual ~CelestialBody() = default;

    virtual double metric() const;
    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;

    operator double() const {return metric();}
};

inline constexpr double EPS = 1e-6;

std::ostream& operator<<(std::ostream&, const CelestialBody&);
std::istream& operator>>(std::istream&, CelestialBody&);

bool operator==(const CelestialBody&, const CelestialBody&);
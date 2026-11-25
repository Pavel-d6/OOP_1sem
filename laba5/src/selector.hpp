#pragma once

#include "point.hpp"

class Selector {
public:
    virtual ~Selector() = default;

    virtual bool match(const Point& p) const = 0;
};

class FirstQSelector : public Selector {
public:
    bool match(const Point& p) const override;
};

class AllSelector : public Selector {
public:
    bool match(const Point& p) const override;
};
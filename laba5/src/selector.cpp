#include "selector.hpp"
#include "point.hpp"


bool FirstQSelector::match(const Point& p) const {
    return p.x() > 0 && p.y() > 0;
}

bool AllSelector::match(const Point& p) const {
    return true;
}



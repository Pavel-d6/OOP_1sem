#include<iostream>
#include "Point2D.hpp"

Point2D::Point2D() : x(0), y(0) {}

Point2D::Point2D(int px, int py) : x(px), y(py) {}

int Point2D::getX() const { return x; }
int Point2D::getY() const { return y; }


bool Point2D::operator==(const Point2D& other) const {
    return x == other.x && y == other.y;
}

std::ostream& operator<<(std::ostream& os, const Point2D& p) {
    return os << '(' << p.x << ", " << p.y << ')';
}
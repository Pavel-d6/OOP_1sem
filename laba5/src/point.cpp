#include "point.hpp"
#include <ostream>
#include <cmath>

std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << p.x() << ' ' << p.y();
}

Point::Point() : x_(0.0), y_(0.0) { ++alive_; }
Point::Point(double x, double y) : x_(x), y_(y) { ++alive_; }
Point::Point(const Point& other) : x_(other.x_), y_(other.y_) { ++alive_; }
Point::~Point() { --alive_; }   

Point& Point::operator=(const Point& other){
    if (this != &other) {
        x_ = other.x_;
        y_ = other.y_;
    }
    return *this;
}

double Point::x() const { return x_; }
double Point::y() const { return y_; }

bool Point::operator<(const Point& other) const {
    if (x_ < other.x_) return true;
    if (x_ > other.x_) return false;
    return y_ < other.y_;
}
bool Point::operator==(const Point& other) const {
    return x_ == other.x_ && y_ == other.y_;
}
int Point::alive() { return alive_; }


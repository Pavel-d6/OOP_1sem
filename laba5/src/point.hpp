#pragma once
#include <iostream>




class Point {
public:
    Point();
    Point(double x, double y);
    Point(const Point& other);
    Point& operator=(const Point& other);
    ~Point();

    double x() const;
    double y() const;

    bool operator<(const Point& other) const;
    bool operator==(const Point& other) const;

    static int alive();

private:
    double x_;
    double y_;
    static inline int alive_ = 0;
};

std::ostream& operator<<(std::ostream& os, const Point& p);
#pragma once
#include <iostream>

class Point2D {
    
public:
    Point2D();
    Point2D(int px, int py);
    int getX() const;
    int getY() const;
    bool operator==(const Point2D& other) const;

private:
    int x;
    int y;
    friend std::ostream& operator<<(std::ostream& os, const Point2D& p);
};
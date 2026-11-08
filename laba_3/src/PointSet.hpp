#pragma once 
#include <cstddef>
#include "Point2D.hpp"
#include <utility>

class PointSet
{
private:
    Point2D* data_;
    std::size_t size_;
    std::size_t capacity_;
    void grow();
public:
    PointSet();
    ~PointSet();
    PointSet(const PointSet& other);
    void swap(PointSet& other) noexcept;
    PointSet& operator=(const PointSet& other);
    std::size_t size() const;
    PointSet(PointSet&& other) noexcept;
    PointSet& operator=(PointSet&& other) noexcept;
    bool empty() const;
    bool contains(const Point2D& p) const;
    bool operator()(const Point2D& p) const;
    bool add(const Point2D& p);
    bool remove(const Point2D& p);
    PointSet& operator+=(const Point2D& p);
    PointSet& operator-=(const Point2D& p);
    PointSet operator|(const PointSet& other) const;
    PointSet operator&(const PointSet& other) const;
    PointSet operator-(const PointSet& other) const;
    bool operator==(const PointSet& other) const;
    bool operator!=(const PointSet& other) const;
    friend std::ostream& operator<<(std::ostream& os, const PointSet& s);
};
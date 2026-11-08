#pragma once
#include <cstddef>
#include "mainClass.hpp"

struct TypeCounts {
    std::size_t stars = 0;
    std::size_t planets = 0;
    std::size_t moons = 0;
};


class BodyCollection {
    CelestialBody** data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
 
    void clear() noexcept;

    public:
    BodyCollection() = default;
    ~BodyCollection();
    BodyCollection(const BodyCollection& other);              
    BodyCollection(BodyCollection&& other) noexcept;
    BodyCollection& operator=(const BodyCollection& other);
    BodyCollection& operator=(BodyCollection&& other) noexcept;
 
    void add(CelestialBody* body);                           
    std::size_t size() const { return size_; }
    CelestialBody& operator[](std::size_t i);
    const CelestialBody& operator[](std::size_t i) const;
 
    double totalMetric() const;
    TypeCounts countByType() const;
};
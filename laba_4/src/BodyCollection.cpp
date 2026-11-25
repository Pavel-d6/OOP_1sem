#include "bodyCollection.hpp"
#include <stdexcept>
#include <utility>
#include "star.hpp"
#include "planet.hpp"
#include "moon.hpp"

void BodyCollection::clear() noexcept {
    for (std::size_t i = 0; i < size_; ++i)
        delete data_[i];
    delete[] data_;
    data_ = nullptr;
    size_ = capacity_ = 0;
}

BodyCollection::~BodyCollection() { clear(); }

BodyCollection::BodyCollection(const BodyCollection& other)
    : data_(other.size_ ? new CelestialBody*[other.size_] : nullptr),
      size_(0),
      capacity_(other.size_) {
    try {
        for (; size_ < other.size_; ++size_)
            data_[size_] = other.data_[size_]->clone();
    } catch (...) {
        clear();
        throw;
    }
}

BodyCollection::BodyCollection(BodyCollection&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = other.capacity_ = 0;
}

BodyCollection& BodyCollection::operator=(const BodyCollection& other) {
    if (this != &other) {
        BodyCollection tmp(other);
        *this = std::move(tmp);
    }
    return *this;
}

BodyCollection& BodyCollection::operator=(BodyCollection&& other) noexcept {
    if (this != &other) {
        clear();
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = other.capacity_ = 0;
    }
    return *this;
}

void BodyCollection::add(CelestialBody* body) {
    if (!body) throw std::invalid_argument("Нельзя добавить нулевой указатель");
    if (size_ == capacity_) {
        std::size_t newCap = capacity_ ? capacity_ * 2 : 4;
        CelestialBody** newData = new CelestialBody*[newCap];
        for (std::size_t i = 0; i < size_; ++i) newData[i] = data_[i];
        delete[] data_;
        data_ = newData;
        capacity_ = newCap;
    }
    data_[size_++] = body;
}

CelestialBody& BodyCollection::operator[](std::size_t i) {
    if (i >= size_) throw std::out_of_range("Индекс вне диапазона");
    return *data_[i];
}

const CelestialBody& BodyCollection::operator[](std::size_t i) const {
    if (i >= size_) throw std::out_of_range("Индекс вне диапазона");
    return *data_[i];
}

double BodyCollection::totalMetric() const {
    double sum = 0.0;
    for (std::size_t i = 0; i < size_; ++i) sum += data_[i]->metric();
    return sum;
}

TypeCounts BodyCollection::countByType() const {
    TypeCounts c;
    for (std::size_t i = 0; i < size_; ++i) {
        const CelestialBody* b = data_[i];
        if (dynamic_cast<const Star*>(b)) ++c.stars;
        else if (dynamic_cast<const Planet*>(b)) ++c.planets;
        else if (dynamic_cast<const Moon*>(b)) ++c.moons;
    }
    return c;
}

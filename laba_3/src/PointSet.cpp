#include<iostream>
#include "PointSet.hpp"

PointSet::PointSet() : data_(nullptr), size_(0), capacity_(0) {}
PointSet::~PointSet() { delete[] data_; }


PointSet::PointSet(const PointSet& other)
    : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
        data_ = new Point2D[capacity_];
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
}

void PointSet::swap(PointSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

PointSet& PointSet::operator=(const PointSet& other) {
    if (this != &other) {
        PointSet tmp(other);   
        swap(tmp);             
    }                          
    return *this;
}

PointSet::PointSet(PointSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

PointSet& PointSet::operator=(PointSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;              
        data_ = other.data_;         
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;        
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

std::size_t PointSet::size() const { return size_; }
bool PointSet::empty() const { return size_ == 0; }

bool PointSet::contains(const Point2D& p) const {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == p) return true;
    }
    return false;
}

bool PointSet::operator()(const Point2D& p) const { return contains(p); }

void PointSet::grow() {
    std::size_t newCap = (capacity_ == 0) ? 1 : capacity_ * 2;
    Point2D* newData = new Point2D[newCap];
    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCap;
}

bool PointSet::add(const Point2D& p) {
    if (contains(p)) return false;
    if (size_ == capacity_) grow();
    data_[size_++] = p;
    return true;
}

bool PointSet::remove(const Point2D& p) {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == p) {
            for (std::size_t j = i; j + 1 < size_; ++j) {
                data_[j] = data_[j + 1];   
            }
            --size_;
            return true;
        }
    }
    return false;
}

PointSet& PointSet::operator+=(const Point2D& p) { add(p); return *this; }
PointSet& PointSet::operator-=(const Point2D& p) { remove(p); return *this; }


PointSet PointSet::operator|(const PointSet& other) const {
    PointSet result(*this);                    
    for (std::size_t i = 0; i < other.size_; ++i) {
        result.add(other.data_[i]);            
    }
    return result;
}

PointSet PointSet::operator&(const PointSet& other) const {
    PointSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }
    return result;
}

PointSet PointSet::operator-(const PointSet& other) const {
    PointSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }
    return result;
}

bool PointSet::operator==(const PointSet& other) const {
    if (size_ != other.size_) return false;
    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) return false;   
    }
    return true;
}

bool PointSet::operator!=(const PointSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const PointSet& s) {
    if (s.size_ == 0) return os << "{ }";
    os << "{ ";
    for (std::size_t i = 0; i < s.size_; ++i) {
        if (i > 0) os << ", ";
        os << s.data_[i];
    }
    return os << " }";
}
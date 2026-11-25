#pragma once

#include "point.hpp"
#include <memory>
#include <cstddef>

class Selection {
public:
    Selection();
    ~Selection() = default;

    Selection(const Selection& other);
    Selection& operator=(const Selection& other);

    Selection(Selection&& other) noexcept;
    Selection& operator=(Selection&& other) noexcept;

    void append(const std::weak_ptr<Point>& p);

    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;

    const std::weak_ptr<Point>& operator[](
        std::size_t index) const;

private:
    std::unique_ptr<std::weak_ptr<Point>[]> data_;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;

    void reserve(std::size_t new_capacity);
};
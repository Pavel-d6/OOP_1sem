#pragma once

#include "point.hpp"
#include <iostream>
#include <memory>
#include <cstddef>

class BST {
public:
    BST();
    ~BST() = default;

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    BST(BST&& other) noexcept;
    BST& operator=(BST&& other) noexcept;

    bool insert(const Point& p);
    bool remove(const Point& p);
    std::weak_ptr<Point> search(const Point& p) const;

    std::size_t size() const;
    std::size_t height() const;

    void print(std::ostream& os) const;
    void reversePrint(std::ostream& os) const;

private:
    struct Node {
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        std::shared_ptr<Point> data;

        Node(const Point& p)
            : left(nullptr), right(nullptr),
              data(std::make_shared<Point>(p)) {}
    };

    std::unique_ptr<Node> root;
    std::size_t size_ = 0;

    bool insertNode(std::unique_ptr<Node>& node, const Point& p);
    bool removeNode(std::unique_ptr<Node>& node, const Point& p);
    std::weak_ptr<Point> searchNode(const std::unique_ptr<Node>& node,const Point& p) const;
    std::size_t heightNode(const std::unique_ptr<Node>& node) const;
    void printNode(std::ostream& os,const std::unique_ptr<Node>& node) const;
    void reversePrintNode(std::ostream& os,const std::unique_ptr<Node>& node) const;
};
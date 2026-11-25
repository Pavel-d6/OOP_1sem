#include "tree.hpp"
#include <iostream>
#include <utility>
#include <algorithm>

BST::BST() : root(nullptr), size_(0) {}

BST::BST(BST&& other) noexcept : root(std::move(other.root)), size_(other.size_) {
    other.size_ = 0;
}

BST& BST::operator=(BST&& other) noexcept {
    if (this != &other) {
        root = std::move(other.root);
        size_ = other.size_;
        other.size_ = 0;
    }
    return *this;
}


bool BST::insert(const Point& p) {
    if (insertNode(root, p)) {
        ++size_;
        return true;
    }
    return false;
}

bool BST::insertNode(std::unique_ptr<Node>& node, const Point& p) {

    if (!node) {
        node = std::make_unique<Node>(p);
        return true;
    }

    if (p < *node->data) {
        return insertNode(node->left, p);
    } else if (*node->data < p) {
        return insertNode(node->right, p);
    }

    return false;
}


bool BST::remove(const Point& p) {
    if (removeNode(root, p)) {
        --size_;
        return true;
    }
    return false;
}

bool BST::removeNode(std::unique_ptr<Node>& node, const Point& p) {

    if (!node) {
        return false;
    }

    if (p < *node->data) {
        return removeNode(node->left, p);
    } else if (*node->data < p) {
        return removeNode(node->right, p);
    } else {
        if (!node->left) {
            auto child = std::move(node->right);
            node = std::move(child);

        } else if (!node->right) {
            auto child = std::move(node->left);
            node = std::move(child);

        } else {
            Node* minNode = node->right.get();

            while (minNode->left) {
                minNode = minNode->left.get();
            }

            node->data = minNode->data;
            removeNode(node->right, *minNode->data);
        }

        return true;
    }
}


std::weak_ptr<Point> BST::search(const Point& p) const {
    return searchNode(root, p);
}

std::weak_ptr<Point> BST::searchNode(const std::unique_ptr<Node>& node,const Point& p) const {

    if (!node) {
        return {};
    }

    if (p < *node->data) {
        return searchNode(node->left, p);
    } else if (*node->data < p) {
        return searchNode(node->right, p);
    } else {
        return node->data;
    }
}


std::size_t BST::size() const {
    return size_;
}


std::size_t BST::height() const {
    return heightNode(root);
}

std::size_t BST::heightNode(const std::unique_ptr<Node>& node) const {

    if (!node) {
        return 0;
    }

    return 1 + std::max(
        heightNode(node->left),
        heightNode(node->right)
    );
}


void BST::print(std::ostream& os) const {
    printNode(os, root);
}

void BST::printNode(std::ostream& os,const std::unique_ptr<Node>& node) const {

    if (!node) {
        return;
    }

    printNode(os, node->left);
    os << *node->data << '\n';
    printNode(os, node->right);
}


void BST::reversePrint(std::ostream& os) const {
    reversePrintNode(os, root);
}

void BST::reversePrintNode(std::ostream& os,const std::unique_ptr<Node>& node) const {

    if (!node) {
        return;
    }

    reversePrintNode(os, node->right);
    os << *node->data << '\n';
    reversePrintNode(os, node->left);
}
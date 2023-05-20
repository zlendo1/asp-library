#ifndef BINARYNODE_H
#define BINARYNODE_H

#include <memory>
#include <stdexcept>

template<typename T>
struct BinaryNode {
    T value;

    std::weak_ptr<BinaryNode> parent;

    std::shared_ptr<BinaryNode> left;
    std::shared_ptr<BinaryNode> right;

    ~BinaryNode();

    BinaryNode();
    BinaryNode(const T &value);
};

template<typename T>
BinaryNode<T>::~BinaryNode() {
    left = nullptr;
    right = nullptr;
}

template<typename T>
BinaryNode<T>::BinaryNode() {
    parent.reset();

    left = nullptr;
    right = nullptr;
}

template<typename T>
BinaryNode<T>::BinaryNode(const T &value) : BinaryNode() {
    BinaryNode::value = value;
}

template<typename T>
void connectLeft(std::shared_ptr<BinaryNode<T>> parent, std::shared_ptr<BinaryNode<T>> child) {
    parent->left = child;
    
    if (child) {
        child->parent = parent;
    }
}

template<typename T>
void connectRight(std::shared_ptr<BinaryNode<T>> parent, std::shared_ptr<BinaryNode<T>> child) {
    parent->right = child;
    
    if (child) {
        child->parent = parent;
    }
}

template<typename T>
std::shared_ptr<BinaryNode<T>> copyDownward(const std::shared_ptr<BinaryNode<T>> &node) {
    if (!node) {
        return nullptr;
    }

    std::shared_ptr<BinaryNode<T>> copy = std::make_shared<BinaryNode<T>>(node->value);

    copy->parent = node->parent;

    copy->left = copyDownward(node->left);
    copy->right = copyDownward(node->right);

    return copy;
}

template<typename T>
bool hasParent(std::shared_ptr<BinaryNode<T>> &node) {
    return node && !node->parent.expired();
}

template<typename T>
bool hasLeft(std::shared_ptr<BinaryNode<T>> &node) {
    return node && node->left;
}

template<typename T>
bool hasRight(std::shared_ptr<BinaryNode<T>> &node) {
    return node && node->right;
}

template<typename T>
bool isSaturated(std::shared_ptr<BinaryNode<T>> &node) {
    return node && node->left && node->right;
}

template<typename T>
bool hasChild(std::shared_ptr<BinaryNode<T>> &node) {
    return node && node->left || node->right;
}

template<typename T>
bool goParent(std::shared_ptr<BinaryNode<T>> &node) {
    if (hasParent(node)) {
        node = node->parent.lock();

        return true;
    }

    return false;
}

template<typename T>
bool goLeft(std::shared_ptr<BinaryNode<T>> &node) {
    if (hasLeft(node)) {
        node = node->left;

        return true;
    }

    return false;
}

template<typename T>
bool goRight(std::shared_ptr<BinaryNode<T>> &node) {
    if (hasRight(node)) {
        node = node->right;

        return true;
    }

    return false;
}

#endif
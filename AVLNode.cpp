#ifndef AVLNODE_H
#define AVLNODE_H

#include <stdexcept>

template <typename T>
struct AVLNode {
    T value;

    int height;

    AVLNode *parent;

    AVLNode *left;
    AVLNode *right;

    AVLNode();
    AVLNode(const T &value);
};

template <typename T>
AVLNode<T>::AVLNode() {
    parent = nullptr;

    left = nullptr;
    right = nullptr;

    height = 1;
}

template <typename T>
AVLNode<T>::AVLNode(const T &value): AVLNode() {
    AVLNode::value = value;
}

template <typename T>
void connectLeft(AVLNode<T> *parent, AVLNode<T> *child) {
    parent->left = child;

    if (child) {
        child->parent = parent;
    }
}

template <typename T>
void connectRight(AVLNode<T> *parent, AVLNode<T> *child) {
    parent->right = child;

    if (child) {
        child->parent = parent;
    }
}

template <typename T>
AVLNode<T>* copyDownward(const AVLNode<T> *node) {
    if (!node) {
        return nullptr;
    }

    AVLNode<T> *copy = new AVLNode<T> (node->value);

    copy->height = node->height;

    copy->left = copyDownward(node->left);

    if (hasLeft(copy)) {
        copy->left->parent = copy;
    }

    copy->right = copyDownward(node->right);

    if (hasRight(copy)) {
        copy->right->parent = copy;
    }

    return copy;
}

template <typename T>
bool hasParent(AVLNode<T> *&node) {
    return node &&node->parent;
}

template <typename T>
bool hasLeft(AVLNode<T> *&node) {
    return node &&node->left;
}

template <typename T>
bool hasRight(AVLNode<T> *&node) {
    return node &&node->right;
}

template <typename T>
bool isSaturated(AVLNode<T> *&node) {
    return node &&node->left &&node->right;
}

template <typename T>
bool hasChild(AVLNode<T> *&node) {
    return node &&(node->left || node->right);
}

template <typename T>
bool goParent(AVLNode<T> *&node) {
    if (hasParent(node)) {
        node = node->parent;

        return true;
    }

    return false;
}

template <typename T>
bool goLeft(AVLNode<T> *&node) {
    if (hasLeft(node)) {
        node = node->left;

        return true;
    }

    return false;
}

template <typename T>
bool goRight(AVLNode<T> *&node) {
    if (hasRight(node)) {
        node = node->right;

        return true;
    }

    return false;
}

template <typename T>
int height(AVLNode<T> *node) {
    if (!node) {
        return 0;
    }

    return node->height;
}

template <typename T>
void updateHeight(AVLNode<T> *node) {
    node->height = std::max(height(node->left), height(node->right)) + 1;
}

template <typename T>
int getBalance(AVLNode<T> *node) {
    return height(node->left) - height(node->right);
}

template <typename T>
AVLNode<T>* rotateLeft(AVLNode<T> *node) {
    AVLNode<T> *node_parent = node->parent;
    AVLNode<T> *node_right = node->right;
    AVLNode<T> *right_left = node_right->left;

    if (!node_parent) {
        node_right->parent = nullptr;

    } else if (node == node->parent->left) {
        connectLeft(node_parent, node_right);

    } else {
        connectRight(node_parent, node_right);
    }

    connectLeft(node_right, node);
    connectRight(node, right_left);

    updateHeight(node);
    updateHeight(node_right);

    return node_right;
}

template <typename T>
AVLNode<T>* rotateRight(AVLNode<T> *node) {
    AVLNode<T> *node_parent = node->parent;
    AVLNode<T> *node_left = node->left;
    AVLNode<T> *left_right = node_left->right;

    if (!node_parent) {
        node_left->parent = nullptr;

    } else if (node == node->parent->left) {
        connectLeft(node_parent, node_left);

    } else {
        connectRight(node_parent, node_left);
    }

    connectRight(node_left, node);
    connectLeft(node, left_right);

    updateHeight(node);
    updateHeight(node_left);

    return node_left;
}

template <typename T>
AVLNode<T>* balance(AVLNode<T> *node) {
    updateHeight(node);

    int balance_factor = getBalance(node);

    if (balance_factor > 1) {
        if (getBalance(node->left) < 0) {
            rotateLeft(node->left);
        }

        rotateRight(node);
    }

    if (balance_factor < -1) {
        if (getBalance(node->right) > 0) {
            rotateRight(node->right);
        }

        rotateLeft(node);
    }

    if (!hasParent(node)) {
        return node;
    }

    return balance(node->parent);
}

template <typename T>
void deleteTree(AVLNode<T> *root) {
    if (!root) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

#endif
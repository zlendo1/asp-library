#ifndef AVLSTABLOMAPA_H
#define AVLSTABLOMAPA_H

#include "Mapa.cpp"
#include "AVLNode.cpp"

template <typename T_Kljuc, typename T>
class AVLStabloMapa: public Mapa <T_Kljuc, T> {
    using T_Par = std::pair <T_Kljuc, T> ;
    using Node = AVLNode < T_Par > ;

    T FAILSAFE;

    Node *korijen;

    int no_elem = 0;

    void copyTree(const Node *node);

    Node *insertNode(Node *node, T_Kljuc key);

public:
    ~AVLStabloMapa();

    AVLStabloMapa();

    AVLStabloMapa(const AVLStabloMapa &m);

    AVLStabloMapa &operator=(const AVLStabloMapa &m);

    T &operator[](const T_Kljuc &kljuc);
    const T &operator[](const T_Kljuc &kljuc) const;

    int brojElemenata() const;

    void obrisi();
    void obrisi(const T_Kljuc &kljuc);
};

template <typename T_Kljuc, typename T>
AVLStabloMapa<T_Kljuc, T>::~AVLStabloMapa() {
    deleteTree(korijen);
}

template <typename T_Kljuc, typename T>
AVLStabloMapa<T_Kljuc, T>::AVLStabloMapa(): FAILSAFE(), korijen(nullptr) {}

template <typename T_Kljuc, typename T>
AVLStabloMapa<T_Kljuc, T>::AVLStabloMapa(const AVLStabloMapa<T_Kljuc, T> &m) : AVLStabloMapa() {
    *this = m;
}

template <typename T_Kljuc, typename T>
AVLStabloMapa<T_Kljuc, T>& AVLStabloMapa<T_Kljuc, T>::operator = (const AVLStabloMapa<T_Kljuc, T> &m) {
    if (this == &m) {
        return *this;
    }

    deleteTree(korijen);

    no_elem = m.no_elem;

    copyTree(m.korijen);

    return *this;
}

template <typename T_Kljuc, typename T>
T& AVLStabloMapa<T_Kljuc, T>::operator[](const T_Kljuc &kljuc) {
    if (!no_elem) {
        ++no_elem;

        korijen = new Node(T_Par(kljuc, T()));

        return korijen->value.second;
    }

    Node *iter = korijen;

    while (true) {
        if (iter->value.first == kljuc) {
            break;
        }

        if (kljuc < iter->value.first) {
            if (!goLeft(iter)) {
                Node *new_child = new Node(T_Par(kljuc, T()));

                connectLeft(iter, new_child);

                ++no_elem;

                korijen = balance(iter); // Only addition

                iter = new_child;

                break;
            }

        } else {
            if (!goRight(iter)) {
                Node *new_child = new Node(T_Par(kljuc, T()));

                connectRight(iter, new_child);

                ++no_elem;

                korijen = balance(iter); // Only addition

                iter = new_child;

                break;
            }
        }
    }

    return iter->value.second;
}

template <typename T_Kljuc, typename T>
const T& AVLStabloMapa<T_Kljuc, T>::operator[](const T_Kljuc &kljuc) const {
    if (!no_elem) {
        return FAILSAFE;
    }

    Node *iter = korijen;

    while (true) {
        if (iter->value.first == kljuc) {
            return iter->value.second;
        }

        if (kljuc < iter->value.first) {
            if (!goLeft(iter)) {
                break;
            }

        } else {
            if (!goRight(iter)) {
                break;
            }
        }
    }

    return FAILSAFE;
}

template <typename T_Kljuc, typename T>
int AVLStabloMapa<T_Kljuc, T>::brojElemenata() const {
    return no_elem;
}

template <typename T_Kljuc, typename T>
void AVLStabloMapa<T_Kljuc, T> ::obrisi() {
    no_elem = 0;

    deleteTree(korijen);

    korijen = nullptr;
}

template <typename T_Kljuc, typename T>
void AVLStabloMapa<T_Kljuc, T>::obrisi(const T_Kljuc &kljuc) {
    if (!no_elem) {
        return;
    }

    Node *iter = korijen;
    Node *prethodnik = nullptr;

    while (true) {
        if (iter->value.first == kljuc) {
            prethodnik = iter->parent;

            break;
        }

        if (kljuc < iter->value.first) {
            if (!goLeft(iter)) {
                return;
            }

        } else {
            if (!goRight(iter)) {
                return;
            }
        }
    }

    if (!--no_elem) {
        deleteTree(korijen);

        korijen = nullptr;

        return;
    }

    Node *tmp = nullptr, *pp = nullptr, *rp = nullptr;
    Node *old_parent = nullptr;

    if (!hasLeft(iter)) {
        rp = iter->right;

    } else if (!hasRight(iter)) {
        rp = iter->left;

    } else {
        pp = iter;
        rp = iter->left;
        tmp = rp->right;

        while (tmp) {
            pp = rp;
            rp = tmp;

            tmp = rp->right;
        }

        old_parent = rp->parent;

        if (pp != iter) {
            connectRight(pp, rp->left);
            connectLeft(rp, iter->left);
        }

        connectRight(rp, iter->right);
    }

    if (!prethodnik) {
        rp->parent = nullptr;

        korijen = rp;

    } else if (iter == prethodnik->left) {
        connectLeft(prethodnik, rp);

    } else {
        connectRight(prethodnik, rp);
    }

    if (old_parent && old_parent != iter) {
        korijen = balance(old_parent);
    }

    delete iter;
}

template <typename T_Kljuc, typename T>
void AVLStabloMapa<T_Kljuc, T>::copyTree(const Node *node) {
    korijen = copyDownward(node);
}

#endif
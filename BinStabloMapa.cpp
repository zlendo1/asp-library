#ifndef BINSTABLOMAPA_H
#define BINSTABLOMAPA_H

#include "Mapa.cpp"
#include "BinaryNode.cpp"

template<typename T_Kljuc, typename T>
class BinStabloMapa : public Mapa<T_Kljuc, T> {
    using T_Par = std::pair<T_Kljuc, T>;
    using Node = BinaryNode<T_Par>;

    T FAILSAFE;

    std::shared_ptr<Node> cvorVrh;

    int no_elem = 0;

    void copyTree(const std::shared_ptr<Node> &node);

public:
    ~BinStabloMapa();

    BinStabloMapa();

    BinStabloMapa(const BinStabloMapa &m);

    BinStabloMapa& operator=(const BinStabloMapa &m);

    T& operator[](const T_Kljuc& kljuc);
    const T& operator[](const T_Kljuc& kljuc) const;

    int brojElemenata() const;

    void obrisi();
    void obrisi(const T_Kljuc& kljuc);
};

template<typename T_Kljuc, typename T>
BinStabloMapa<T_Kljuc, T>::~BinStabloMapa() {
    cvorVrh = nullptr;
}

template<typename T_Kljuc, typename T>
BinStabloMapa<T_Kljuc, T>::BinStabloMapa() : FAILSAFE(), cvorVrh(nullptr) {}

template<typename T_Kljuc, typename T>
BinStabloMapa<T_Kljuc, T>::BinStabloMapa(const BinStabloMapa<T_Kljuc, T> &m) : FAILSAFE() {
    *this = m;
}

template<typename T_Kljuc, typename T>
BinStabloMapa<T_Kljuc, T>& BinStabloMapa<T_Kljuc, T>::operator=(const BinStabloMapa<T_Kljuc, T> &m) {
    if (this == &m) {
        return *this;
    }

    no_elem = m.no_elem;
    copyTree(m.cvorVrh);

    return *this;
}

template<typename T_Kljuc, typename T>
T& BinStabloMapa<T_Kljuc, T>::operator[](const T_Kljuc& kljuc) {
    if (!no_elem) {
        ++no_elem;

        cvorVrh = std::make_shared<Node>(T_Par(kljuc, T()));

        return cvorVrh->value.second;
    }
    
    std::shared_ptr<Node> iter = cvorVrh;

    while (true) {
        if (iter->value.first == kljuc) {
            break;
        }

        if (kljuc < iter->value.first) {
            if (!goLeft(iter)) {
                std::shared_ptr<Node> new_child = std::make_shared<Node>(T_Par(kljuc, T()));

                connectLeft(iter, new_child);

                ++no_elem;

                iter = new_child;

                break;
            }

        } else {
            if (!goRight(iter)) {
                std::shared_ptr<Node> new_child = std::make_shared<Node>(T_Par(kljuc, T()));

                connectRight(iter, new_child);

                ++no_elem;

                iter = new_child;

                break;
            }

        }
    }

    return iter->value.second;
}

template<typename T_Kljuc, typename T>
const T& BinStabloMapa<T_Kljuc, T>::operator[](const T_Kljuc& kljuc) const {
    if (!no_elem) {
        return FAILSAFE;
    }
    
    std::shared_ptr<Node> iter = cvorVrh;

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

template<typename T_Kljuc, typename T>
int BinStabloMapa<T_Kljuc, T>::brojElemenata() const {
    return no_elem;
}

template<typename T_Kljuc, typename T>
void BinStabloMapa<T_Kljuc, T>::obrisi() {
    no_elem = 0;

    cvorVrh = nullptr;
}

template<typename T_Kljuc, typename T>
void BinStabloMapa<T_Kljuc, T>::obrisi(const T_Kljuc& kljuc) {
    if (!no_elem) {
        return;
    }
    
    std::shared_ptr<Node> iter = cvorVrh;
    std::shared_ptr<Node> prethodnik = nullptr;

    while (true) {
        if (iter->value.first == kljuc) {
            prethodnik = iter->parent.lock();

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
        cvorVrh = nullptr;

        return;
    }

    std::shared_ptr<Node> tmp = nullptr, pp = nullptr, rp = nullptr;

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

        if (pp != iter) {
            connectRight(pp, rp->left);
            connectLeft(rp, iter->left);
        }

        connectRight(rp, iter->right);
    }

    if (!prethodnik) {
        cvorVrh = rp;

    } else if (iter == prethodnik->left) {
        connectLeft(prethodnik, rp);

    } else {
        connectRight(prethodnik, rp);
        
    }
}

template<typename T_Kljuc, typename T>
void BinStabloMapa<T_Kljuc, T>::copyTree(const std::shared_ptr<Node> &node) {
    cvorVrh = copyDownward(node);
}

#endif
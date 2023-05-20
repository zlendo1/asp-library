#ifndef LISTMAPA_H
#define LISTMAPA_H

#include "DvostrukaLista.cpp"
#include <functional>

template <typename T_Key, typename T>
class ListMapa : public DvostrukaLista<std::pair<T_Key, T>> {
    using T_Pair = std::pair<T_Key, T>;
    
    T FAILSAFE = T();

public:
    const T& find(const T_Key& key) const;
    T& findOrInsert(const T_Key& key);
    void pop(const T_Key& key);
};

template <typename T_Key, typename T>
const T& ListMapa<T_Key, T>::find(const T_Key& key) const {
    if (!this->brojElemenata()) {
        return FAILSAFE;
    }
    
    auto *pair = &this->pocetak().trenutni();

    while (true) {
        if (pair->first == key) {
            return this->trenutni().second;
        }

        if (pair->first > key || !this->sljedeci()) {
            break;
        }
    }

    return FAILSAFE;
}

template <typename T_Key, typename T>
T& ListMapa<T_Key, T>::findOrInsert(const T_Key& key) {
    if (!this->brojElemenata()) {
        this->dodajIza(T_Pair(key, T()));
    }

    auto *pair = &this->pocetak().trenutni();

    while (true) {
        if (pair->first == key) {
            break;
        }

        if (pair->first > key) {
            this->dodajIspred(T_Pair(key, T()));

            this->prethodni();

            break;
        }

        if (!this->sljedeci()) {
            this->dodajIza(T_Pair(key, T()));

            this->sljedeci();

            break;
        }

        pair = &this->trenutni();
    }

    pair = &this->trenutni();

    return pair->second;
}

template <typename T_Key, typename T>
void ListMapa<T_Key, T>::pop(const T_Key& key) {
    if (!this->brojElemenata()) {
        return;
    }

    auto *pair = &this->pocetak().trenutni();

    while (true) {
        if (pair->first == key) {
            this->obrisi();

            break;
        }

        if (pair->first > key || !this->sljedeci()) {
            break;
        }

        pair = &this->trenutni();
    }
}

#endif
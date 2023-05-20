#ifndef HASHMAPA_H
#define HASHMAPA_H

#include "Mapa.cpp"
#include "HashNiz.cpp"
#include <stdexcept>

template <typename T_Key, typename T>
class HashMapa : public Mapa<T_Key, T> {
    using T_Hash = unsigned int (*)(T_Key, unsigned int);

    HashNiz<T_Key, T> niz;

    T FAILSAFE = T();

    void checkHashExistence() const;

public:
    ~HashMapa();

    HashMapa();

    HashMapa(const HashMapa &m);

    HashMapa& operator=(const HashMapa &m);

    void definisiHashFunkciju(T_Hash f);

    T& operator[](const T_Key& kljuc);
    const T& operator[](const T_Key& kljuc) const;

    int brojElemenata() const;

    void obrisi();
    void obrisi(const T_Key& kljuc);
};

template <typename T_Key, typename T>
void HashMapa<T_Key, T>::checkHashExistence() const {
    if (!niz.hasHasher()) {
        throw std::logic_error("Hash function not defined");
    }
}

template <typename T_Key, typename T>
HashMapa<T_Key, T>::~HashMapa() {}

template <typename T_Key, typename T>
HashMapa<T_Key, T>::HashMapa() {}

template <typename T_Key, typename T>
HashMapa<T_Key, T>::HashMapa(const HashMapa &m) : niz(m.niz) {}

template <typename T_Key, typename T>
void HashMapa<T_Key, T>::definisiHashFunkciju(T_Hash f) {
    niz.setHasher(f);
}

template <typename T_Key, typename T>
HashMapa<T_Key, T>& HashMapa<T_Key, T>::operator=(const HashMapa &m) {
    niz = m.niz;

    return *this;
}

template <typename T_Key, typename T>
T& HashMapa<T_Key, T>::operator[](const T_Key& kljuc) {
    checkHashExistence();

    T *ptr = niz.get(kljuc);

    if (ptr) {
        return *ptr;
    }

    return niz.create(kljuc);
}

template <typename T_Key, typename T>
const T& HashMapa<T_Key, T>::operator[](const T_Key& kljuc) const {
    checkHashExistence();

    T *ptr = niz.get(kljuc);

    if (ptr) {
        return *ptr;
    }

    return FAILSAFE;
}

template <typename T_Key, typename T>
int HashMapa<T_Key, T>::brojElemenata() const {
    return niz.size();
}

template <typename T_Key, typename T>
void HashMapa<T_Key, T>::obrisi() {
    niz.destroy();
}

template <typename T_Key, typename T>
void HashMapa<T_Key, T>::obrisi(const T_Key& kljuc) {
    checkHashExistence();

    niz.pop(kljuc);
}

#endif
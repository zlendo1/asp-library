#ifndef HASHMAPALAN_H
#define HASHMAPALAN_H

#include "Mapa.cpp"
#include "HashNizLan.cpp"
#include <utility>
#include <stdexcept>

template <typename T_Key, typename T>
class HashMapaLan : Mapa<T_Key, T> {
    using T_Hash = unsigned int (*)(T_Key, unsigned int);

    HashNizLan<T_Key, T> niz;

    void checkHashExistence() const;

public:
    ~HashMapaLan();

    HashMapaLan();

    HashMapaLan(const HashMapaLan &m);

    HashMapaLan& operator=(const HashMapaLan &m);

    void definisiHashFunkciju(T_Hash f);

    T& operator[](const T_Key& kljuc);
    const T& operator[](const T_Key& kljuc) const;

    int brojElemenata() const;

    void obrisi();
    void obrisi(const T_Key& kljuc);
};

template <typename T_Key, typename T>
void HashMapaLan<T_Key, T>::checkHashExistence() const {
    if (!niz.hasHasher()) {
        throw std::logic_error("Hash function not defined");
    }
}

template <typename T_Key, typename T>
HashMapaLan<T_Key, T>::~HashMapaLan() {}

template <typename T_Key, typename T>
HashMapaLan<T_Key, T>::HashMapaLan() {}

template <typename T_Key, typename T>
HashMapaLan<T_Key, T>::HashMapaLan(const HashMapaLan &m) : niz(m.niz) {}

template <typename T_Key, typename T>
void HashMapaLan<T_Key, T>::definisiHashFunkciju(T_Hash f) {
    niz.setHasher(f);
}

template <typename T_Key, typename T>
HashMapaLan<T_Key, T>& HashMapaLan<T_Key, T>::operator=(const HashMapaLan &m) {
    niz = m.niz;

    return *this;
}

template <typename T_Key, typename T>
T& HashMapaLan<T_Key, T>::operator[](const T_Key& kljuc) {
    checkHashExistence();

    return niz[kljuc];
}

template <typename T_Key, typename T>
const T& HashMapaLan<T_Key, T>::operator[](const T_Key& kljuc) const {
    checkHashExistence();

    return niz[kljuc];
}

template <typename T_Key, typename T>
int HashMapaLan<T_Key, T>::brojElemenata() const {
    return niz.size();
}

template <typename T_Key, typename T>
void HashMapaLan<T_Key, T>::obrisi() {
    niz.destroy();
}

template <typename T_Key, typename T>
void HashMapaLan<T_Key, T>::obrisi(const T_Key& kljuc) {
    checkHashExistence();

    niz.pop(kljuc);
}

#endif
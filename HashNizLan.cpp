#ifndef HASHNIZLAN_H
#define HASHNIZLAN_H

#include "ListMapa.cpp"
#include <utility>
#include <stdexcept>

template <typename T_Key, typename T>
class HashNizLan {
    using T_Hash = unsigned int (*)(T_Key, unsigned int);
    using T_Pair = std::pair<T_Key, T>;
    using MyList = ListMapa<T_Key, T>;

    MyList *niz = nullptr;

    T_Hash hash = nullptr;

    unsigned int no_elem = 0;
    unsigned int kapacitet = 0;

public:
    ~HashNizLan();

    HashNizLan();
    HashNizLan(const HashNizLan &h);
    HashNizLan(HashNizLan &&h);

    HashNizLan& operator=(const HashNizLan &h);
    HashNizLan& operator=(HashNizLan &&h);

    unsigned int size() const;
    unsigned int capacity() const;

    void resize(const unsigned int new_capacity);
    void destroy();

    void setHasher(T_Hash f);
    bool hasHasher() const;

    const T& operator[](const T_Key &key) const;
    T& operator[](const T_Key &key);
    void pop(const T_Key &key);
};

template <typename T_Key, typename T>
HashNizLan<T_Key, T>::HashNizLan() : kapacitet(1000) {
    niz = new MyList[kapacitet] {};
}

template <typename T_Key, typename T>
HashNizLan<T_Key, T>::~HashNizLan() {
    delete [] niz;
}

template <typename T_Key, typename T>
HashNizLan<T_Key, T>::HashNizLan(const HashNizLan &h) : kapacitet(h.kapacitet), no_elem(h.no_elem), hash(h.hash) {
    niz = new MyList[kapacitet] {};
    
    for (unsigned i = 0; i < h.capacity(); ++i) {
        niz[i] = h.niz[i];
    }
}

template <typename T_Key, typename T>
HashNizLan<T_Key, T>::HashNizLan(HashNizLan &&h) {
    std::swap(niz, h.niz);
    std::swap(no_elem, h.no_elem);
    std::swap(kapacitet, h.kapacitet);
    std::swap(hash, h.hash);
}

template <typename T_Key, typename T>
HashNizLan<T_Key, T>& HashNizLan<T_Key, T>::operator=(const HashNizLan<T_Key, T> &h) {
    if (this == &h) {
        return *this;
    }
    
    delete [] niz;
    niz = new MyList[kapacitet] {};

    kapacitet = h.kapacitet;
    no_elem = h.no_elem;

    for (unsigned i = 0; i < h.capacity(); ++i) {
        niz[i] = h.niz[i];
    }

    return *this;
}

template <typename T_Key, typename T>
HashNizLan<T_Key, T>& HashNizLan<T_Key, T>::operator=(HashNizLan<T_Key, T> &&h) {
    if (this == &h) {
        return *this;
    }

    std::swap(niz, h.niz);
    std::swap(no_elem, h.no_elem);
    std::swap(kapacitet, h.kapacitet);
    std::swap(hash, h.hash);

    return *this;
}

template <typename T_Key, typename T>
void HashNizLan<T_Key, T>::destroy() {
    no_elem = 0;

    for (unsigned int i = 0; i < capacity(); ++i) {
        niz[i].obrisiListu();
    }
}

template <typename T_Key, typename T>
unsigned int HashNizLan<T_Key, T>::size() const {
    return no_elem;
}

template <typename T_Key, typename T>
unsigned int HashNizLan<T_Key, T>::capacity() const {
    return kapacitet;
}

template <typename T_Key, typename T>
void HashNizLan<T_Key, T>::resize(const unsigned int new_capacity) {
    if (capacity() >= new_capacity) {
        return;
    }

    MyList *new_niz = new MyList[kapacitet] {};

    for (unsigned i = 0; i < capacity(); ++i) {
        new_niz[i] = std::move(niz[i]);
    }

    kapacitet = new_capacity;

    delete [] niz;
    niz = new_niz;
}

template <typename T_Key, typename T>
const T& HashNizLan<T_Key, T>::operator[](const T_Key &key) const {
    unsigned index = hash(key, capacity());

    return niz[index].find(key);
}

template <typename T_Key, typename T>
T& HashNizLan<T_Key, T>::operator[](const T_Key &key) {
    unsigned index = hash(key, capacity());
    
    unsigned old_size = niz[index].brojElemenata();

    T& element = niz[index].findOrInsert(key);

    no_elem += niz[index].brojElemenata() - old_size;

    return element;
}

template <typename T_Key, typename T>
void HashNizLan<T_Key, T>::pop(const T_Key &key) {
    unsigned index = hash(key, capacity());

    unsigned old_size = niz[index].brojElemenata();

    niz[index].pop(key);

    no_elem += niz[index].brojElemenata() - old_size;
}

template <typename T_Key, typename T>
void HashNizLan<T_Key, T>::setHasher(T_Hash f) {
    hash = f;
}

template <typename T_Key, typename T>
bool HashNizLan<T_Key, T>::hasHasher() const {
    return hash;
}

#endif

#ifndef MAPA_H
#define MAPA_H

template<typename T_Kljuc, typename T>
class Mapa {
public:
    virtual ~Mapa() = default;

    Mapa() = default;

    Mapa(const Mapa &m) = delete;

    virtual Mapa& operator=(const Mapa &m) = delete;

    virtual T& operator[](const T_Kljuc& kljuc) = 0;
    virtual const T& operator[](const T_Kljuc& kljuc) const = 0;

    virtual int brojElemenata() const = 0;

    virtual void obrisi() = 0;
    virtual void obrisi(const T_Kljuc& kljuc) = 0;
};

#endif
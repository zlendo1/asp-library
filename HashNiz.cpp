#ifndef HASHNIZ_H
#define HASHNIZ_H

#include <utility>
#include <stdexcept>

template <typename T_Key, typename T>
class HashNiz {
    using T_Hash = unsigned int (*)(T_Key, unsigned int);
    using T_Pair = std::pair<T_Key, T>;

    T_Pair **niz = nullptr;

    T_Hash hash = nullptr;

    unsigned int no_elem = 0;
    unsigned int kapacitet = 0;

    unsigned int shuffle(unsigned int hashValue, int i) const;

public:
    ~HashNiz();

    HashNiz();
    HashNiz(const HashNiz &h);
    HashNiz(HashNiz &&h);

    HashNiz& operator=(const HashNiz &h);
    HashNiz& operator=(HashNiz &&h);

    unsigned int size() const;
    unsigned int capacity() const;

    void resize(const unsigned int new_capacity);
    void destroy();

    void setHasher(T_Hash f);
    bool hasHasher() const;

    T* get(const T_Key &key) const;
    T& create(const T_Key &key);
    void pop(const T_Key &key);
};

template <typename T_Key, typename T>
HashNiz<T_Key, T>::HashNiz() : kapacitet(1000) {
    niz = new T_Pair*[kapacitet] { nullptr };
}

template <typename T_Key, typename T>
HashNiz<T_Key, T>::~HashNiz() {
    destroy();

    delete [] niz;
}

template <typename T_Key, typename T>
HashNiz<T_Key, T>::HashNiz(const HashNiz &h) : kapacitet(h.kapacitet), no_elem(h.no_elem), hash(h.hash) {
    niz = new T_Pair*[kapacitet] { nullptr };
    
    for (unsigned i = 0; i < h.capacity(); ++i) {
        if (!h.niz[i]) {
            continue;
        }

        niz[i] = new T_Pair(*h.niz[i]);
    }
}

template <typename T_Key, typename T>
HashNiz<T_Key, T>::HashNiz(HashNiz &&h) {
    std::swap(niz, h.niz);
    std::swap(no_elem, h.no_elem);
    std::swap(kapacitet, h.kapacitet);
    std::swap(hash, h.hash);
}

template <typename T_Key, typename T>
HashNiz<T_Key, T>& HashNiz<T_Key, T>::operator=(const HashNiz<T_Key, T> &h) {
    if (this == &h) {
        return *this;
    }

    destroy();
    
    delete [] niz;
    niz = new T_Pair*[h.kapacitet] { nullptr };

    kapacitet = h.kapacitet;
    no_elem = h.no_elem;

    for (unsigned i = 0; i < h.capacity(); ++i) {
        if (!h.niz[i]) {
            continue;
        }

        niz[i] = new T_Pair(*h.niz[i]);
    }

    return *this;
}

template <typename T_Key, typename T>
HashNiz<T_Key, T>& HashNiz<T_Key, T>::operator=(HashNiz<T_Key, T> &&h) {
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
void HashNiz<T_Key, T>::destroy() {
    no_elem = 0;

    for (unsigned int i = 0; i < capacity(); ++i) {
        delete niz[i];

        niz[i] = nullptr;
    }
}

template <typename T_Key, typename T>
unsigned int HashNiz<T_Key, T>::size() const {
    return no_elem;
}

template <typename T_Key, typename T>
unsigned int HashNiz<T_Key, T>::capacity() const {
    return kapacitet;
}

template <typename T_Key, typename T>
void HashNiz<T_Key, T>::resize(const unsigned int new_capacity) {
    if (capacity() >= new_capacity) {
        return;
    }

    T_Pair** new_niz = new T_Pair*[new_capacity] { nullptr };

    for (unsigned i = 0; i < capacity(); ++i) {
        new_niz[i] = niz[i];
    }

    kapacitet = new_capacity;

    delete [] niz;
    niz = new_niz;
}

template <typename T_Key, typename T>
unsigned int HashNiz<T_Key, T>::shuffle(unsigned int hashValue, int i) const {
    return (hashValue + i) % capacity();
}

template <typename T_Key, typename T>
T* HashNiz<T_Key, T>::get(const T_Key &key) const {
    unsigned index = hash(key, capacity());

    for (unsigned i = 0; i < capacity(); index = shuffle(index, i++)) {
        if (!niz[index]) {
            continue;
        }
        
        if (niz[index]->first == key) {
            return &niz[index]->second;
        }
    }

    return nullptr;
}

template <typename T_Key, typename T>
T& HashNiz<T_Key, T>::create(const T_Key &key) {
    unsigned index = hash(key, capacity());

    for (unsigned i = 0; i < capacity(); index = shuffle(index, i++)) {
        if (!niz[index]) {
            niz[index] = new T_Pair(key, T());

            ++no_elem;

            return niz[index]->second;
        }
    }

    resize(2 * capacity());

    return create(key);
}

template <typename T_Key, typename T>
void HashNiz<T_Key, T>::pop(const T_Key &key) {
    unsigned index = hash(key, capacity());

    for (unsigned i = 0; i < capacity(); index = shuffle(index, i++)) {
        if (!niz[index]) {
            continue;
        }
        
        if (niz[index]->first == key) {
            delete niz[index];

            niz[index] = nullptr;

            --no_elem;

            return;
        }
    }
}

template <typename T_Key, typename T>
void HashNiz<T_Key, T>::setHasher(T_Hash f) {
    hash = f;
}

template <typename T_Key, typename T>
bool HashNiz<T_Key, T>::hasHasher() const {
    return hash;
}

#endif
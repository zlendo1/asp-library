#ifndef NIZLISTA_H
#define NIZLISTA_H

#include "Lista.cpp"

template<typename T>
class NizLista;

template<typename T>
class NizListaIterator;

template<typename T>
class NizLista : public Lista<T> {
private:
    T **niz = nullptr;
    int broj_elem = 0;
    int kapacitet = 0;
    mutable int trenutni_el = 0;

    void obrisiListu() {
        for (int i = 0; i < broj_elem; ++i) {
            delete niz[i];
        }

        delete [] niz;

        niz = nullptr;
        broj_elem = 0;
        kapacitet = 0;
        trenutni_el = 0;
    }

public:
    friend NizListaIterator<T>;

    NizLista() {
        kapacitet = 128;
        niz = new T *[kapacitet]{};
    }

    ~NizLista() { obrisiListu(); }

    NizLista &operator=(const NizLista &l) {
        if (&l == this) {
            return *this;
        }

        obrisiListu();

        broj_elem = l.broj_elem;
        kapacitet = l.kapacitet;
        trenutni_el = l.trenutni_el;

        niz = new T *[kapacitet]{};

        for (int i = 0; i < broj_elem; i++) {
            niz[i] = new T(*l.niz[i]);
        }

        return *this;
    }

    NizLista(const NizLista &l) { *this = l; }

    int brojElemenata() const { return broj_elem; }

    T &trenutni() {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        return *niz[trenutni_el];
    }

    const T &trenutni() const {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        return *niz[trenutni_el];
    }

    bool prethodni() const {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        if (trenutni_el == 0) {
            return false;
        }

        trenutni_el--;

        return true;
    }

    bool sljedeci() const {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        if (trenutni_el == broj_elem - 1) {
            return false;
        }

        trenutni_el++;

        return true;
    }

    Lista<T> &pocetak() {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        trenutni_el = 0;

        return *this;
    }

    const Lista<T> &pocetak() const {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        trenutni_el = 0;

        return *this;
    }

    Lista<T> &kraj() {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        trenutni_el = broj_elem - 1;

        return *this;
    }

    const Lista<T> &kraj() const {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        trenutni_el = broj_elem - 1;

        return *this;
    }

    Lista<T> &obrisi() {
        Lista<T>::praznaListaIzuzetak(!broj_elem);

        delete niz[trenutni_el];

        for (int i = trenutni_el; i < broj_elem - 1; i++) {
            niz[i] = niz[i + 1];
        }

        niz[broj_elem - 1] = nullptr;

        if (trenutni_el == broj_elem - 1) {
            trenutni_el--;

            if (trenutni_el < 0) {
                trenutni_el = 0;
            }
        }

        broj_elem--;

        return *this;
    }

    Lista<T> &dodajIza(const T &el) {
        if (!broj_elem) {
            niz[0] = new T(el);
            broj_elem++;

            return *this;
        }

        if (kapacitet > broj_elem) {
            for (int i = broj_elem - 1; i > trenutni_el; i--) {
                niz[i + 1] = niz[i];
            }

            niz[trenutni_el + 1] = new T(el);

            broj_elem++;

            return *this;
        }

        auto **novi_niz = new T *[kapacitet *= 2]{};

        for (int i = 0; i <= trenutni_el; i++) {
            novi_niz[i] = niz[i];
        }

        novi_niz[trenutni_el + 1] = new T(el);

        for (int i = trenutni_el + 1; i < broj_elem; i++) {
            novi_niz[i + 1] = niz[i];
        }

        broj_elem++;

        delete [] niz;
        niz = novi_niz;

        return *this;
    }

    Lista<T> &dodajIspred(const T &el) {
        if (!broj_elem) {
            niz[0] = new T(el);
            broj_elem++;

            return *this;
        }

        if (kapacitet > broj_elem) {
            for (int i = broj_elem - 1; i >= trenutni_el; i--) {
                niz[i + 1] = niz[i];
            }

            niz[trenutni_el] = new T(el);

            broj_elem++;
            trenutni_el++;

            return *this;
        }

        auto **novi_niz = new T *[kapacitet *= 2]{};

        for (int i = 0; i < trenutni_el; i++) {
            novi_niz[i] = niz[i];
        }

        novi_niz[trenutni_el] = new T(el);

        for (int i = trenutni_el; i < broj_elem; i++) {
            novi_niz[i + 1] = niz[i];
        }

        broj_elem++;
        trenutni_el++;

        delete[] niz;
        niz = novi_niz;

        return *this;
    }

    T &operator[](int i) {
        if (i < 0 || i >= broj_elem) {
            throw std::range_error("Ilegalan indeks");
        }

        return *niz[i];
    }

    const T &operator[](int i) const {
        if (i < 0 || i >= broj_elem) {
            throw std::range_error("Ilegalan indeks");
        }

        return *niz[i];
    }

    NizListaIterator<T> begin() {
        return NizListaIterator<T>(niz);
    }

    NizListaIterator<T> end() {
        return NizListaIterator<T>(niz + broj_elem);
    }

    const NizListaIterator<T> begin() const {
        return NizListaIterator<T>(niz);
    }

    const NizListaIterator<T> end() const {
        return NizListaIterator<T>(niz + broj_elem);
    }

    NizLista(int broj_el, const T& allocator = T()) : kapacitet(128) {
        while (broj_el > kapacitet) {
            kapacitet *= 2;
        }

        niz = new T*[kapacitet] {};

        for (int i = 0; i < broj_el; ++i) {
            niz[i] = new T(allocator);
        }

        NizLista::broj_el = broj_el;
    }

    NizLista<T>& push_back(const T& el) {
        int old_trenutni = trenutni_el;

        kraj();
        dodajIza(el);

        trenutni_el = old_trenutni;

        return *this;
    }

    NizLista<T>& resize(int new_size, const T& allocator = T()) {
        if (new_size <= broj_elem) {
            throw std::range_error("Stari broj elemenata veci nego novi");
        }

        if (new_size > kapacitet) {
            while (kapacitet < new_size) {
                kapacitet *= 2;
            }

            T** old_niz = niz;
            niz = new T*[kapacitet] {};

            for (int i = 0; i < broj_elem; ++i) {
                niz[i] = old_niz[i];
            }

            delete [] old_niz;
        }

        for (int i = broj_elem; i < new_size; ++i) {
            niz[i] = new T(allocator);
        }

        broj_elem = new_size;

        return *this;
    }
};

template<typename T>
class NizListaIterator {
    mutable T **pok;

    NizListaIterator(T **pok) : pok(pok) {}
    
public:
    friend NizLista<T>;

    T& operator*() {
        return **pok;
    }

    const T& operator*() const {
        return **pok;
    }

    T* operator->() {
        return *pok;
    }

    const T* operator->() const {
        return *pok;
    }

    NizListaIterator<T>& operator++() {
        ++pok;

        return *this;
    }

    const NizListaIterator<T>& operator++() const {
        ++pok;

        return *this;
    }

    NizListaIterator<T> operator++(int) {
        NizListaIterator<T> new_one = *this;

        ++(*this);

        return new_one;
    }

    const NizListaIterator<T> operator++(int) const {
        const NizListaIterator<T> new_one = *this;

        ++(*this);

        return new_one;
    }

    bool operator==(const NizListaIterator<T> &iter) const {
        return pok == iter.pok;
    }

    bool operator!=(const NizListaIterator<T> &iter) const {
        return pok != iter.pok;
    }
};

#endif
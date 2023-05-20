#ifndef LISTA_H
#define LISTA_H

#include <iostream>
#include <stdexcept>

template<typename Tip>
class Lista {
protected:
    static void praznaListaIzuzetak(bool uslov) {
        if (uslov) {
            throw std::range_error("Lista je prazna");
        }
    }

public:
    Lista() = default;

    virtual ~Lista() = default;

    Lista(const Lista &l) = delete;

    virtual Lista &operator=(const Lista &l) = delete;

    virtual int brojElemenata() const = 0;

    virtual Tip &trenutni() = 0;

    virtual const Tip &trenutni() const = 0;

    virtual bool prethodni() const = 0;

    virtual bool sljedeci() const = 0;

    virtual Lista &pocetak() = 0;

    virtual const Lista &pocetak() const = 0;

    virtual Lista &kraj() = 0;

    virtual const Lista &kraj() const = 0;

    virtual Lista &obrisi() = 0;

    virtual Lista &dodajIspred(const Tip &el) = 0;

    virtual Lista &dodajIza(const Tip &el) = 0;

    virtual Tip &operator[](int i) = 0;

    virtual const Tip &operator[](int i) const = 0;
};

#endif
#ifndef DVOSTRUKALISTA_H
#define DVOSTRUKALISTA_H

#include "Lista.cpp"
#include "DvostrukiCvor.cpp"

template<typename Tip>
class DvostrukaLista : public Lista<Tip> {
private:
    DvostrukiCvor<Tip> *pocetni_cvor = nullptr;
    DvostrukiCvor<Tip> *krajnji_cvor = nullptr;
    mutable DvostrukiCvor<Tip> *trenutni_cvor = nullptr;

    int broj_elem = 0;

    int pozicijaCvora(DvostrukiCvor<Tip> *cvor) const; // vraca 0 za nijedno, 1 za pocetak, 2 za kraj i 3 za oboje

    void promjenaCvoraNaKrajeve(DvostrukiCvor<Tip> *cvor, int nova_pozicija);

public:
    DvostrukaLista();

    ~DvostrukaLista();

    DvostrukaLista(const DvostrukaLista &l);

    DvostrukaLista(DvostrukaLista &&l);

    DvostrukaLista &operator=(const DvostrukaLista &l);

    DvostrukaLista &operator=(DvostrukaLista &&l);

    int brojElemenata() const;

    Tip &trenutni();

    const Tip &trenutni() const;

    bool prethodni() const;

    bool sljedeci() const;

    DvostrukaLista &pocetak();

    const DvostrukaLista &pocetak() const;

    DvostrukaLista &kraj();

    const DvostrukaLista &kraj() const;

    DvostrukaLista &obrisi();

    DvostrukaLista &dodajIspred(const Tip &el);

    DvostrukaLista &dodajIza(const Tip &el);

    Tip &operator[](int i);

    const Tip &operator[](int i) const;

    void obrisiListu();
};

template<typename Tip>
void DvostrukaLista<Tip>::obrisiListu() {
    for (DvostrukiCvor<Tip> *x = pocetni_cvor; x; x = oslobodiDvostrukiCvor(x));

    pocetni_cvor = nullptr;
    krajnji_cvor = nullptr;
    trenutni_cvor = nullptr;

    broj_elem = 0;
}

template<typename Tip>
DvostrukaLista<Tip>::DvostrukaLista() = default;

template<typename Tip>
DvostrukaLista<Tip>::~DvostrukaLista() {
    obrisiListu();
}

template<typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &l) {
    *this = l;
}

template<typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(DvostrukaLista &&l) {
    *this = std::move(l);
}

template<typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator=(const DvostrukaLista &l) {
    if (this == &l || !l.brojElemenata()) {
        return *this;
    }

    obrisiListu();

    broj_elem = l.broj_elem;
    pocetni_cvor = stvoriDvostrukiCvor(l.pocetni_cvor->element);
    trenutni_cvor = pocetni_cvor;

    auto iteracijski_cvor = pocetni_cvor;

    for (auto x = l.pocetni_cvor->sljedbenik; x; x = x = x->sljedbenik) {
        spojiDvostrukeCvorove(iteracijski_cvor, stvoriDvostrukiCvor(x->element));

        iteracijski_cvor = iteracijski_cvor->sljedbenik;

        if (x == l.trenutni_cvor) {
            trenutni_cvor = iteracijski_cvor;
        }
    }

    krajnji_cvor = iteracijski_cvor;

    return *this;
}

template<typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator=(DvostrukaLista &&l) {
    if (this == &l) {
        return *this;
    }

    obrisiListu();

    std::swap(broj_elem, l.broj_elem);
    std::swap(trenutni_cvor, l.trenutni_cvor);
    std::swap(krajnji_cvor, l.krajnji_cvor);
    std::swap(trenutni_cvor, l.trenutni_cvor);

    return *this;
}

template<typename Tip>
int DvostrukaLista<Tip>::brojElemenata() const {
    return broj_elem;
}

template<typename Tip>
Tip &DvostrukaLista<Tip>::trenutni() {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    return trenutni_cvor->element;
}

template<typename Tip>
const Tip &DvostrukaLista<Tip>::trenutni() const {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    return trenutni_cvor->element;
}

template<typename Tip>
bool DvostrukaLista<Tip>::prethodni() const {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    if (!trenutni_cvor->prethodnik) {
        return false;
    }

    trenutni_cvor = trenutni_cvor->prethodnik;

    return true;
}

template<typename Tip>
bool DvostrukaLista<Tip>::sljedeci() const {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    if (!trenutni_cvor->sljedbenik) {
        return false;
    }

    trenutni_cvor = trenutni_cvor->sljedbenik;

    return true;
}

template<typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::pocetak() {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    trenutni_cvor = pocetni_cvor;

    return *this;
}

template<typename Tip>
const DvostrukaLista<Tip> &DvostrukaLista<Tip>::pocetak() const {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    trenutni_cvor = pocetni_cvor;

    return *this;
}

template<typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::kraj() {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    trenutni_cvor = krajnji_cvor;

    return *this;
}

template<typename Tip>
const DvostrukaLista<Tip> &DvostrukaLista<Tip>::kraj() const {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    trenutni_cvor = krajnji_cvor;

    return *this;
}

template<typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::obrisi() {
    Lista<Tip>::praznaListaIzuzetak(!broj_elem);

    broj_elem--;

    int pozicija = pozicijaCvora(trenutni_cvor);

    trenutni_cvor = oslobodiDvostrukiCvor(trenutni_cvor);

    promjenaCvoraNaKrajeve(trenutni_cvor, pozicija);

    return *this;
}

template<typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::dodajIspred(const Tip &el) {
    if (!(broj_elem++)) {
        pocetni_cvor = stvoriDvostrukiCvor(el);
        krajnji_cvor = pocetni_cvor;
        trenutni_cvor = krajnji_cvor;

        return *this;
    }

    int pozicija = pozicijaCvora(trenutni_cvor);

    stvoriDvostrukiCvor(el)->spoji(trenutni_cvor->prethodnik, trenutni_cvor);

    if (pozicija == 3 || pozicija == 1) {
        pocetni_cvor = trenutni_cvor->prethodnik;
    }

    return *this;
}

template<typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::dodajIza(const Tip &el) {
    if (!(broj_elem++)) {
        pocetni_cvor = stvoriDvostrukiCvor(el);
        krajnji_cvor = pocetni_cvor;
        trenutni_cvor = krajnji_cvor;

        return *this;
    }

    int pozicija = pozicijaCvora(trenutni_cvor);

    stvoriDvostrukiCvor(el)->spoji(trenutni_cvor, trenutni_cvor->sljedbenik);

    if (pozicija == 3 || pozicija == 2) {
        krajnji_cvor = trenutni_cvor->sljedbenik;
    }

    return *this;
}

template<typename Tip>
Tip &DvostrukaLista<Tip>::operator[](int i) {
    if (i < 0 || i >= broj_elem) {
        throw std::range_error("Ilegalan indeks");
    }

    auto iteracijski_cvor = pocetni_cvor;

    for (int index = 0; index < i; index++) {
        iteracijski_cvor = iteracijski_cvor->sljedbenik;
    }

    return iteracijski_cvor->element;
}

template<typename Tip>
const Tip &DvostrukaLista<Tip>::operator[](int i) const {
    if (i < 0 || i >= broj_elem) {
        throw std::range_error("Ilegalan indeks");
    }

    auto iteracijski_cvor = pocetni_cvor;

    for (int index = 0; index < i; index++) {
        iteracijski_cvor = iteracijski_cvor->sljedbenik;
    }

    return iteracijski_cvor->element;
}

template<typename Tip>
int DvostrukaLista<Tip>::pozicijaCvora(DvostrukiCvor<Tip> *cvor) const {
    if (trenutni_cvor == pocetni_cvor && trenutni_cvor == krajnji_cvor) {
        return 3;
    }

    if (trenutni_cvor == pocetni_cvor) {
        return 1;
    }

    if (trenutni_cvor == krajnji_cvor) {
        return 2;
    }

    return 0;
}

template<typename Tip>
void DvostrukaLista<Tip>::promjenaCvoraNaKrajeve(DvostrukiCvor<Tip> *cvor, int nova_pozicija) {
    if (nova_pozicija == 3 || nova_pozicija == 1) {
        pocetni_cvor = cvor;
    }

    if (nova_pozicija == 3 || nova_pozicija == 2) {
        krajnji_cvor = cvor;
    }
}

#endif
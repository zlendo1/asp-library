#pragma once

#include "Lista.cpp"
#include "JednostrukiCvor.cpp"

template<typename Tip>
class JednostrukaLista : public Lista<Tip> {
private:
    JednostrukiCvor<Tip> *pocetni_cvor = nullptr;
    JednostrukiCvor<Tip> *krajnji_cvor = nullptr;
    mutable JednostrukiCvor<Tip> *trenutni_cvor = nullptr;

    int broj_elem = 0;

    void obrisiListu() {
        for (auto x = pocetni_cvor; x; x = oslobodiCvor(x));

        pocetni_cvor = nullptr;
        krajnji_cvor = nullptr;
        trenutni_cvor = nullptr;

        broj_elem = 0;
    }

public:
    JednostrukaLista() {
        pocetni_cvor = stvoriCvor<Tip>();
        trenutni_cvor = pocetni_cvor;
        krajnji_cvor = pocetni_cvor;
    }

    ~JednostrukaLista() { obrisiListu(); }

    JednostrukaLista &operator=(const JednostrukaLista &l) {
        if (&l == this) {
            return *this;
        }

        obrisiListu();
        broj_elem = l.broj_elem;

        pocetni_cvor = stvoriCvor(l.pocetni_cvor->element);
        trenutni_cvor = pocetni_cvor;
        krajnji_cvor = pocetni_cvor;

        auto iteracijski_cvor = pocetni_cvor;

        for (auto x = l.pocetni_cvor->sljedbenik; x; x = x->sljedbenik) {
            iteracijski_cvor->sljedbenik = stvoriCvor(x->element);

            if (x == l.trenutni_cvor) {
                trenutni_cvor = iteracijski_cvor;
            }

            krajnji_cvor = iteracijski_cvor;
            iteracijski_cvor = iteracijski_cvor->sljedbenik;
        }

        return *this;
    }

    JednostrukaLista &operator=(JednostrukaLista &&l) {
        if (&l == this) {
            return *this;
        }

        obrisiListu();

        broj_elem = l.broj_elem;
        pocetni_cvor = l.pocetni_cvor;
        krajnji_cvor = l.krajnji_cvor;
        trenutni_cvor = l.krajnji_cvor;

        l.pocetni_cvor = nullptr;

        l.obrisiListu();

        return *this;
    }

    JednostrukaLista(const JednostrukaLista &l) { *this = l; }

    JednostrukaLista(JednostrukaLista &&l) { *this = std::move(l); }

    int brojElemenata() const { return broj_elem; }

    Tip &trenutni() {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        return trenutni_cvor->sljedbenik->element;
    }

    const Tip &trenutni() const {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        return trenutni_cvor->sljedbenik->element;
    }

    bool prethodni() const {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        if (trenutni_cvor == pocetni_cvor) {
            return false;
        }

        auto novi_pok = pocetni_cvor;
        for (; novi_pok->sljedbenik != trenutni_cvor;
               novi_pok = novi_pok->sljedbenik) {
        }

        trenutni_cvor = novi_pok;

        return true;
    }

    bool sljedeci() const {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        if (!trenutni_cvor->sljedbenik->sljedbenik) {
            return false;
        }

        trenutni_cvor = trenutni_cvor->sljedbenik;

        return true;
    }

    JednostrukaLista &pocetak() {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        trenutni_cvor = pocetni_cvor;

        return *this;
    }

    const JednostrukaLista &pocetak() const {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        trenutni_cvor = pocetni_cvor;

        return *this;
    }

    JednostrukaLista &kraj() {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        trenutni_cvor = krajnji_cvor;

        return *this;
    }

    const JednostrukaLista &kraj() const {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        trenutni_cvor = krajnji_cvor;

        return *this;
    }

    JednostrukaLista &obrisi() {
        Lista<Tip>::praznaListaIzuzetak(!broj_elem);

        broj_elem--;

        if (!broj_elem) {
            pocetni_cvor->sljedbenik = oslobodiCvor(pocetni_cvor->sljedbenik);
            trenutni_cvor = pocetni_cvor;
            krajnji_cvor = pocetni_cvor;

            return *this;
        }

        if (trenutni_cvor == krajnji_cvor) {
            trenutni_cvor = trenutni_cvor->nadjiPrethodnika(pocetni_cvor);
            krajnji_cvor = trenutni_cvor;
        }

        trenutni_cvor->sljedbenik = oslobodiCvor(trenutni_cvor->sljedbenik);

        return *this;
    }

    JednostrukaLista &dodajIza(const Tip &el) {
        if (!(broj_elem++)) {
            pocetni_cvor->sljedbenik = stvoriCvor(el);

            return *this;
        }

        if (trenutni_cvor == krajnji_cvor) {
            krajnji_cvor = trenutni_cvor->sljedbenik;
            krajnji_cvor->sljedbenik = stvoriCvor(el);

            return *this;
        }

        auto prethodni_sljedbenik = trenutni_cvor->sljedbenik->sljedbenik;
        trenutni_cvor->sljedbenik->sljedbenik = stvoriCvor(el);
        trenutni_cvor->sljedbenik->sljedbenik->sljedbenik = prethodni_sljedbenik;

        if (trenutni_cvor->sljedbenik == krajnji_cvor) {
            krajnji_cvor = krajnji_cvor->sljedbenik;
        }

        return *this;
    }

    JednostrukaLista &dodajIspred(const Tip &el) {
        if (!(broj_elem++)) {
            pocetni_cvor->sljedbenik = stvoriCvor(el);

            return *this;
        }

        bool je_krajnji = trenutni_cvor == krajnji_cvor;

        auto tren_spasen = trenutni_cvor->sljedbenik;
        trenutni_cvor->sljedbenik = stvoriCvor(el);
        trenutni_cvor->sljedbenik->sljedbenik = tren_spasen;
        trenutni_cvor = trenutni_cvor->sljedbenik;

        if (je_krajnji) {
            krajnji_cvor = trenutni_cvor;
        }

        return *this;
    }

    Tip &operator[](int i) {
        if (i < 0 || i >= broj_elem) {
            throw std::range_error("Ilegalan indeks");
        }

        auto trazeni = pocetni_cvor;

        for (int j = 0; j < i; j++) {
            trazeni = trazeni->sljedbenik;
        }

        return trazeni->sljedbenik->element;
    }

    const Tip &operator[](int i) const {
        if (i < 0 || i >= broj_elem) {
            throw std::range_error("Ilegalan indeks");
        }

        auto trazeni = pocetni_cvor;

        for (int j = 0; j < i; j++) {
            trazeni = trazeni->sljedbenik;
        }

        return trazeni->sljedbenik->element;
    }
};
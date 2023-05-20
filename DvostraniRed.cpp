#pragma once

#include "DvostrukaLista.cpp"

template<typename Tip>
class DvostraniRed {
private:
    DvostrukaLista<Tip> elementi;

public:
    ~DvostraniRed();
    DvostraniRed();

    DvostraniRed(const DvostraniRed &l);
    DvostraniRed(DvostraniRed &&l);

    DvostraniRed &operator=(const DvostraniRed &l);
    DvostraniRed &operator=(DvostraniRed &&l);

    DvostraniRed &brisi();

    DvostraniRed &staviNaVrh(const Tip &el);
    Tip skiniSaVrha();
    Tip &vrh();
    const Tip &vrh() const;

    DvostraniRed &staviNaCelo(const Tip &el);
    Tip skiniSaCela();
    Tip &celo();
    const Tip &celo() const;

    int brojElemenata() const;
};

template<typename Tip>
DvostraniRed<Tip>::~DvostraniRed() = default;

template<typename Tip>
DvostraniRed<Tip>::DvostraniRed() : elementi() {}

template<typename Tip>
DvostraniRed<Tip>::DvostraniRed(const DvostraniRed &l) : elementi(l.elementi) {}

template<typename Tip>
DvostraniRed<Tip>::DvostraniRed(DvostraniRed &&l) : elementi(std::move(l.elementi)) {}

template<typename Tip>
DvostraniRed<Tip> &DvostraniRed<Tip>::operator=(const DvostraniRed &l) {
    elementi = l.elementi;
    return *this;
}

template<typename Tip>
DvostraniRed<Tip> &DvostraniRed<Tip>::operator=(DvostraniRed &&l) {
    elementi = std::move(l.elementi);
    return *this;
}

template<typename Tip>
DvostraniRed<Tip> &DvostraniRed<Tip>::brisi() {
    try {
        while (true) {
            elementi.obrisi();
        }
    } catch (std::range_error &e) {}

    return *this;
}

template<typename Tip>
DvostraniRed<Tip> &DvostraniRed<Tip>::staviNaVrh(const Tip &el) {
    if (!elementi.brojElemenata()) {
        elementi.dodajIza(el);
    } else {
        elementi.kraj().dodajIza(el);
    }

    return *this;
}

template<typename Tip>
Tip DvostraniRed<Tip>::skiniSaVrha() {
    try {
        Tip el = elementi.kraj().trenutni();
        elementi.obrisi();

        return el;
    } catch (std::range_error &e) {
        throw std::range_error("DvostraniRed je prazan");
    }
}

template<typename Tip>
Tip &DvostraniRed<Tip>::vrh() {
    try {
        return elementi.kraj().trenutni();
    } catch (std::range_error &e) {
        throw std::range_error("DvostraniRed je prazan");
    }
}

template<typename Tip>
const Tip &DvostraniRed<Tip>::vrh() const {
    try {
        return elementi.kraj().trenutni();
    } catch (std::range_error &e) {
        throw std::range_error("DvostraniRed je prazan");
    }
}

template<typename Tip>
DvostraniRed<Tip> &DvostraniRed<Tip>::staviNaCelo(const Tip &el) {
    if (!elementi.brojElemenata()) {
        elementi.dodajIspred(el);
    } else {
        elementi.pocetak().dodajIspred(el);
    }

    return *this;
}

template<typename Tip>
Tip DvostraniRed<Tip>::skiniSaCela() {
    try {
        Tip el = elementi.pocetak().trenutni();
        elementi.obrisi();

        return el;
    } catch (std::range_error &e) {
        throw std::range_error("DvostraniRed je prazan");
    }
}

template<typename Tip>
Tip &DvostraniRed<Tip>::celo() {
    try {
        return elementi.pocetak().trenutni();
    } catch (std::range_error &e) {
        throw std::range_error("DvostraniRed je prazan");
    }
}

template<typename Tip>
const Tip &DvostraniRed<Tip>::celo() const {
    try {
        return elementi.pocetak().trenutni();
    } catch (std::range_error &e) {
        throw std::range_error("DvostraniRed je prazan");
    }
}

template<typename Tip>
int DvostraniRed<Tip>::brojElemenata() const {
    return elementi.brojElemenata();
}
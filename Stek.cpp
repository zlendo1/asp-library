#pragma once

#include "JednostrukaLista.cpp"

template<typename Tip>
class Stek {
private:
    JednostrukaLista<Tip> elementi;

public:
    Stek();

    Stek(const Stek &l);

    Stek(Stek &&l);

    ~Stek();

    Stek &operator=(const Stek &l);

    Stek &operator=(Stek &&l);

    Stek &brisi();

    Stek &stavi(const Tip &el);

    Tip skini();

    Tip &vrh();

    const Tip &vrh() const;

    int brojElemenata() const;
};

template<typename Tip>
Stek<Tip>::Stek() : elementi() {}

template<typename Tip>
Stek<Tip>::Stek(const Stek &l) : elementi(l.elementi) {}

template<typename Tip>
Stek<Tip>::Stek(Stek &&l) : elementi(std::move(l.elementi)) {}

template<typename Tip>
Stek<Tip>::~Stek() = default;

template<typename Tip>
Stek<Tip> &Stek<Tip>::operator=(const Stek &l) {
    elementi = l.elementi;

    return *this;
}

template<typename Tip>
Stek<Tip> &Stek<Tip>::operator=(Stek &&l) {
    elementi = std::move(l.elementi);

    return *this;
}

template<typename Tip>
Stek<Tip> &Stek<Tip>::brisi() {
    try {
        while (true) {
            elementi.obrisi();
        }
    } catch (std::range_error &e) {}

    return *this;
}

template<typename Tip>
Stek<Tip> &Stek<Tip>::stavi(const Tip &el) {
    elementi.dodajIspred(el);
    elementi.pocetak();

    return *this;
}

template<typename Tip>
Tip Stek<Tip>::skini() {
    try {
        Tip e = elementi.trenutni();
        elementi.obrisi();
        
        return e;
    } catch (std::range_error &e) {
        throw std::range_error("Stek je prazan");
    }
}

template<typename Tip>
Tip &Stek<Tip>::vrh() {
    try {
        return elementi.trenutni();
    } catch (std::range_error &error) {
        throw std::range_error("Stek je prazan");
    }
}

template<typename Tip>
const Tip &Stek<Tip>::vrh() const {
    try {
        return elementi.trenutni();
    } catch (std::range_error &error) {
        throw std::range_error("Stek je prazan");
    }
}

template<typename Tip>
int Stek<Tip>::brojElemenata() const {
    return elementi.brojElemenata();
}
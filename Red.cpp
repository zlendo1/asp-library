#pragma once

#include "JednostrukaLista.cpp"

template<typename Tip>
class Red {
private:
    JednostrukaLista<Tip> elementi;

public:
    ~Red();

    Red();

    Red(const Red &l);

    Red(Red &&l);

    Red &operator=(const Red &l);

    Red &operator=(Red &&l);

    Red &brisi();

    Red &stavi(const Tip &el);

    Tip skini();

    Tip &celo();

    const Tip &celo() const;

    int brojElemenata() const;
};

template<typename Tip>
Red<Tip>::~Red() = default;

template<typename Tip>
Red<Tip>::Red() : elementi() {}

template<typename Tip>
Red<Tip>::Red(const Red &l) : elementi(l.elementi) {}

template<typename Tip>
Red<Tip>::Red(Red &&l) : elementi(std::move(l.elementi)) {}

template<typename Tip>
Red<Tip> &Red<Tip>::operator=(const Red &l) {
    elementi = l.elementi;
    return *this;
}

template<typename Tip>
Red<Tip> &Red<Tip>::operator=(Red &&l) {
    elementi = std::move(l.elementi);
    return *this;
}

template<typename Tip>
Red<Tip> &Red<Tip>::brisi() {
    try {
        while (true) {
            elementi.obrisi();
        }
    } catch (std::range_error &e) {}

    return *this;
}

template<typename Tip>
Red<Tip> &Red<Tip>::stavi(const Tip &el) {
    if (!elementi.brojElemenata()) {
        elementi.dodajIza(el);
    } else {
        elementi.kraj().dodajIza(el);
    }

    return *this;
}

template<typename Tip>
Tip Red<Tip>::skini() {
    try {
        Tip el = elementi.pocetak().trenutni();
        elementi.obrisi();

        return el;
    } catch (std::range_error &e) {
        throw std::range_error("Red je prazan");
    }
}

template<typename Tip>
Tip &Red<Tip>::celo() {
    try {
        return elementi.pocetak().trenutni();
    } catch (std::range_error &e) {
        throw std::range_error("Red je prazan");
    }
}

template<typename Tip>
const Tip &Red<Tip>::celo() const {
    try {
        return elementi.pocetak().trenutni();
    } catch (std::range_error &e) {
        throw std::range_error("Red je prazan");
    }
}

template<typename Tip>
int Red<Tip>::brojElemenata() const {
    return elementi.brojElemenata();
}
#ifndef NIZMAPA_H
#define NIZMAPA_H

#include "Mapa.cpp"
#include "NizLista.cpp"

template<typename T_Kljuc, typename T>
class NizMapa : public Mapa<T_Kljuc, T> {
    using T_Par = std::pair<T_Kljuc, T>;
    
    NizLista<T_Par> lista;

    T FAILSAFE;

public:
    ~NizMapa();

    NizMapa();

    NizMapa(const NizMapa &m);

    NizMapa& operator=(const NizMapa &m);

    T& operator[](const T_Kljuc& kljuc);
    const T& operator[](const T_Kljuc& kljuc) const;

    int brojElemenata() const;

    void obrisi();
    void obrisi(const T_Kljuc& kljuc);
};

template<typename T_Kljuc, typename T>
NizMapa<T_Kljuc, T>::~NizMapa() {}

template<typename T_Kljuc, typename T>
NizMapa<T_Kljuc, T>::NizMapa() : lista(), FAILSAFE() {}

template<typename T_Kljuc, typename T>
NizMapa<T_Kljuc, T>::NizMapa(const NizMapa<T_Kljuc, T> &m) : lista(m.lista), FAILSAFE() {}

template<typename T_Kljuc, typename T>
NizMapa<T_Kljuc, T>& NizMapa<T_Kljuc, T>::operator=(const NizMapa<T_Kljuc, T> &m) {
    if (this == &m) {
        return *this;
    }

    lista = m.lista;

    return *this;
}

template<typename T_Kljuc, typename T>
T& NizMapa<T_Kljuc, T>::operator[](const T_Kljuc& kljuc) {
    for (int i = 0; i < lista.brojElemenata(); ++i) {
        if (kljuc == lista[i].first) {
            return lista[i].second;
        }
    }

    if (lista.brojElemenata()) {
        lista.kraj();
    }

    lista.dodajIza(T_Par(kljuc, T()));
    lista.pocetak();

    return lista[lista.brojElemenata() - 1].second;
}

template<typename T_Kljuc, typename T>
const T& NizMapa<T_Kljuc, T>::operator[](const T_Kljuc& kljuc) const {
    for (int i = 0; i < lista.brojElemenata(); ++i) {
        if (kljuc == lista[i].first) {
            return lista[i].second;
        }
    }

    return FAILSAFE;
}

template<typename T_Kljuc, typename T>
int NizMapa<T_Kljuc, T>::brojElemenata() const {
    return lista.brojElemenata();
}

template<typename T_Kljuc, typename T>
void NizMapa<T_Kljuc, T>::obrisi() {
    if (!lista.brojElemenata()) {
        return;
    }
    
    lista.pocetak();
    
    try {
        while (true) {
            lista.obrisi();
        }

    } catch (const std::range_error &e) {}
}

template<typename T_Kljuc, typename T>
void NizMapa<T_Kljuc, T>::obrisi(const T_Kljuc& kljuc) {
    lista.pocetak();
    
    do {
        if (kljuc == lista.trenutni().first) {
            lista.obrisi();

            break;
        }
    } while (lista.sljedeci());

    lista.pocetak();
}

#endif
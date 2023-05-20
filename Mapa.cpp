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
#pragma once

template<typename Tip>
class JednostrukiCvor {
public:
    Tip element;
    JednostrukiCvor *sljedbenik = nullptr;

    JednostrukiCvor *nadjiPrethodnika(JednostrukiCvor *pocetni_cvor) {
        if (pocetni_cvor == this) {
            return pocetni_cvor;
        }

        auto tr_pok = pocetni_cvor;

        for (; tr_pok != nullptr && tr_pok->sljedbenik != this;
               tr_pok = tr_pok->sljedbenik) {
        }

        return tr_pok;
    }
};

template<typename Tip>
JednostrukiCvor<Tip> *stvoriCvor(const Tip &element = {}) {
    auto *x = new JednostrukiCvor<Tip>;
    x->element = element;

    return x;
}

template<typename Tip>
JednostrukiCvor<Tip> *oslobodiCvor(JednostrukiCvor<Tip> *cvor) {
    if (!cvor) {
        return cvor;
    }

    auto x = cvor->sljedbenik;
    delete cvor;

    return x;
}
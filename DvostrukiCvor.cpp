#ifndef DVOSTRUKICVOR_H
#define DVOSTRUKICVOR_H

template<typename Tip>
class DvostrukiCvor {
public:
    Tip element;
    DvostrukiCvor *prethodnik = nullptr;
    DvostrukiCvor *sljedbenik = nullptr;

    DvostrukiCvor *spoji(DvostrukiCvor *prethodnik, DvostrukiCvor *sljedbenik);
};

template<typename Tip>
void spojiDvostrukeCvorove(DvostrukiCvor<Tip> *prethodnik, DvostrukiCvor<Tip> *sljedbenik) {
    if (prethodnik) {
        prethodnik->sljedbenik = sljedbenik;
    }

    if (sljedbenik) {
        sljedbenik->prethodnik = prethodnik;
    }
}

template<typename Tip>
DvostrukiCvor<Tip> *
DvostrukiCvor<Tip>::spoji(DvostrukiCvor<Tip> *prethodnik, DvostrukiCvor<Tip> *sljedbenik) {
    spojiDvostrukeCvorove(prethodnik, this);
    spojiDvostrukeCvorove(this, sljedbenik);

    return this;
}

template<typename Tip>
DvostrukiCvor<Tip> *stvoriDvostrukiCvor(const Tip &element = {}) {
    auto *x = new DvostrukiCvor<Tip>;
    x->element = element;

    return x;
}

template<typename Tip>
DvostrukiCvor<Tip> *oslobodiDvostrukiCvor(DvostrukiCvor<Tip> *cvor) {
    if (!cvor) {
        return cvor;
    }

    DvostrukiCvor<Tip> *x = cvor->sljedbenik ? cvor->sljedbenik : cvor->prethodnik;

    spojiDvostrukeCvorove(cvor->prethodnik, cvor->sljedbenik);

    cvor->prethodnik = nullptr;
    cvor->sljedbenik = nullptr;

    delete cvor;

    return x;
}

#endif
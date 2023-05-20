#ifndef USMJERENIGRAF_H
#define USMJERENIGRAF_H

#include <vector>
#include <queue>

template <typename T>
class Cvor;

template <typename T>
class Grana;

template <typename T>
class GranaIterator;

template <typename T>
class UsmjereniGraf {
public:
    UsmjereniGraf() {}
    virtual ~UsmjereniGraf() {}

    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int new_size) = 0;

    virtual void dodajGranu(int root, int destination, float weight = 0) = 0;
    virtual void obrisiGranu(int root, int destination) = 0;

    virtual void postaviTezinuGrane(int root, int destination, float weight = 0) = 0;
    virtual float dajTezinuGrane(int root, int destination) const = 0;
    virtual bool postojiGrana(int root, int destination) const = 0;

    virtual void postaviOznakuCvora(int node, const T& mark) = 0;
    virtual T dajOznakuCvora(int node) const = 0;

    virtual void postaviOznakuGrane(int root, int destination, const T& mark) = 0;
    virtual T dajOznakuGrane(int root, int destination) const = 0;

    Cvor<T> dajCvor(int node);
    Grana<T> dajGranu(int root, int destination);

    virtual GranaIterator<T> dajGranePocetak() = 0;
    virtual GranaIterator<T> dajGraneKraj() = 0;

    virtual GranaIterator<T> dajSljedecuGranu(int root, int destination) = 0;
};

template <typename T>
Cvor<T> UsmjereniGraf<T>::dajCvor(int node) {
    return Cvor<T>(this, node);
}

template <typename T>
Grana<T> UsmjereniGraf<T>::dajGranu(int root, int destination) {
    return Grana<T>(this, root, destination);
}

template <typename T>
class Cvor {
    UsmjereniGraf<T> *graph_ptr;

    int position;

public:
    Cvor(UsmjereniGraf<T> *graph_ptr, int position) : graph_ptr(graph_ptr), position(position) {}

    void postaviOznaku(const T& mark) {
        graph_ptr->postaviOznakuCvora(position, mark);
    }

    T dajOznaku() const {
        return graph_ptr->dajOznakuCvora(position);
    }

    int  dajRedniBroj() const {
        return position;
    }

    bool operator==(const Cvor<T> &other) const {
        return graph_ptr == other.graph_ptr && position == other.position;
    }
};

template <typename T>
class Grana {
    UsmjereniGraf<T> *graph_ptr;

    int root, destination;

public:
    Grana(UsmjereniGraf<T> *graph_ptr, int root, int destination) : graph_ptr(graph_ptr), root(root), destination(destination) {}

    void postaviOznaku(const T& mark) {
        graph_ptr->postaviOznakuGrane(root, destination, mark);
    }

    T dajOznaku() const {
        return graph_ptr->dajOznakuGrane(root, destination);
    }

    void postaviTezinu(const float weight) {
        graph_ptr->postaviTezinuGrane(root, destination, weight);
    }

    float dajTezinu() const {
        return graph_ptr->dajTezinuGrane(root, destination);
    }

    Cvor<T> dajPolazniCvor() const {
        return graph_ptr->dajCvor(root);
    }

    Cvor<T> dajDolazniCvor() const {
        return graph_ptr->dajCvor(destination);
    }

    bool operator==(const Grana<T> &other) const {
        return root == other.root && destination == other.destination;
    }
};

template <typename T>
class GranaIterator  {
    UsmjereniGraf<T> *graph_ptr;

    int root, destination;

public:
    GranaIterator(UsmjereniGraf<T> *graph_ptr, int root, int destination) : graph_ptr(graph_ptr), root(root), destination(destination) {}

    Grana<T> operator*() {
        return graph_ptr->dajGranu(root, destination);
    }

    bool operator==(const GranaIterator &other) const {
        return graph_ptr == other.graph_ptr && root == other.root && destination == other.destination;
    }

    bool operator!=(const GranaIterator &other) const {
        return !(*this == other);
    }

    GranaIterator& operator++() {
        return *this = graph_ptr->dajSljedecuGranu(root, destination);
    }

    GranaIterator operator++(int) {
        GranaIterator<T> old = *this;

        ++(*this);

        return old;
    }
};

template <typename T>
void dfs(UsmjereniGraf<T> *graph_ptr, std::vector<Cvor<T>> &obilazak, Cvor<T> cvor) {
    cvor.postaviOznaku(1);

    obilazak.push_back(cvor);

    for (auto iter = graph_ptr->dajGranePocetak(); iter != graph_ptr->dajGraneKraj(); ++iter) {
        Cvor<T> destination = (*iter).dajDolazniCvor();

        if (destination.dajOznaku() != 1 && (*iter).dajPolazniCvor().dajRedniBroj() == cvor.dajRedniBroj()) {
            dfs(graph_ptr, obilazak, destination);
        }
    }
}

template <typename T>
void bfs(UsmjereniGraf<T> *graph_ptr, std::vector<Cvor<T>> &obilazak, Cvor<T> cvor) {
    cvor.postaviOznaku(1);

    obilazak.push_back(cvor);

    std::queue<Cvor<T>> queue;

    queue.push(cvor);

    while (!queue.empty()) {
        Cvor<T> temp = queue.front();

        queue.pop();

        for (auto iter = graph_ptr->dajGranePocetak(); iter != graph_ptr->dajGraneKraj(); ++iter) {
            Cvor<T> root = (*iter).dajPolazniCvor();
            Cvor<T> destination = (*iter).dajDolazniCvor();

            if ((*iter).dajDolazniCvor().dajOznaku() != 1 && temp.dajRedniBroj() != root.dajOznaku()) {
                destination.postaviOznaku(1);

                obilazak.push_back(destination);

                queue.push(destination);
            }
        }
    }
}

#endif
#ifndef LISTAGRAF_H
#define LISTAGRAF_H

#include "UsmjereniGraf.cpp"
#include <list>
#include <stdexcept>

template <typename T>
class ListaGraf : public UsmjereniGraf<T> {
    struct Vertex {
        T mark;

        float weight;

        int destination;

        Vertex(int destination, float weight, const T &mark = T()) : destination(destination), weight(weight), mark(mark) {}

        bool operator<(const Vertex &other) const {
            return destination < other.destination;
        }
    };

    struct Node {
        T mark;

        std::list<Vertex> vertices;

        Node(const T &mark = T()) : mark(mark) {}
    };

    std::vector<Node> nodes;

    void CheckNodeExistence(int i) const;
    void CheckVertexExistence(int i, int j) const;

public:
    ListaGraf(int broj_cvorova);
    ~ListaGraf();

    ListaGraf<T>& operator=(const ListaGraf<T> &other);

    int dajBrojCvorova() const;
    void postaviBrojCvorova(int new_size);

    void dodajGranu(int root, int destination, float weight);
    void obrisiGranu(int root, int destination);

    void postaviTezinuGrane(int root, int destination, float weight = 0);
    float dajTezinuGrane(int root, int destination) const;
    bool postojiGrana(int root, int destination) const;

    void postaviOznakuCvora(int node, const T& mark);
    T dajOznakuCvora(int node) const;

    void postaviOznakuGrane(int root, int destination, const T& mark);
    T dajOznakuGrane(int root, int destination) const;

    GranaIterator<T> dajGranePocetak();
    GranaIterator<T> dajGraneKraj();

    GranaIterator<T> dajSljedecuGranu(int root, int destination);
};

template <typename T>
void ListaGraf<T>::CheckNodeExistence(int i) const {
    if (i >= nodes.size()) {
        throw std::range_error("Node non-existent");
    }
}

template <typename T>
void ListaGraf<T>::CheckVertexExistence(int i, int j) const {
    if (i != -1 || j != -1) {
        return;
    }

    if (i >= nodes.size()) {
        throw std::range_error("Vertex non-existent");
    }

    const auto &vertices = nodes[i].vertices;

    for (auto iter = vertices.begin(); iter != vertices.end(); ++iter) {
        if (iter->destination == j) {
            return;
        }
    }

    throw std::range_error("Vertex non-existent");
}

template <typename T>
ListaGraf<T>::ListaGraf(int broj_cvorova) : nodes(broj_cvorova) {}

template <typename T>
ListaGraf<T>::~ListaGraf() {}

template <typename T>
ListaGraf<T>& ListaGraf<T>::operator=(const ListaGraf<T> &other) {
    nodes = other.nodes;
}

template <typename T>
int ListaGraf<T>::dajBrojCvorova() const {
    return nodes.size();
}

template <typename T>
void ListaGraf<T>::postaviBrojCvorova(int new_size) {
    if (new_size <= nodes.size()) {
        throw std::range_error("Current number of nodes larger than new number of nodes");
    }

    nodes.resize(new_size);
}

template <typename T>
void ListaGraf<T>::dodajGranu(int root, int destination, float weight) {
    auto &vertices = nodes[root].vertices;

    auto iter = vertices.begin();

    for (; iter != vertices.end(); ++iter) {
        if (iter->destination > destination) {
            break;
        }
    }

    vertices.emplace(iter, destination, weight);
}

template <typename T>
void ListaGraf<T>::obrisiGranu(int root, int destination) {
    CheckNodeExistence(root);

    auto &vertices = nodes[root].vertices;

    for (auto iter = vertices.begin(); iter != vertices.end(); ++iter) {
        if (iter->destination == destination) {
            vertices.erase(iter);

            return;
        }
    }
}

template <typename T>
void ListaGraf<T>::postaviTezinuGrane(int root, int destination, float weight) {
    CheckVertexExistence(root, destination);

    auto &vertices = nodes[root].vertices;

    for (Vertex &x : vertices) {
        if (x.destination == destination) {
            x.weight = weight;
        }
    }
}

template <typename T>
float ListaGraf<T>::dajTezinuGrane(int root, int destination) const {
    CheckVertexExistence(root, destination);

    for (const Vertex &x : nodes[root].vertices) {
        if (x.destination == destination) {
            return x.weight;
        }
    }

    return 0;
}

template <typename T>
bool ListaGraf<T>::postojiGrana(int root, int destination) const {
    if (root >= nodes.size() || destination >= nodes.size()) {
        return false;
    }

    for (const Vertex &x : nodes[root].vertices) {
        if (x.destination == destination) {
            return true;
        }
    }

    return false;
}

template <typename T>
void ListaGraf<T>::postaviOznakuCvora(int node, const T& mark) {
    CheckNodeExistence(node);

    nodes[node].mark = mark;
}

template <typename T>
T ListaGraf<T>::dajOznakuCvora(int node) const {
    CheckNodeExistence(node);

    return nodes[node].mark;
}

template <typename T>
void ListaGraf<T>::postaviOznakuGrane(int root, int destination, const T& mark) {
    CheckVertexExistence(root, destination);

    for (Vertex &x : nodes[root].vertices) {
        if (x.destination == destination) {
            x.mark = mark;
        }
    }
}

template <typename T>
T ListaGraf<T>::dajOznakuGrane(int root, int destination) const {
    CheckVertexExistence(root, destination);

    for (const Vertex &x : nodes[root].vertices) {
        if (x.destination == destination) {
            return x.mark;
        }
    }

    return T();
}

template <typename T>
GranaIterator<T> ListaGraf<T>::dajGranePocetak() {
    GranaIterator<T> pocetni(this, 0, -1);

    return ++pocetni;
}

template <typename T>
GranaIterator<T> ListaGraf<T>::dajGraneKraj() {
    return {this, -1, -1};
}

template <typename T>
GranaIterator<T> ListaGraf<T>::dajSljedecuGranu(int root, int destination) {
    CheckVertexExistence(root, destination);

    for (int i = root; i < dajBrojCvorova(); ++i) {
        for (auto iter = nodes[i].vertices.begin(); iter != nodes[i].vertices.end(); ++iter) {
            if (i == root && iter->destination <= destination) {
                continue;
            }

            return GranaIterator<T>(this, i, iter->destination);
        }
    }

    return GranaIterator<T>(this, -1, -1);
}

#endif
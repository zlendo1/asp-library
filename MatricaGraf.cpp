#ifndef MATRICAGRAF_H
#define MATRICAGRAF_H

#include "UsmjereniGraf.cpp"
#include <vector>
#include <stdexcept>

template <typename T>
class MatricaGraf : public UsmjereniGraf<T> {
    struct Vertex {
        T mark;

        float weight;

        Vertex(float weight, const T &mark = T()) : weight(weight), mark(mark) {}
    };

    std::vector<T> nodes;
    std::vector<std::vector<Vertex*>> matrix;

    void destroy();
    void deep_copy(const MatricaGraf &other);

    void CheckNodeExistence(int i) const;
    void CheckVertexExistence(int i, int j) const;

public:
    MatricaGraf(int broj_cvorova);
    ~MatricaGraf();

    MatricaGraf<T>& operator=(const MatricaGraf<T> &other);

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
void MatricaGraf<T>::CheckNodeExistence(int i) const {
    if (i >= nodes.size()) {
        throw std::range_error("Node non-existent");
    }
}

template <typename T>
void MatricaGraf<T>::CheckVertexExistence(int i, int j) const {
    if (i != -1 && j != -1 && !matrix.at(i).at(j)) {
        throw std::range_error("Vertex non-existent");
    }
}

template <typename T>
void MatricaGraf<T>::destroy() {
    for (auto &vector : matrix) {
        for (auto &vertex_ptr : vector) {
            delete vertex_ptr;

            vertex_ptr = nullptr;
        }
    }
}

template <typename T>
void MatricaGraf<T>::deep_copy(const MatricaGraf<T> &other) {
    const int new_size(other.nodes.size());
    
    nodes.resize(new_size);
    matrix.resize(new_size);

    for (int i = 0; i < new_size; ++i) {
        matrix[i].resize(new_size, nullptr);

        for (int j = 0; j < new_size; ++j) {
            Vertex *ptr = other.matrix[i][j];
            
            if (!ptr) {
                continue;
            }

            matrix[i][j] = new Vertex(ptr->weight, ptr->mark);
        }
    }
}

template <typename T>
MatricaGraf<T>::MatricaGraf(int broj_cvorova) :
    nodes(broj_cvorova),
    matrix(broj_cvorova, std::vector<Vertex*>(broj_cvorova, nullptr)) {}

template <typename T>
MatricaGraf<T>::~MatricaGraf() {
    destroy();
}

template <typename T>
int MatricaGraf<T>::dajBrojCvorova() const {
    return nodes.size();
}

template <typename T>
void MatricaGraf<T>::postaviBrojCvorova(int new_size) {
    if (new_size <= dajBrojCvorova()) {
        throw std::range_error("Current number of nodes larger than new number of nodes");
    }
    
    nodes.resize(new_size);
    matrix.resize(new_size);

    for (auto &vector : matrix) {
        vector.resize(new_size);
    }
}

template <typename T>
void MatricaGraf<T>::dodajGranu(int root, int destination, float weight) {
    matrix[root][destination] = new Vertex(weight);
}

template <typename T>
void MatricaGraf<T>::obrisiGranu(int root, int destination) {
    delete matrix[root][destination];

    matrix[root][destination] = nullptr;
}

template <typename T>
void MatricaGraf<T>::postaviTezinuGrane(int root, int destination, float weight) {
    CheckVertexExistence(root, destination);

    matrix[root][destination]->weight = weight;
}

template <typename T>
float MatricaGraf<T>::dajTezinuGrane(int root, int destination) const {
    CheckVertexExistence(root, destination);

    return matrix[root][destination]->weight;
}

template <typename T>
bool MatricaGraf<T>::postojiGrana(int root, int destination) const {
    return matrix.at(root).at(destination);
}

template <typename T>
void MatricaGraf<T>::postaviOznakuCvora(int node, const T& mark) {
    CheckNodeExistence(node);

    nodes[node] = mark;
}

template <typename T>
T MatricaGraf<T>::dajOznakuCvora(int node) const {
    CheckNodeExistence(node);

    return nodes[node];
}

template <typename T>
void MatricaGraf<T>::postaviOznakuGrane(int root, int destination, const T& mark) {
    CheckVertexExistence(root, destination);

    matrix[root][destination]->mark = mark;
}

template <typename T>
T MatricaGraf<T>::dajOznakuGrane(int root, int destination) const {
    CheckVertexExistence(root, destination);

    return matrix[root][destination]->mark;
}

template <typename T>
GranaIterator<T> MatricaGraf<T>::dajGranePocetak() {
    GranaIterator<T> pocetni(this, 0, -1);

    return ++pocetni;
}

template <typename T>
GranaIterator<T> MatricaGraf<T>::dajGraneKraj() {
    return {this, -1, -1};
}

template <typename T>
GranaIterator<T> MatricaGraf<T>::dajSljedecuGranu(int root, int destination) {
    CheckVertexExistence(root, destination);

    for (int i = root; i < dajBrojCvorova(); ++i) {
        for (int j = 0; j < dajBrojCvorova(); ++j) {
            if ((i == root && j <= destination) || !matrix[i][j]) {
                continue;
            }

            return GranaIterator<T>(this, i, j);
        }
    }

    return GranaIterator<T>(this, -1, -1);
}

#endif
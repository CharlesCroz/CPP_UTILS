//
// Created by charles on 28/04/22.
// Matrix class, row-major
//

#ifndef CPP_UTILS_MATRIX_H
#define CPP_UTILS_MATRIX_H

#include <cstdlib> // size_t

template<typename T>
class Matrix {
public:
    Matrix() = default;
    Matrix(const size_t &width, const size_t &height);
    Matrix(const Matrix<T> &other);
    Matrix(Matrix<T> &&other);
    Matrix(const size_t &width, const size_t &height, const T *valTab);
    Matrix(const size_t &width, const size_t &height, const T val);
    Matrix<T> &operator=(const Matrix<T> &other);
    Matrix<T> &operator=(Matrix<T> &&other);
    ~Matrix();

    const T &at(const size_t &x, const size_t &y) const;
    T &at(const size_t &x, const size_t &y);

    const T &at(const size_t &i) const;
    T &at(const size_t &i);

    size_t get_width() const;
    size_t get_height() const;
    size_t get_surface() const;

    T *get_data();
    const T *get_data() const;

    class Iterator {
    public:
        Iterator(Matrix<T> *mat, const size_t &val);
        Iterator(const Iterator &other) = default;
        Iterator(Iterator &&other) = default;
        Iterator &operator=(const Iterator &other) = default;
        Iterator &operator=(Iterator &&other) = default;
        ~Iterator() = default;

        Iterator &operator++();
        Iterator operator++(int);
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        T &operator*();
        T *operator->();

    private:
        Matrix<T> *mat;
        size_t val = {};
    };

    class Const_Iterator {
    public:
        Const_Iterator(const Matrix<T> *mat, const size_t &val);
        Const_Iterator(const Const_Iterator &other) = default;
        Const_Iterator(Const_Iterator &&other) = default;
        Const_Iterator &operator=(const Const_Iterator &other) = default;
        Const_Iterator &operator=(Const_Iterator &&other) = default;
        ~Const_Iterator() = default;

        Const_Iterator &operator++();
        Const_Iterator operator++(int);
        bool operator==(const Const_Iterator &other) const;
        bool operator!=(const Const_Iterator &other) const;
        const T &operator*();
        const T *operator->();

    private:
        const Matrix<T> *mat;
        size_t val = {};
    };

    Iterator begin();
    Const_Iterator begin() const;
    Iterator end();
    Const_Iterator end() const;

private:
    size_t width = 0;
    size_t height = 0;
    size_t surface = 0;
    T *data = nullptr;
};

// Functions definitions
template<typename T>
Matrix<T>::Matrix(const size_t &width, const size_t &height) : width(width), height(height), surface(height * width),
                                                               data(new T[width * height]) {}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> &other) : width(other.width), height(other.height), surface(other.surface),
                                            data(new T[other.width * other.height]) {
#pragma omp parallel for default(none) shared(other, surface)
    for (size_t i = 0; i < surface; ++i) {
        data[i] = other.data[i];
    }
}

template<typename T>
Matrix<T>::Matrix(Matrix<T> &&other) : width(other.width), height(other.height), surface(other.surface),
                                       data(other.data) {
    other.data = nullptr;
}

template<typename T>
Matrix<T>::Matrix(const size_t &width, const size_t &height, const T *valTab) : width(width), height(height),
                                                                                surface(height * width),
                                                                                data(new T[width * height]) {
#pragma omp parallel for default(none) shared(valTab, surface)
    for (size_t i = 0; i < surface; ++i) {
        data[i] = valTab[i];
    }
}

template<typename T>
Matrix<T>::Matrix(const size_t &width, const size_t &height, const T val) : width(width), height(height),
                                                                            surface(height * width),
                                                                            data(new T[width * height]) {
#pragma omp parallel for default(none) shared(val, surface)
    for (size_t i = 0; i < surface; ++i) {
        data[i] = val;
    }
}

template<typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other) {
    if (&other != this) {
        width = other.width;
        height = other.height;
        surface = other.surface;
        delete[] data;
        data = new T[surface];
#pragma omp parallel for default(none) shared(other, surface)
        for (size_t i = 0; i < surface; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template<typename T>
Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other) {
    width = other.width;
    height = other.height;
    surface = other.surface;
    delete[] data;
    data = other.data;
    other.data = nullptr;
    return *this;
}

template<typename T>
Matrix<T>::~Matrix() {
    delete[] data;
}

template<typename T>
const T &Matrix<T>::at(const size_t &x, const size_t &y) const {
    return data[x + y * width];
}

template<typename T>
T &Matrix<T>::at(const size_t &x, const size_t &y) {
    return data[x + y * width];
}

template<typename T>
const T &Matrix<T>::at(const size_t &i) const {
    return data[i];
}

template<typename T>
T &Matrix<T>::at(const size_t &i) {
    return data[i];
}

template<typename T>
size_t Matrix<T>::get_width() const {
    return width;
}

template<typename T>
size_t Matrix<T>::get_height() const {
    return height;
}

template<typename T>
size_t Matrix<T>::get_surface() const {
    return surface;
}

template<typename T>
T *Matrix<T>::get_data() {
    return data;
}

template<typename T>
const T *Matrix<T>::get_data() const {
    return data;
}

template<typename T>
Matrix<T>::Iterator::Iterator(Matrix<T> *mat, const size_t &val) : mat(mat), val(val) {}

template<typename T>
typename Matrix<T>::Iterator &Matrix<T>::Iterator::operator++() {
    ++val;
    return *this;
}

template<typename T>
typename Matrix<T>::Iterator Matrix<T>::Iterator::operator++(int) {
    Iterator old(*this);
    ++val;
    return old;
}

template<typename T>
bool Matrix<T>::Iterator::operator==(const Iterator &other) const {
    return (mat == other.mat && val == other.val);
}

template<typename T>
bool Matrix<T>::Iterator::operator!=(const Iterator &other) const {
    return (mat != other.mat || val != other.val);
}

template<typename T>
T &Matrix<T>::Iterator::operator*() {
    return mat->at(val);
}

template<typename T>
T *Matrix<T>::Iterator::operator->() {
    return &(mat->at(val));
}

template<typename T>
Matrix<T>::Const_Iterator::Const_Iterator(const Matrix<T> *mat, const size_t &val) : mat(mat), val(val) {}

template<typename T>
typename Matrix<T>::Const_Iterator &Matrix<T>::Const_Iterator::operator++() {
    ++val;
    return *this;
}

template<typename T>
typename Matrix<T>::Const_Iterator Matrix<T> ::Const_Iterator::operator++(int) {
    Const_Iterator old(*this);
    ++val;
    return old;
}

template<typename T>
bool Matrix<T>::Const_Iterator::operator==(const Const_Iterator &other) const {
    return (mat == other.mat && val == other.val);
}

template<typename T>
bool Matrix<T>::Const_Iterator::operator!=(const Const_Iterator &other) const {
    return (mat != other.mat || val != other.val);
}

template<typename T>
const T &Matrix<T>::Const_Iterator::operator*() {
    return mat->at(val);
}

template<typename T>
const T *Matrix<T>::Const_Iterator::operator->() {
    return &(mat->at(val));
}

template<typename T>
typename Matrix<T>::Iterator Matrix<T>::begin() {
    return Iterator(this, 0);
}

template<typename T>
typename Matrix<T>::Const_Iterator Matrix<T>::begin() const {
    return Const_Iterator(this, 0);
}

template<typename T>
typename Matrix<T>::Iterator Matrix<T>::end() {
    return Iterator(this, surface);
}

template<typename T>
typename Matrix<T>::Const_Iterator Matrix<T>::end() const {
    return Const_Iterator(this, surface);
}

#endif //CPP_UTILS_MATRIX_H

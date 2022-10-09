//
// Created by charles on 28/04/22.
// Matrix class, row-major
//

#ifndef CPP_UTILS_MATRIX_H
#define CPP_UTILS_MATRIX_H

#include <cstdio>
#include <cstdlib>

template<typename T>
class Matrix {
public:
    Matrix() = default;

    Matrix(const size_t &width, const size_t &height) : width(width), height(height),
                                                        data(new T[width * height]) {}

    Matrix(const Matrix<T> &other) : width(other.width), height(other.height),
                                     data(new T[other.width * other.height]) {
#pragma omp parallel for default(shared)
        for (size_t i = 0; i < (width * height); ++i) {
            data[i] = other.data[i];
        }
    }

    Matrix(Matrix<T> &&other) : width(other.width), height(other.height),
                                data(other.data) {
        other.data = nullptr;
    }

    Matrix(const size_t &width, const size_t &height, const T *valTab) : width(width), height(height),
                                                                         data(new T[width * height]) {
#pragma omp parallel for default(shared)
        for (size_t i = 0; i < width * height; ++i) {
            data[i] = valTab[i];
        }
    }

    Matrix(const size_t &width, const size_t &height, const T val) : width(width), height(height),
                                                                     data(new T[width * height]) {
#pragma omp parallel for default(shared)
        for (size_t i = 0; i < width * height; ++i) {
            data[i] = val;
        }
    }

    Matrix<T> &operator=(const Matrix<T> &other) {
        if (&other != this) {
            width = other.width;
            height = other.height;
            delete[] data;
            data = new T[width * height];
#pragma omp parallel for default(shared)
            for (size_t i = 0; i < (width * height); ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    Matrix<T> &operator=(Matrix<T> &&other) {
        width = other.width;
        height = other.height;
        delete[] data;
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    ~Matrix() {
        delete[] data;
    }

    const T &at(const size_t &x, const size_t &y) const {
        return data[x + y * width];
    }

    T &at(const size_t &x, const size_t &y) {
        return data[x + y * width];
    }

    const T &at(const size_t &i) const {
        return data[i];
    }

    T &at(const size_t &i) {
        return data[i];
    }

    size_t get_width() const {
        return width;
    }

    size_t get_height() const {
        return height;
    }

    T *get_data() {
        return data;
    }

    const T *get_data() const {
        return data;
    }

    class Iterator {
    public:
        Iterator(Matrix<T> *mat, const size_t &val) : mat(mat), val(val) {}

        Iterator(const Iterator &other) = default;

        Iterator(Iterator &&other) = default;

        Iterator &operator=(const Iterator &other) = default;

        Iterator &operator=(Iterator &&other) = default;

        ~Iterator() = default;

        Iterator &operator++() {
            ++val;
            return *this;
        }

        Iterator operator++(int) {
            Iterator old(*this);
            ++val;
            return old;
        }

        bool operator==(const Iterator &other) const {
            return (mat == other.mat && val == other.val);
        }

        bool operator!=(const Iterator &other) const {
            return (mat != other.mat || val != other.val);
        }

        T &operator*() {
            return mat->at(val);
        }

        T *operator->() {
            return &(mat->at(val));
        }

    private:
        Matrix<T> *mat;
        size_t val = {};
    };

    class Const_Iterator {
    public:
        Const_Iterator(const Matrix<T> *mat, const size_t &val) : mat(mat), val(val) {}

        Const_Iterator(const Const_Iterator &other) = default;

        Const_Iterator(Const_Iterator &&other) = default;

        Const_Iterator &operator=(const Const_Iterator &other) = default;

        Const_Iterator &operator=(Const_Iterator &&other) = default;

        ~Const_Iterator() = default;

        Const_Iterator &operator++() {
            ++val;
            return *this;
        }

        Const_Iterator operator++(int) {
            Const_Iterator old(*this);
            ++val;
            return old;
        }

        bool operator==(const Const_Iterator &other) const {
            return (mat == other.mat && val == other.val);
        }

        bool operator!=(const Const_Iterator &other) const {
            return (mat != other.mat || val != other.val);
        }

        const T &operator*() {
            return mat->at(val);
        }

        const T *operator->() {
            return &(mat->at(val));
        }

    private:
        const Matrix<T> *mat;
        size_t val = {};
    };

    Iterator begin() {
        return Iterator(this, 0);
    }

    Const_Iterator begin() const {
        return Const_Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, width * height);
    }

    Const_Iterator end() const {
        return Const_Iterator(this, width * height);
    }

private:
    size_t width = 0;
    size_t height = 0;
    T *data = nullptr;
};

#endif //CPP_UTILS_MATRIX_H

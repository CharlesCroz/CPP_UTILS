//
// Created by charles on 28/04/22.
//

#ifndef CPP_UTILS_MATRIX_H
#define CPP_UTILS_MATRIX_H

#include <cstdio>
#include <cstdlib>

template<typename T>
class Matrix {
public:
    Matrix() = default;

    Matrix(const size_t &width, const size_t &height) : _width(width), _height(height),
                                                        _data(new T[width * height]) {}

    Matrix(const Matrix<T> &other) : _width(other._width), _height(other._height),
                                     _data(new T[other._width * other._height]) {
        for (size_t i = 0; i < (_width * _height); ++i) {
            _data[i] = other._data[i];
        }
    }

    Matrix(Matrix<T> &&other) noexcept: _width(other._width), _height(other._height), _data(other._data) {
        other._data = nullptr;
    }

    Matrix(const size_t &width, const size_t &height, const T *val) : _width(width), _height(height),
                                                                      _data(new T[width * height]) {
        for (size_t i = 0; i < width * height; ++i) {
            _data[i] = val[i];
        }
    }

    Matrix<T> &operator=(const Matrix<T> &other) {
        if (&other != this) {
            _width = other._width;
            _height = other._height;
            delete[] _data;
            _data = new T[_width * _height];
            for (size_t i = 0; i < (_width * _height); ++i) {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }

    Matrix<T> &operator=(Matrix<T> &&other) noexcept {
        _width = other._width;
        _height = other._height;
        delete[] _data;
        _data = other._data;
        other._data = nullptr;
        return *this;
    }

    ~Matrix() {
        delete[] _data;
    }

    const T &at(const size_t &x, const size_t &y) const {
        return _data[x + y * _width];
    }

    T &at(const size_t &x, const size_t &y) {
        return _data[x + y * _width];
    }

    const T &at(const size_t &i) const {
        return _data[i];
    }

    T &at(const size_t &i) {
        return _data[i];
    }

    size_t get_width() const {
        return _width;
    }

    size_t get_height() const {
        return _height;
    }

    class Iterator {
    public:
        Iterator(Matrix<T> *mat, const size_t &val) : _mat(mat), _val(val) {}

        Iterator(const Iterator &other) = default;
        Iterator(Iterator &&other) = default;
        Iterator &operator=(const Iterator &other) = default;
        Iterator &operator=(Iterator &&other) = default;
        ~Iterator() = default;

        Iterator &operator++() {
            ++_val;
            return *this;
        }

        Iterator operator++(int) {
            Iterator old(*this);
            ++_val;
            return old;
        }

        bool operator==(const Iterator &other) const {
            return (_mat == other._mat && _val == other._val);
        }

        bool operator!=(const Iterator &other) const {
            return (_mat != other._mat || _val != other._val);
        }

        T &operator*() {
            return _mat->at(_val);
        }

    private:
        Matrix<T> *_mat;
        size_t _val = {};
    };

    class Const_Iterator {
    public:
        Const_Iterator(const Matrix<T> *mat, const size_t &val) : _mat(mat), _val(val) {}

        Const_Iterator(const Const_Iterator &other) = default;
        Const_Iterator(Const_Iterator &&other) = default;
        Const_Iterator &operator=(const Const_Iterator &other) = default;
        Const_Iterator &operator=(Const_Iterator &&other) = default;
        ~Const_Iterator() = default;

        Const_Iterator &operator++() {
            ++_val;
            return *this;
        }

        Const_Iterator operator++(int) {
            Const_Iterator old(*this);
            ++_val;
            return old;
        }

        bool operator==(const Const_Iterator &other) const {
            return (_mat == other._mat && _val == other._val);
        }

        bool operator!=(const Const_Iterator &other) const {
            return (_mat != other._mat || _val != other._val);
        }

        const T &operator*() {
            return _mat->at(_val);
        }

    private:
        const Matrix<T> *_mat;
        size_t _val = {};
    };

    Iterator begin() {
        return Iterator(this, 0);
    }

    Const_Iterator begin() const {
        return Const_Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, _width * _height);
    }

    Const_Iterator end() const {
        return Const_Iterator(this, _width * _height);
    }

private:
    size_t _width = 0;
    size_t _height = 0;
    T *_data = nullptr;
};

#endif //CPP_UTILS_MATRIX_H

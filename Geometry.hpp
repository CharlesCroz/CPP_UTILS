#ifndef CPP_UTILS_GEOMETRY_H
#define CPP_UTILS_GEOMETRY_H

#include <stdexcept>
#include <math.h>

struct Point;
struct Vector;

using Distance = double;
using Area = double;
using Volume = double;

struct Point
{
    Distance x;
    Distance y;
    Distance z;

    Point(Distance, Distance, Distance);
    Point(const Vector &);
    Point() = default;
    Point(const Point &) = default;
    Point(Point &&) = default;
    Point &operator=(const Point &) = default;
    Point &operator=(Point &&) = default;

    Distance &operator[](int);
    Distance operator[](int) const;
    Point operator+(const Vector &) const;
    Point operator-(const Vector &) const;
    Vector operator-(const Point &) const;
    void operator+=(const Vector &);
    void operator-=(const Vector &);

    bool isInsideCircle(const Point&, const Point&, const Point&) const;
    bool isInsideTriangle(const Point&, const Point&, const Point&) const;
};

Point barycenter(double, double, double, const Point &, const Point &, const Point &);
Point isobarycenter(const Point &, const Point &, const Point &);

struct Vector{
    Distance x;
    Distance y;
    Distance z;

    Vector(Distance, Distance, Distance);
    Vector(const Point &);
    Vector() = default;
    Vector(const Vector &) = default;
    Vector(Vector &&) = default;
    Vector &operator=(const Vector &) = default;
    Vector &operator=(Vector &&) = default;

    Distance &operator[](int);
    Distance operator[](int) const;
    Vector operator+(const Vector &) const;
    Vector operator-(const Vector &) const;
    Vector operator-() const;
    void operator+=(const Vector &);
    void operator-=(const Vector &);
    Vector operator*(Distance) const;
    friend Vector operator*(Distance, const Vector &);
    void operator*=(Distance);

    Distance length() const;
};

Area dotProduct(const Vector &, const Vector &);
Vector crossProduct(const Vector &, const Vector &);
Volume det(const Vector &, const Vector &, const Vector &);
Distance length(const Vector &);

Point circumcenter(const Point &, const Point &, const Point &);

double tan(const Vector &, const Vector &);

int orientation2D(const Point &, const Point &, const Point &);
bool isInsideCircle(const Point &, const Point &, const Point &, const Point &);
bool isInsideTriangle(const Point &, const Point &, const Point &, const Point &);


// Functions definitions

Point::Point(Distance x, Distance y, Distance z) : x(x), y(y), z(z) {}

Point::Point(const Vector &v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

Distance &Point::operator[](int i) {
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    if (i == 2)
        return z;
    throw std::invalid_argument("Distance &Point::operator[](int i) requires i to be 0, 1 or 2");
}

Distance Point::operator[](int i) const {
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    if (i == 2)
        return z;
    throw std::invalid_argument("Distance Point::operator[](int i) const requires i to be 0, 1 or 2");
}

Point Point::operator+(const Vector &v) const {
    return {x + v.x, y + v.y, z + v.z};
}

Point Point::operator-(const Vector &v) const {
    return {x - v.x, y - v.y, z - v.z};
}

Vector Point::operator-(const Point &p) const {
    return {x - p.x, y - p.y, z - p.z};
}

void Point::operator+=(const Vector &v) {
    x += v.x;
    y += v.y;
    z += v.z;
}

void Point::operator-=(const Vector &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

bool Point::isInsideCircle(const Point &p0, const Point &p1, const Point &p2) const {
    return ::isInsideCircle(p0, p1, p2, *this);
}

bool Point::isInsideTriangle(const Point &p0, const Point &p1, const Point &p2) const {
    return ::isInsideTriangle(p0, p1, p2, *this);
}


Vector::Vector(Distance x, Distance y, Distance z) : x(x), y(y), z(z) {}

Vector::Vector(const Point &p) {
    x = p.x;
    y = p.y;
    z = p.z;
}

Distance &Vector::operator[](int i) {
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    if (i == 2)
        return z;
    throw std::invalid_argument("Distance &Vector::operator[](int i) requires i to be 0, 1 or 2");
}

Distance Vector::operator[](int i) const {
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    if (i == 2)
        return z;
    throw std::invalid_argument("Distance Vector::operator[](int i) const requires i to be 0, 1 or 2");
}

Vector Vector::operator+(const Vector &v) const {
    return {x + v.x, y + v.y, z + v.z};
}

Vector Vector::operator-(const Vector &v) const {
    return {x - v.x, y - v.y, z - v.z};
}

Vector Vector::operator-() const {
    return {-x, -y, -z};
}

void Vector::operator+=(const Vector &v) {
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vector::operator-=(const Vector &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

Vector Vector::operator*(Distance d) const {
    return {d * x, d * y, d * z};
}

void Vector::operator*=(Distance d) {
    x *= d;
    y *= d;
    z *= d;
}

Distance Vector::length() const {
    return sqrt(x * x + y * y + z * z);
}

Area dotProduct(const Vector &v1, const Vector &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector crossProduct(const Vector &v1, const Vector &v2) {
    return {v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x};
}

Volume det(const Vector &v1, const Vector &v2, const Vector &v3) {
    return dotProduct(crossProduct(v1, v2), v3);
}

Distance length(const Vector &v) {
    return v.length();
}

int orientation2D(const Point &p0, const Point &p1, const Point &p2) {
    Area a = det(p1 - p0, p2 - p0, Vector(0, 0, 1));
    if (a > 0)
        return 1;
    if (a < 0)
        return -1;
    return 0;
}

bool isInsideCircle(const Point &p0, const Point &p1, const Point &p2, const Point &p) {
    Vector v1 = p1 - p0;
    Vector v2 = p2 - p0;
    Vector v = p - p0;
    v1.z = v1.x * v1.x + v1.y * v1.y;
    v2.z = v2.x * v2.x + v2.y * v2.y;
    v.z = v.x * v.x + v.y * v.y;
    return det(v1, v2, v) > 0;
}

bool isInsideTriangle(const Point &p0, const Point &p1, const Point &p2, const Point &p) {
    if (orientation2D(p, p0, p1) != 1) {
        return false;
    } else if (orientation2D(p, p1, p2) != 1) {
        return false;
    } else if (orientation2D(p, p2, p0) != 1) {
        return false;
    }
    return true;
}

Vector operator*(Distance d, const Vector &v) {
    return {d * v.x, d * v.y, d * v.z};
}

Point circumcenter(const Point &p0, const Point &p1, const Point &p2) {
    Vector v0 = p1 - p0;
    Vector v1 = p2 - p1;
    Vector v2 = p0 - p2;

    double sin0 = length(crossProduct(v0, v2));
    double sin1 = length(crossProduct(v1, v0));
    double sin2 = length(crossProduct(v2, v1));
    double cos0 = dotProduct(v0, v2);
    double cos1 = dotProduct(v1, v0);
    double cos2 = dotProduct(v2, v1);

    double c0 = sin0 * cos1 * cos2;
    double c1 = sin1 * cos2 * cos0;
    double c2 = sin2 * cos0 * cos1;

    return barycenter(c1 + c2, c2 + c0, c0 + c1, p0, p1, p2);
}

inline Point barycenter(double a0, double a1, double a2, const Point &p0, const Point &p1, const Point &p2) {
    double a = 1 / (a0 + a1 + a2);
    return {a * (a0 * p0.x + a1 * p1.x + a2 * p2.x),
            a * (a0 * p0.y + a1 * p1.y + a2 * p2.y),
            a * (a0 * p0.z + a1 * p1.z + a2 * p2.z)};
}

Point isobarycenter(const Point &p0, const Point &p1, const Point &p2) {
    return barycenter(1, 1, 1, p0, p1, p2);
}

#endif // CPP_UTILS_GEOMETRY_H

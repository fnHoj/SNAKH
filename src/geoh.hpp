#pragma once

#include "irrationoid.hpp"

namespace geoh {
    using namespace std;
    typedef irrationoid::Irrationoid<long long, 5> irr;
    const irr PHI = irr(1, 1, 2);
    const irr phi = irr(-1, 1, 2);
    const double SQRT_PHI = sqrt(double(PHI));
    
    struct Vec {
        irr z, x, y;

        operator string() const {
            return "(" + string(z) + ", " + string(x) + ", " + string(y) + ")";
        }

        Vec(): z(0), x(0), y(0) {}
        Vec(const irr& z, const irr& x, const irr& y): z(z), x(x), y(y) {}
    
        inline const Vec& operator+() const {
            return *this;
        }
        inline const Vec operator-() const {
            return Vec(-z, -x, -y);
        }
        inline const Vec operator+(const Vec& b) const {
            return Vec(z + b.z, x + b.x, y + b.y);
        }
        inline const Vec operator-(const Vec& b) const {
            return Vec(z - b.z, x - b.x, y - b.y);
        }
        inline const Vec operator*(const irr& k) const {
            return Vec(z * k, x * k, y * k);
        }
        inline const Vec operator/(const irr& k) const {
            return Vec(z / k, x / k, y / k);
        }
        Vec& operator+=(const Vec& b) {
            z += b.z; x += b.x; y += b.y;
            return *this;
        }
        Vec& operator-=(const Vec& b) {
            z -= b.z; x -= b.x; y -= b.y;
            return *this;
        }
        Vec& operator*=(const irr& k) {
            z *= k; x *= k; y *= k;
            return *this;
        }
        Vec& operator/=(const irr& k) {
            z /= k; x /= k; y /= k;
            return *this;
        }
    
        inline const irr norm() const {
            return z * z - PHI * (x * x + y * y);
        }
    
        inline bool operator==(const Vec& b) const {
            return z == b.z && x == b.x && y == b.y;
        }
        inline bool operator!=(const Vec& b) const {
            return z != b.z || x != b.x || y != b.y;
        }
        inline bool operator<(const Vec& b) const {
            return z != b.z ? z < b.z : x != b.x ? x < b.x : y < b.y;
        }
        inline bool operator>(const Vec& b) const {
            return z != b.z ? z > b.z : x != b.x ? x > b.x : y > b.y;
        }
        inline bool operator<=(const Vec& b) const {
            return z != b.z ? z < b.z : x != b.x ? x < b.x : y <= b.y;
        }
        inline bool operator>=(const Vec& b) const {
            return z != b.z ? z > b.z : x != b.x ? x > b.x : y >= b.y;
        }
    };
    
    inline const Vec operator*(const irr& k, const Vec& a) {
        return Vec(k * a.z, k * a.x, k * a.y);
    }
    const irr dot(const Vec& a, const Vec& b) {
        return a.z * b.z - PHI * (a.x * b.x + a.y * b.y);
    }
    const Vec cross(const Vec& a, const Vec& b) {
        return Vec(
            PHI * (a.x * b.y - a.y * b.x),
            a.z * b.y - a.y * b.z,
            a.x * b.z - a.z * b.x
        );
    }
    
    struct Mat {
        static const Mat identity;

        irr zz, zx, zy;
        irr xz, xx, xy;
        irr yz, yx, yy;

        Mat():
            zz(), zx(), zy(),
            xz(), xx(), xy(),
            yz(), yx(), yy() {}
        Mat(const Mat& b):
            zz(b.zz), zx(b.zx), zy(b.zy),
            xz(b.xz), xx(b.xx), xy(b.xy),
            yz(b.yz), yx(b.yx), yy(b.yy) {}
        Mat(
            const irr& zz, const irr& zx, const irr& zy,
            const irr& xz, const irr& xx, const irr& xy,
            const irr& yz, const irr& yx, const irr& yy
        ):
            zz(zz), zx(zx), zy(zy),
            xz(xz), xx(xx), xy(xy),
            yz(yz), yx(yx), yy(yy) {}
        
        operator string() const {
            return "[\n"
                + string(zz) + ", " + string(zx) + ", " + string(zy) + "\n"
                + string(xz) + ", " + string(xx) + ", " + string(xy) + "\n"
                + string(yz) + ", " + string(yx) + ", " + string(yy) + "\n"
                + "]";
        }

        inline const Mat& operator+() const {
            return *this;
        }
        inline const Mat operator-() const {
            return Mat(
                -zz, -zx, -zy,
                -xz, -xx, -xy,
                -yz, -yx, -yy
            );
        }
        inline const Mat operator+(const Mat& b) const {
            return Mat(
                zz + b.zz, zx + b.zx, zy + b.zy,
                xz + b.xz, xx + b.xx, xy + b.xy,
                yz + b.yz, yx + b.yx, yy + b.yy
            );
        }
        inline const Mat operator-(const Mat& b) const {
            return Mat(
                zz - b.zz, zx - b.zx, zy - b.zy,
                xz - b.xz, xx - b.xx, xy - b.xy,
                yz - b.yz, yx - b.yx, yy - b.yy
            );
        }
        inline const Mat operator*(const irr& k) const {
            return Mat(
                zz * k, zx * k, zy * k,
                xz * k, xx * k, xy * k,
                yz * k, yx * k, yy * k
            );
        }
        inline const Mat operator/(const irr& k) const {
            return Mat(
                zz / k, zx / k, zy / k,
                xz / k, xx / k, xy / k,
                yz / k, yx / k, yy / k
            );
        }
        inline const Vec operator*(const Vec& v) const {
            return Vec(
                zz * v.z + zx * v.x + zy * v.y,
                xz * v.z + xx * v.x + xy * v.y,
                yz * v.z + yx * v.x + yy * v.y
            );
        }
        inline const Mat operator*(const Mat& b) const {
            return Mat(
                zz * b.zz + zx * b.xz + zy * b.yz,  zz * b.zx + zx * b.xx + zy * b.yx,  zz * b.zy + zx * b.xy + zy * b.yy,
                xz * b.zz + xx * b.xz + xy * b.yz,  xz * b.zx + xx * b.xx + xy * b.yx,  xz * b.zy + xx * b.xy + xy * b.yy,
                yz * b.zz + yx * b.xz + yy * b.yz,  yz * b.zx + yx * b.xx + yy * b.yx,  yz * b.zy + yx * b.xy + yy * b.yy
            );
        }

        inline Mat& operator+=(const Mat& b) {
            return operator=(operator+(b));
        }
        inline Mat& operator-=(const Mat& b) {
            return operator=(operator-(b));
        }
    };

    const Mat Mat::identity(1, 0, 0, 0, 1, 0, 0, 0, 1);

    inline const Mat operator*(const irr& k, const Mat& m) {
        return m * k;
    }

    inline const Vec operator*(const Vec& v, const Mat& m) {
        return m * v;
    }
    inline Vec& operator*=(Vec& v, const Mat& m) {
        return v = m * v;
    }
}

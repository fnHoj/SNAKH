#pragma once

#include <string>
#include <cmath>

namespace irrationoid {
    using namespace std;

    template<typename T, T p> class Irrationoid {
    private:
        static const T iabs(const T&);
        static const T gcd(T, T);
        static inline const T gcd(const T&, const T&, const T&);
        static const string dec(T);
    public:
        T a, b, q;

        void simplify() {
            if (!q) {
                a = 1;
                b = 0;
                return;
            }
            T g = gcd(a, b, q);
            if (q < 0)
                g = -g;
            a /= g;
            b /= g;
            q /= g;
        }

        Irrationoid<T, p> (T a = 0, T b = 0, T q = 1): a(a), b(b), q(q) {
            simplify();
        }

        inline explicit operator double() const {
            return (sqrt(static_cast<double>(p)) * b + a) / q;
        }

        int sign() const {
            if (!a && !b)
                return 0;
            if (a >= 0 && b >= 0)
                return 1;
            if (a <= 0 && b <= 0)
                return -1;
            if (a > 0)
                return (a*a > p*b*b) ? 1 : -1;
            else
                return (p*b*b > a*a) ? 1 : -1;
        }
        inline explicit operator bool() const {
            return sign();
        }

        inline bool operator==(const Irrationoid<T, p>& other) const {
            return operator-(other).sign() == 0;
        }
        inline bool operator!=(const Irrationoid<T, p>& other) const {
            return operator-(other).sign() != 0;
        }
        inline bool operator<(const Irrationoid<T, p>& other) const {
            return operator-(other).sign() < 0;
        }
        inline bool operator>(const Irrationoid<T, p>& other) const {
            return operator-(other).sign() > 0;
        }
        inline bool operator<=(const Irrationoid<T, p>& other) const {
            return operator-(other).sign() <= 0;
        }
        inline bool operator>=(const Irrationoid<T, p>& other) const {
            return operator-(other).sign() >= 0;
        }

        inline const Irrationoid<T, p>& operator+() const {
            return *this;
        }
        inline const Irrationoid<T, p> operator-() const {
            return Irrationoid(-a, -b, q);
        }
        inline const Irrationoid<T, p> operator+(const Irrationoid<T, p>& other) const {
            return Irrationoid(a*other.q + other.a*q, b*other.q + other.b*q, q*other.q);
        }
        inline const Irrationoid<T, p> operator-(const Irrationoid<T, p>& other) const {
            return operator+(-other);
        }

        inline const Irrationoid<T, p> inverse() const {
            return Irrationoid(q*a, -q*b, a*a - p*b*b);
        }
        inline const Irrationoid<T, p> operator*(const Irrationoid<T, p>& other) const {
            return Irrationoid(a*other.a + p * b*other.b, a*other.b + other.a*b, q*other.q);
        }
        inline const Irrationoid<T, p> operator/(const Irrationoid<T, p>& other) const {
            return operator*(other.inverse());
        }

        inline Irrationoid<T, p>& operator+=(const Irrationoid<T, p>& other) {
            return *this = operator+(other);
        }
        inline Irrationoid<T, p>& operator-=(const Irrationoid<T, p>& other) {
            return *this = operator-(other);
        }
        inline Irrationoid<T, p>& operator*=(const Irrationoid<T, p>& other) {
            return *this = operator*(other);
        }
        inline Irrationoid<T, p>& operator/=(const Irrationoid<T, p>& other) {
            return *this = operator/(other);
        }
        inline Irrationoid<T, p>& operator++() {
            return operator+=(1);
        }
        inline Irrationoid<T, p>& operator--() {
            return operator-=(1);
        }
        inline const Irrationoid<T, p> operator++(int) {
            Irrationoid<T, p> tmp = *this;
            operator++();
            return tmp;
        }
        inline const Irrationoid<T, p> operator--(int) {
            Irrationoid<T, p> tmp = *this;
            operator--();
            return tmp;
        }

        operator string() {
            string ans;
            simplify();
            if (!q) {
                if (a || b)
                    return "inf";
                return "nan";
            }
            switch (sign()) {
                case 0:
                    return "0";
                case -1:
                    return "-" + string(operator-());
            }
            if (!a) {
                if (b == 1)
                    ans = "\\sqrt{" + dec(p) + "}";
                else
                    ans = dec(b) + "\\sqrt{" + dec(p) + "}";
            }
            else if (a > 0) {
                ans = dec(a);
                if (b == 1)
                    ans += "+\\sqrt{" + dec(p) + "}";
                else if (b > 1)
                    ans += "+" + dec(b) + "\\sqrt{" + dec(p) + "}";
                else if (b == -1)
                    ans += "-\\sqrt{" + dec(p) + "}";
                else if (b < -1)
                    ans += "-" + dec(b) + "\\sqrt{" + dec(p) + "}";
            }
            else {
                if (b == 1)
                    ans = "\\sqrt{" + dec(p) + "}-" + dec(-a);
                else
                    ans = dec(b) + "\\sqrt{" + dec(p) + "}-" + dec(-a);
            }
            if (q == 1)
                return ans;
            return "\\frac{" + ans + "}{" + dec(q) + "}";
        }
        operator string() const {
            Irrationoid<T, p> tmp = *this;
            return string(tmp);
        }
    };

    template<typename T, T p> const T Irrationoid<T, p>::iabs(const T& x) {
        return x < 0 ? -x : x;
    }
    template<typename T, T p> const T Irrationoid<T, p>::gcd(T a, T b) {
        a = iabs(a);
        b = iabs(b);
        if (!b)
            return a;
        return gcd(b, a%b);
    }
    template<typename T, T p> inline const T Irrationoid<T, p>::gcd(const T& a, const T& b, const T& c) {
        return gcd(gcd(a, b), c);
    }
    template<typename T, T p> const string Irrationoid<T, p>::dec(T x) {
        string ans;
        bool sign = (x < 0);
        if (sign)
            x = -x;
        do {
            ans = char('0' + x % 10) + ans;
        } while (x /= 10);
        if (sign)
            ans = "-" + ans;
        return ans;
    }

    template<typename T, T p> const Irrationoid<T, p> operator+(const T& a, const Irrationoid<T, p>& b) {
        return b + a;
    }
    template<typename T, T p> const Irrationoid<T, p> operator-(const T& a, const Irrationoid<T, p>& b) {
        return -b + a;
    }
    template<typename T, T p> const Irrationoid<T, p> operator*(const T& a, const Irrationoid<T, p>& b) {
        return b * a;
    }
    template<typename T, T p> const Irrationoid<T, p> operator/(const T& a, const Irrationoid<T, p>& b) {
        return b.inverse() * a;
    }
}

#pragma once
#include "geoh.hpp"
#include <queue>

namespace gridlines {
    using namespace geoh;
    using namespace std;
    const Vec rot(const Vec& a, const Vec& b) {
        return a * dot(a, b) - cross(a, b);
    }
    const Vec opposite(const Vec& a, const Vec& b) {
        return a * (dot(a, b) * 2) - b;
    }
    const Vec rot_clockwise(const Vec& a, const Vec& b) {
        return a * dot(a, b) + cross(a, b);
    }
    const unsigned steps = 0x10;
    const irr MAXZ = 128;
    struct Tile {
        unsigned prec;
        Vec pos;

        Tile(): prec(), pos() {}
        Tile(unsigned prec, const Vec& pos): prec(prec), pos(pos) {}
    };
    class GridLines {
    public:
        unsigned n = 5;
        Tile tile[0x4000] = {
            {0, Vec(1, 0, 0)},
            {0, Vec(PHI, 1, 0)},
            {0, Vec(PHI, 0, 1)},
            {0, Vec(PHI, -1, 0)},
            {0, Vec(PHI, 0, -1)},
        };
        unsigned m = 4;
        unsigned line[0x8000][2] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}};

        unsigned attemt_expansion(const Vec& pos, unsigned prec) {
            for (unsigned i = 0; i < n; i++) {
                if (pos == tile[i].pos) {
                    if (i < prec) {
                        line[m][0] = i;
                        line[m][1] = prec;
                        m++;
                    }
                    return -1;
                }
            }
            if (tile[prec].pos.z > MAXZ)
                return -1;
            tile[n] = Tile(prec, pos);
            line[m][0] = prec;
            line[m][1] = n;
            m++;
            return n++;
        }

        GridLines() {
            queue<unsigned> q;
            Vec cpos, precpos;
            q.push(1); q.push(2); q.push(3); q.push(4);
            while (!q.empty()) {
                unsigned x = q.front();
                unsigned dest;
                q.pop();
                cpos = tile[x].pos;
                precpos = tile[tile[x].prec].pos;
                if (~(dest = attemt_expansion(rot(cpos, precpos), x)))
                    q.push(dest);
                if (~(dest = attemt_expansion(opposite(cpos, precpos), x)))
                    q.push(dest);
                if (~(dest = attemt_expansion(rot_clockwise(cpos, precpos), x)))
                    q.push(dest);
            }
        }
    };
}

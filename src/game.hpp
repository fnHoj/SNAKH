#pragma once

#include "irrationoid.hpp"
#include "geoh.hpp"
#include <queue>
#include <set>
#include <random>

namespace snakh {
    using namespace geoh;
    using namespace std;

    const Vec sid[4] = {
        {PHI, 1, 0},
        {PHI, 0, 1},
        {PHI, -1, 0},
        {PHI, 0, -1},
    };

    const Mat adj[4] = {
        {
            PHI, PHI, 0,
            1 , PHI, 0,
            0 ,  0 , 1
        },
        {
            PHI, -PHI, 0,
            -1 ,  PHI, 0,
            0 ,   0 , 1
        },
        {
            PHI, 0, PHI,
            0 , 1,  0 ,
            1 , 0, PHI
        },
        {
            PHI, 0, -PHI,
            0 , 1,   0 ,
            -1 , 0,  PHI
        }
    };

    class Game {
    protected:
        default_random_engine gen;   

        bool dead;
        Vec head, apple;
        queue<Vec> snake;
        set<Vec> is_snake;

        Mat to_orig;
        Mat to_head;

        inline const Vec transform_at_head(const Mat& mat, const Vec& v) const {
            return to_head * (mat * (to_orig * v));
        }
        inline const Vec transform_at_head(const Vec& v, const Mat& mat) const {
            return to_head * (mat * (to_orig * v));
        }

        bool append(const Vec& p) {
            if (dead)
                return false;
            if (!is_snake.insert(p).second)
                return false;
            snake.push(p);
            return true;
        }
        bool advance(unsigned direction) {
            if (dead)
                return false;
            direction &= 3;
            if (!append(to_head * sid[direction]))
                return false;
            to_orig = to_orig * adj[direction ^ 2];
            to_head = adj[direction] * to_head;
            return true;
        }
        const Vec pop() {
            const Vec tail = snake.front();
            snake.pop();
            is_snake.erase(tail);
            return tail;
        }
        const Vec random_empty_tile(unsigned turns = 32){
            Vec ans(1, 0, 0);
            Mat to_apple = Mat::identity;
            unsigned direction;
            while (turns > 0 || is_snake.count(ans)) {
                direction = uniform_int_distribution(0, 3)(gen);
                ans = to_apple * sid[direction];
                to_apple = adj[direction] * to_apple;
                turns--;
            }
            return ans;
        }
    public:
        Game(unsigned len = 8):
                gen(time(0)), dead(false), head(1, 0, 0),
                to_orig(Mat::identity), to_head(Mat::identity),
                snake(), is_snake() {
            for (unsigned i = 0; i < len; i++) {
                head *= adj[2];
            }
            snake.push(head);
            is_snake.insert(head);
            for (unsigned i = 1; i < len; i++) {
                advance(0);
            }
            apple = random_empty_tile();
        }
        bool move(unsigned direction) {
            if (dead)
                return false;
            if (!advance(direction)) {
                dead = true;
                return false;
            }
            if (head == apple)
                apple = random_empty_tile();
            else
                pop();
            return true;
        }
        bool is_dead() const {
            return dead;
        }
        const queue<Vec> get_snake() const {
            return snake;
        }
    };
}

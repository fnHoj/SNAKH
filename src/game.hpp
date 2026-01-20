#pragma once

#include "irrationoid.hpp"
#include "geoh.hpp"
#include <queue>
#include <set>
#include <random>
#include <iostream>

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
            PHI, 0, PHI,
             0 , 1,  0 ,
             1 , 0, PHI
        },
        {
            PHI, -PHI, 0,
            -1 ,  PHI, 0,
             0 ,   0 , 1
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

        bool growing, dead;
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
            const Vec p = to_head * sid[direction];
            if (!append(p))
                return false;
            head = p;
            to_orig = adj[direction ^ 2] * to_orig;
            to_head = to_head * adj[direction];
            return true;
        }
        void pop() {
            snake.pop();
            is_snake.erase(snake.front());
        }
        const Vec random_empty_tile(unsigned turns = 4){
            Vec ans(1, 0, 0);
            Mat to_apple = Mat::identity;
            unsigned direction;
            while (turns || is_snake.count(ans)) {
                direction = uniform_int_distribution(0, 3)(gen);
                ans = to_apple * sid[direction];
                to_apple = adj[direction] * to_apple;
                if (turns)
                    turns--;
            }
            return ans;
        }
    public:
        Game(unsigned len = 8):
                gen(time(0)), growing(false), dead(false), head(1, 0, 0),
                to_orig(Mat::identity), to_head(Mat::identity),
                snake(), is_snake() {
            for (unsigned i = 0; i < len; i++) {
                head *= adj[2];
                to_orig = adj[0] * to_orig;
                to_head = to_head * adj[2];
            }
            snake.push(head);
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
            if (head == apple) {
                apple = random_empty_tile();
                growing = true;
            }
            else {
                pop();
                growing = false;
            }
            return true;
        }
        bool is_growing() const {
            return growing;
        }
        bool is_dead() const {
            return dead;
        }
        const Vec& get_head() const {
            return head;
        }
        const Vec& get_apple() const {
            return apple;
        }
        const queue<Vec>& get_snake() const {
            return snake;
        }
        const Mat& get_to_orig() const {
            return to_orig;
        }
        const Mat& get_to_head() const {
            return to_head;
        }
    };
}

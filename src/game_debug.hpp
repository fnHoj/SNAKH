#pragma once

#include "game.hpp"
#include <string>
#include <iostream>
namespace game_debug {
    using namespace snakh;
    using namespace std;

    class GameDebug : public Game {
    public:
        GameDebug(): Game() {}
        const string print_snake() const {
            string ans = "";
            queue<Vec> snake1 = snake;
            while (!snake1.empty()) {
                Vec p = snake1.front();
                snake1.pop();
                ans += "(" + string(p.z) + ", " + string(p.x) + ", " + string(p.y) + ")\n";
            }
            cout << ans << endl;
            return ans;
        }
        void apples(unsigned n = 0x10) {
            Vec apple;
            for (unsigned i = 0; i < n; i++) {
                apple = random_empty_tile();
                cout << "(" + string(apple.z) +  ", " + string(apple.x) + ", " + string(apple.y) + ")" << endl;
            }
        }
    };
}
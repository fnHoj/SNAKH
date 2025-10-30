#pragma once
#include "graphical_utils.hpp"
using namespace snakh;
using namespace std;

void render(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    draw_horizon(window, game, direction, t);
    draw_apple(window, game, direction, t);
    draw_snake(window, game, direction, t);
}
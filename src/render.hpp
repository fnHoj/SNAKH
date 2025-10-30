#pragma once
#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace snakh;
using namespace std;

const double dist = asinh(SQRT_PHI);
const double line_width = 0.05;

const pair<double, double> proj(const Vec& p, unsigned direction = 0, double t = 0) {
    double z = double(p.z);
    double x = SQRT_PHI * double(p.x);
    double y = SQRT_PHI * double(p.y);
    double z1 = 1.0 + z;
    double c = cosh(dist * t), s = sinh(dist * t);
    if (!(direction & 2))
        s = -s;
    if (direction & 1) {
        z1 += x * s;
        return make_pair((z * s + x * c) / z1, y / z1);
    }
    else {
        z1 += y * s;
        return make_pair(x / z1, (z * s + y * c) / z1);
    }
}

const sf::Vector2f proj_screen(const Vec& p) {
    const pair<double, double> pos = proj(p);
    return sf::Vector2f(400.0 * (1.0 - pos.second), 400.0 * (1.0 - pos.first));
}

void render(sf::RenderWindow& window, const Game& game, double t) {
    queue<Vec> snake = game.get_snake();
    sf::CircleShape horizon(400);
    sf::VertexArray polyline(sf::PrimitiveType::LineStrip, 0);
    horizon.setFillColor({0x22, 0x22, 0x22});
    horizon.setPointCount(0x40);
    window.draw(horizon);
    // polyline.append({sf::Vector2f(400, 400), {0xff, 0xff, 0xff}});
    while (!snake.empty()) {
        polyline.append({proj_screen(game.get_to_orig() * snake.front()), {0xff, 0xff, 0xff}});
        snake.pop();
    }
    window.draw(polyline);
}
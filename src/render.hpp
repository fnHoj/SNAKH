#pragma once
#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace snakh;
using namespace std;

const double dist = asinh(SQRT_PHI);
const double line_width = 0.05;

const pair<pair<double, double>, double> proj(const Vec& p, unsigned direction = 0, double t = 0) {
    double z = double(p.z);
    double x = SQRT_PHI * double(p.x);
    double y = SQRT_PHI * double(p.y);
    double c = cosh(dist * t), s = sinh(dist * t);
    double z1;
    if (!(direction & 2))
        s = -s;
    if (direction & 1) {
        z1 = 1.0 + z * c + y * s;
        y = z * s + y * c;
    }
    else {
        z1 = 1.0 + z * c + x * s;
        x = z * s + x * c;
    }
    return make_pair(make_pair(x / z1, y / z1), 1 / z1);
}

const sf::Vector2f proj_screen(const Vec& p, unsigned direction = 0, double t = 0) {
    const pair<double, double> pos = proj(p, direction, t).first;
    return sf::Vector2f(400.0 * (1.0 - pos.second), 400.0 * (1.0 - pos.first));
}

void ball(sf::RenderWindow& window, const Vec& p, double r, unsigned direction = 0, double t = 0) {
    const pair<pair<double, double>, double> pos = proj(p, direction, t);
    double R = 400.0 * r * pos.second;
    if (R < 8)
        R = 8;
    sf::CircleShape circ(R);
    circ.setPosition(sf::Vector2f(400 * (1.0 - pos.first.second) - R, 400 * (1.0 - pos.first.first) - R));
    circ.setFillColor({0xff, 0x00, 0x00});
    window.draw(circ);
}

void render(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    const Mat to_orig = game.get_to_orig();
    queue<Vec> snake = game.get_snake();
    sf::CircleShape horizon(400);
    sf::VertexArray polyline(sf::PrimitiveType::LineStrip, 0);
    horizon.setFillColor({0x22, 0x22, 0x22});
    horizon.setPointCount(0x40);
    window.draw(horizon);
    ball(window, to_orig * game.get_apple(), 0.2, direction, t);
    while (!snake.empty()) {
        polyline.append({proj_screen(to_orig * snake.front(), direction, t), {0xff, 0xff, 0xff}});
        snake.pop();
    }
    polyline.append({sf::Vector2f(400, 400), {0xff, 0xff, 0xff}});
    window.draw(polyline);
}
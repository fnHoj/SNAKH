#pragma once
#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace snakh;
using namespace std;

const double dist = asinh(SQRT_PHI);
const double line_width = 0.05;

const pair<pair<double, double>, double> proj(double z, double x, double y, unsigned direction = 0, double t = 0) {
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
inline const pair<pair<double, double>, double> proj(const Vec& p, unsigned direction = 0, double t = 0) {
    return proj(double(p.z), SQRT_PHI * double(p.x), SQRT_PHI * double(p.y), direction, t);
}

const sf::Vector2f proj_screen(const Vec& p, unsigned direction = 0, double t = 0) {
    const pair<double, double> pos = proj(p, direction, t).first;
    return sf::Vector2f(400.0 * (1.0 - pos.second), 400.0 * (1.0 - pos.first));
}
const sf::Vector2f proj_screen(double z, double x, double y, unsigned direction = 0, double t = 0) {
    const pair<double, double> pos = proj(z, x, y, direction, t).first;
    return sf::Vector2f(400.0 * (1.0 - pos.second), 400.0 * (1.0 - pos.first));
}

void ball(sf::RenderWindow& window, const Vec& p, double r, unsigned direction = 0, double t = 0) {
    const pair<pair<double, double>, double> pos = proj(p, direction, t);
    double R = 400.0 * r * pos.second;
    if (R < 4)
        R = 4;
    sf::CircleShape circ(R);
    circ.setPosition(sf::Vector2f(400 * (1.0 - pos.first.second) - R, 400 * (1.0 - pos.first.first) - R));
    circ.setFillColor({0xff, 0x00, 0x00});
    window.draw(circ);
}

void interpolate(double z0, double x0, double y0, double z1, double x1, double y1, double t, double &z, double &x, double &y, double alpha = dist) {
    double s = sinh(alpha);
    double u = sinh(alpha * (1.0 - t)) / s;
    double v = sinh(alpha * t) / s;
    z = z0 * u + z1 * v;
    x = x0 * u + x1 * v;
    y = y0 * u + y1 * v;
}

void segment(sf::RenderWindow& window, double width, const sf::Color& color, double z0, double x0, double y0, double z1, double x1, double y1, unsigned direction, double t, unsigned steps = 0x10) {
    sf::VertexArray seg(sf::PrimitiveType::LineStrip, 0);
    double zn = x0 * y1 - y0 * x1;
    double xn = z0 * y1 - y0 * z1;
    double yn = x0 * z1 - z0 * x1;
    double nn = xn * xn + yn * yn - zn * zn;
    double c = sqrt(1 + nn);
    double s = sqrt(nn);
    double alpha = asinh(s);
    double k = width / s;
    double zi, xi, yi;
    zn *= k; xn *= k; yn *= k;
    for (unsigned i = 0; i <= steps; i++) {
        interpolate(z0, x0, y0, z1, x1, y1, double(i) / steps, zi, xi, yi, alpha);
        seg.append({proj_screen(zi + zn, xi + xn, yi + yn, direction, t), color});
        seg.append({proj_screen(zi - zn, xi - xn, yi - yn, direction, t), color});
    }
    window.draw(seg);
}
inline void segment(sf::RenderWindow& window, double width, const sf::Color& color, const Vec& p0, const Vec& p1, unsigned direction, double t, unsigned steps = 0x10) {
    segment(window, width, color,
        double(p0.z), SQRT_PHI * double(p0.x), SQRT_PHI * double(p0.y),
        double(p1.z), SQRT_PHI * double(p1.x), SQRT_PHI * double(p1.y),
        direction, t, steps
    );
}

const double snake_width = 0.1;
const sf::Color snake_color(0xff, 0xff, 0xff);

void draw_tail(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    queue<Vec> snake = game.get_snake();
    Vec p0 = game.get_to_orig() * snake.front();
    snake.pop();
    Vec p1 = game.get_to_orig() * snake.front();
    double z0 = double(p0.z);
    double x0 = SQRT_PHI * double(p0.x);
    double y0 = SQRT_PHI * double(p0.y);
    snake.pop();
    double z1 = double(p1.z);;
    double x1 = SQRT_PHI * double(p1.x);
    double y1 = SQRT_PHI * double(p1.y);
    interpolate(z0, x0, y0, z1, x1, y1, t, z0, x0, y0, (1 - t) * dist);
    segment(window, snake_width, snake_color, z0, x0, y0, z1, x1, y1, direction, t);
}
void draw_head(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    const Mat to_orig = game.get_to_orig();
    Vec head = to_orig * game.get_head();
    Vec dest = to_orig * game.get_to_head() * sid[direction];
    double z0 = double(head.z);
    double x0 = SQRT_PHI * double(head.x);
    double y0 = SQRT_PHI * double(head.y);
    double z1 = double(dest.z);
    double x1 = SQRT_PHI * double(dest.x);
    double y1 = SQRT_PHI * double(dest.y);
    interpolate(z0, x0, y0, z1, x1, y1, t, z1, x1, y1, t * dist);
    segment(window, snake_width, snake_color, z0, x0, y0, z1, x1, y1, direction, t);
}

void render(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    const Mat to_orig = game.get_to_orig();
    queue<Vec> snake = game.get_snake();
    sf::CircleShape horizon(400);
    snake.pop();
    Vec prevp = to_orig * snake.front();
    Vec currp = prevp;
    snake.pop();
    sf::VertexArray polyline(sf::PrimitiveType::LineStrip, 0);
    horizon.setFillColor({0x22, 0x22, 0x22});
    horizon.setPointCount(0x40);
    window.draw(horizon);
    ball(window, to_orig * game.get_apple(), 0.2, direction, t);
    if (!game.is_growing())
        draw_tail(window, game, direction, t);
    while (!snake.empty()) {
        currp = to_orig * snake.front();
        segment(window, snake_width, snake_color, prevp, currp, direction, t);
        polyline.append({proj_screen(currp, direction, t), {0xff, 0xff, 0xff}});
        prevp = currp;
        snake.pop();
    }
    draw_head(window, game, direction, t);
    polyline.append({sf::Vector2f(400, 400), {0xff, 0xff, 0xff}});
    window.draw(polyline);
}
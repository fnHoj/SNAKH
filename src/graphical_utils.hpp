#pragma once
#include "game.hpp"
#include "gridlines.hpp"
#include <SFML/Graphics.hpp>
using namespace snakh;
using namespace gridlines;
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

void ball(sf::RenderWindow& window, sf::Color color, const Vec& p, double r, unsigned direction = 0, double t = 0, double min_radius = 0) {
    const pair<pair<double, double>, double> pos = proj(p, direction, t);
    double R = 400.0 * r * pos.second;
    if (R <= min_radius)
        R = min_radius;
    sf::CircleShape circ(R);
    circ.setPosition(sf::Vector2f(400 * (1.0 - pos.first.second) - R, 400 * (1.0 - pos.first.first) - R));
    circ.setFillColor(color);
    window.draw(circ);
}
void ball(sf::RenderWindow& window, sf::Color color, double z, double x, double y, double r, unsigned direction = 0, double t = 0, double min_radius = 0) {
    const pair<pair<double, double>, double> pos = proj(z, x, y, direction, t);
    double R = 400.0 * r * pos.second;
    if (R <= min_radius)
        R = min_radius;
    sf::CircleShape circ(R);
    circ.setPosition(sf::Vector2f(400 * (1.0 - pos.first.second) - R, 400 * (1.0 - pos.first.first) - R));
    circ.setFillColor(color);
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
    sf::VertexArray seg(sf::PrimitiveType::TriangleStrip, 0);
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
    Vec p0 = snake.front();
    snake.pop();
    Vec p1 = snake.front();
    p0 *= game.get_to_orig();
    p1 *= game.get_to_orig();
    double z0 = double(p0.z);
    double x0 = SQRT_PHI * double(p0.x);
    double y0 = SQRT_PHI * double(p0.y);
    double z1 = double(p1.z);
    double x1 = SQRT_PHI * double(p1.x);
    double y1 = SQRT_PHI * double(p1.y);
    interpolate(z0, x0, y0, z1, x1, y1, t, z0, x0, y0, (1 - t) * dist);
    ball(window, snake_color, z0, x0, y0, snake_width, direction, t);

    z0 = double(p0.z);
    x0 = SQRT_PHI * double(p0.x);
    y0 = SQRT_PHI * double(p0.y);
    z1 = double(p1.z);
    x1 = SQRT_PHI * double(p1.x);
    y1 = SQRT_PHI * double(p1.y);
    interpolate(z0, x0, y0, z1, x1, y1, t, z0, x0, y0, (1 - t) * dist);
    segment(window, snake_width, snake_color, z0, x0, y0, z1, x1, y1, direction, t);

}
void draw_head(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    const Mat to_orig = game.get_to_orig();
    double R = 400 * snake_width / 2;
    sf::CircleShape cap(R);
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
    cap.setFillColor(snake_color);
    cap.setPosition(sf::Vector2f(400 - R, 400 - R));
    window.draw(cap);
}

void draw_horizon(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    sf::CircleShape horizon(400);
    horizon.setFillColor({0x22, 0x22, 0x22});
    horizon.setPointCount(0x40);
    window.draw(horizon);
}

void draw_apple(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    ball(window, {0xff, 0x00, 0x00}, game.get_to_orig() * game.get_apple(), 0.2, direction, t, 4);
}

void draw_snake(sf::RenderWindow& window, const Game& game, unsigned direction, double t) {
    const Mat to_orig = game.get_to_orig();
    queue<Vec> snake = game.get_snake();
    snake.pop();
    Vec prevp = to_orig * snake.front();
    Vec currp = prevp;
    snake.pop();
    if (!game.is_growing())
        draw_tail(window, game, direction, t);
    ball(window, snake_color, currp, snake_width, direction, t);
    while (!snake.empty()) {
        currp = to_orig * snake.front();
        ball(window, snake_color, currp, snake_width, direction, t);
        segment(window, snake_width, snake_color, prevp, currp, direction, t);
        prevp = currp;
        snake.pop();
    }
    draw_head(window, game, direction, t);
}

const double grid_width = 0.05;
const sf::Color grid_color(0x11, 0x11, 0x11);
void draw_grid(sf::RenderWindow& window, const Game& game, const GridLines& grid, unsigned direction, double t) {
    for (unsigned i = 0; i < grid.m; i++) {
        segment(window, grid_width, grid_color,
            grid.tile[grid.line[i][0]].pos,
            grid.tile[grid.line[i][1]].pos,
        direction, t);
    }
}
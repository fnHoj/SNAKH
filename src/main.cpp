#include "game.hpp"
#include "game_debug.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace snakh;
using namespace game_debug;

int main() {
    GameDebug g;

    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({960u, 540u}), "SNAKH");
    window.setFramerateLimit(144);
    window.setKeyRepeatEnabled(false);

    g.apples();

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->scancode) {
                    case sf::Keyboard::Scancode::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::Scancode::Up:
                        g.move(0);
                        break;
                    case sf::Keyboard::Scancode::Left:
                        g.move(1);
                        break;
                    case sf::Keyboard::Scancode::Down:
                        g.move(2);
                        break;
                    case sf::Keyboard::Scancode::Right:
                        g.move(3);
                        break;
                    default:
                        break;
                }
            }
        }
        window.clear();
        window.display();
    }
}

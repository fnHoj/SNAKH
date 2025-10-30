#include "game.hpp"
#include "render.hpp"
using namespace snakh;

class GamePlatform {
private:
    Game game;
    sf::RenderWindow& window;
    sf::Clock clock;

    static const unsigned T;

    unsigned steptimer;
    unsigned curr_direction, next_direction;

    bool turn_to(unsigned direction) {
        direction &= 3;
        if ((direction ^ curr_direction) == 2)
            return false;
        next_direction = direction;
        return true;
    }

    void motion(const sf::Time& dt) {
        if (!game.is_dead()) {
            steptimer += dt.asMicroseconds();
            while (steptimer >= T && !game.is_dead()) {
                if (game.move(curr_direction)) {
                    curr_direction = next_direction;
                    steptimer -= T;
                }
            }
        }
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->scancode) {
                    case sf::Keyboard::Scancode::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::Scancode::R:
                        restart();
                        break;
                    case sf::Keyboard::Scancode::W:
                    case sf::Keyboard::Scancode::Up:
                        turn_to(0);
                        break;
                    case sf::Keyboard::Scancode::A:
                    case sf::Keyboard::Scancode::Left:
                        turn_to(1);
                        break;
                    case sf::Keyboard::Scancode::S:
                    case sf::Keyboard::Scancode::Down:
                        turn_to(2);
                        break;
                    case sf::Keyboard::Scancode::D:
                    case sf::Keyboard::Scancode::Right:
                        turn_to(3);
                        break;
                    default:
                        break;
                }
            }
        }
    }
public:
    GamePlatform(sf::RenderWindow& window):
            window(window), clock(),
            steptimer(), curr_direction(), next_direction() {
        clock.restart();
    }

    void restart() {
        steptimer = 0;
        curr_direction = 0;
        next_direction = 0;
        clock.restart();
        game = Game();
    }

    void frame() {
        const sf::Time dt = clock.restart();
        motion(dt);
        window.clear();
        render(window, game, curr_direction, double(steptimer) / T);
        window.display();
    }
};

const unsigned GamePlatform::T = 250000;

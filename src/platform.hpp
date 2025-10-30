#include "game.hpp"
#include "gridlines.hpp"
#include "render.hpp"
using namespace snakh;
using namespace gridlines;

class GamePlatform {
private:
    Game game;
    sf::RenderWindow& window;
    sf::Clock clock;
    GridLines grid;

    static const unsigned T;

    unsigned steptimer;
    unsigned curr_direction;
    queue<unsigned> directions;

    inline unsigned first_pending_direction() {
        return directions.empty() ? curr_direction : directions.front();
    }
    unsigned pop_pending_direction() {
        unsigned ans = curr_direction;
        if (!directions.empty()) {
            ans = directions.front();
            directions.pop();
        }
        return ans;
    }
    inline unsigned last_pending_direction() {
        return directions.empty() ? curr_direction : directions.back();
    }
    bool turn_to(unsigned direction) {
        direction &= 3;
        if (!((direction ^ last_pending_direction()) & 1))
            return false;
        directions.push(direction);
        return true;
    }

    void motion(const sf::Time& dt) {
        if (!game.is_dead()) {
            steptimer += dt.asMicroseconds();
            while (steptimer >= T && !game.is_dead()) {
                if (game.move(curr_direction)) {
                    curr_direction = pop_pending_direction();
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
            window(window), clock(), grid(),
            steptimer(), curr_direction(), directions() {
        clock.restart();
    }

    void restart() {
        steptimer = 0;
        curr_direction = 0;
        while (!directions.empty()) {
            directions.pop();
        }
        
        clock.restart();
        game = Game();
    }

    void frame() {
        const sf::Time dt = clock.restart();
        motion(dt);
        window.clear();
        render(window, game, grid, curr_direction, double(steptimer) / T);
        window.display();
    }
};

const unsigned GamePlatform::T = 250000;

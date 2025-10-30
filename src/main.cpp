#include "platform.hpp"
#include <iostream>
using namespace std;


int main() {
    sf::RenderWindow window(sf::VideoMode({800, 800}), "SNAKH");
    window.setFramerateLimit(144);
    window.setKeyRepeatEnabled(false);
    GamePlatform g(window);

    while (window.isOpen())
        g.frame();
    
    return 0;
}

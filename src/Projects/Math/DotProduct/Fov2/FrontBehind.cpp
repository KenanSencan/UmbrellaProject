#include <iostream>
#include "../../HelperClass/MainTemplate.h"

MAIN_TEMPLATE_START
    std::cout << "Hello world";
    sf::CircleShape player = Object::CreateCircleShape({400.f, 300.f}, 20.f, sf::Color::Blue);

    window.draw(player);
MAIN_TEMPLATE_END

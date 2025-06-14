#include "../../../Projects/Math/HelperClass/HelperCollection.h"
#include "CircleDrawable.h"
#include "IDrawable.h"
#include "RectangleDrawable.h"
#include <SFML/Graphics.hpp>
#include <vector>

MAIN_TEMPLATE_GAME_START


std::vector<IDrawable*> drawables;
//any IDrawable inherited object is suitable for here
drawables.push_back(new CircleDrawable{100, 100, 50, sf::Color::Red});
drawables.push_back(new RectangleDrawable{300, 200, 100, 50, sf::Color::Blue});
drawables.push_back(new CircleDrawable{500, 300, 30, sf::Color::Green});

auto* movingCircle = dynamic_cast<CircleDrawable*>(drawables[0]);
auto* Rectangle = dynamic_cast<RectangleDrawable*>(drawables[1]); 

GAME_LOOP_START

if (movingCircle)
{
    movingCircle->SetPosition(sf::Mouse::getPosition(window).x - 50,
                              sf::Mouse::getPosition(window).y - 50);

    // Change color based on mouse button
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        movingCircle->SetColor(sf::Color::Yellow);
    }
    else
    {
        movingCircle->SetColor(sf::Color::Red);
    }
}

// Draw all objects through the interface
for (IDrawable* drawable : drawables)
{
    drawable->Draw(window);
}

DISPLAY_TEXT("Move the red circle with your mouse");
DISPLAY_TEXT("Click to change its color");
DISPLAY_TEXT("Note: All objects are managed through the IDrawable interface");

MAIN_TEMPLATE_GAME_END




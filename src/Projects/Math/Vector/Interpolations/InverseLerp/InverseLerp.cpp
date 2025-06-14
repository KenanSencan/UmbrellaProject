#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <string>

MAIN_TEMPLATE_GAME_START

// Progress bar range
float minValue = 0.0f;
float maxValue = 100.0f;
float currentValue = 25.0f;  // Start at 25%

sf::Vector2f BarSize = {500,40};
sf::Vector2f BarPos = {150,200};

// Progress bar appearance
sf::RectangleShape barBackground({BarSize.x, BarSize.y});
barBackground.setPosition(BarPos.x, BarPos.y);
barBackground.setFillColor(sf::Color(50, 50, 50));
barBackground.setOutlineThickness(2.0f);
barBackground.setOutlineColor(sf::Color(100, 100, 100));

sf::RectangleShape barFill({0.0f, 40.0f});  // Width will be updated
barFill.setPosition(150.0f, 200.0f);
barFill.setFillColor(sf::Color(50, 150, 250));

GAME_LOOP_START

// Process mouse input to adjust current value
if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    
    // Check if mouse is over the progress bar
    if (mousePos.x >= BarPos.x && mousePos.x <= BarPos.x + BarSize.x &&
        mousePos.y >= BarPos.y && mousePos.y <= BarPos.y + BarSize.y) {
        
        // Convert mouse x-position to a value between minValue and maxValue
        //it might be confusing to see `(mousePos.x - 150.0f) / 500.0f`
        //Bar starts at 150 so we subtract 150 from the mouse position firstly
        //then we divide by the width of the bar (500)
        //If you click the right edge of the bar, mousePos will be 650 so result has to be 1 right?
        //650 - 150 = 500. 500/500 = 1. That's it!
        //calculated `currentValue` actually represents the percentage of the bar filled even though we did lerp. 
        currentValue = std::lerp(minValue, maxValue, (mousePos.x - BarPos.x) / BarSize.x);
            
        // Clamp to range (even though not needed) 
        currentValue = std::clamp(currentValue, minValue, maxValue);
    }
}

// Calculate progress percentage using InverseLerp
float progressPercentage = Math::InverseLerp(minValue, maxValue, currentValue);

// Update the fill bar width based on percentage
barFill.setSize({BarSize.x * progressPercentage, BarSize.y});

// Draw everything
window.clear(sf::Color(30, 30, 30));
window.draw(barBackground);
window.draw(barFill);

// Display information
DISPLAY_TEXT("InverseLerp Progress Bar");
DISPLAY_TEXT("Min: " + std::to_string(int(minValue)) + 
             " | Current: " + std::to_string(int(currentValue)) + 
             " | Max: " + std::to_string(int(maxValue)));
DISPLAY_TEXT("Progress: " + std::to_string(int(progressPercentage * 100)) + "%");
DISPLAY_TEXT("Click on the bar to set value");

MAIN_TEMPLATE_GAME_END
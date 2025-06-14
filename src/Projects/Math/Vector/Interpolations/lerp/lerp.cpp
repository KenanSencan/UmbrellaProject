#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <complex>
#include <filesystem>
#include <string>
#include <vector>

sf::Vector2f operator/(const sf::Vector2f& lhs, double rhs)
{
    return {static_cast<float>(lhs.x / rhs), static_cast<float>(lhs.y / rhs)};    
};

MAIN_TEMPLATE_GAME_START

// Configuration
constexpr int NUM_RECTANGLES = 20;
constexpr float RECT_WIDTH = 40.0f;
constexpr float RECT_HEIGHT = 20.0f;
constexpr float ORBIT_RADIUS = 200.0f;

// Create rectangles with different colors along a spectrum
std::vector<sf::RectangleShape> rectangles;

// Create start and end points for the interpolation
sf::Vector2f startPoint(100.0f, 100.0f);
sf::Vector2f endPoint(window.getSize().x - 100.0f, window.getSize().y - 100.0f);

// Create visual markers for start and end points
sf::CircleShape startMarker(10.0f);
startMarker.setFillColor(sf::Color::Green);
startMarker.setPosition(startPoint - sf::Vector2f(10.0f, 10.0f));

sf::CircleShape endMarker(10.0f);
endMarker.setFillColor(sf::Color::Red);
endMarker.setPosition(endPoint - sf::Vector2f(10.0f, 10.0f));

// Initialize rectangles
for (int i = 0; i < NUM_RECTANGLES; i++)
{
    sf::RectangleShape rect(sf::Vector2f(RECT_WIDTH, RECT_HEIGHT));
    
    // Initialize at start position
    rect.setOrigin(RECT_WIDTH / 2.0f, RECT_HEIGHT / 2.0f);
    
    // Color gradient from green to red
    float t = static_cast<float>(i) / (NUM_RECTANGLES - 1);
    sf::Color color(
        static_cast<sf::Uint8>(255 * t),         // R: 0 -> 255
        static_cast<sf::Uint8>(255 * (1.0f - t)), // G: 255 -> 0
        0                                         // B: 0
    );
    rect.setFillColor(color);
    
    rectangles.push_back(rect);
}

float animationTime = 0.0f;
bool pingPong = false;

GAME_LOOP_START

// Update animation time
animationTime += deltaTime * 0.5f;

// Update rectangle positions
for (int i = 0; i < NUM_RECTANGLES; i++)
{
    float t = static_cast<float>(i) / (NUM_RECTANGLES - 1);
    
    // Calculate position along the path with even spacing
    sf::Vector2f lerpPosition = Math::Lerp(startPoint, endPoint, t);
    
    // Add a sine wave offset for wave pattern
    //1. 't' will be different in every object so wave for every object should be unique with `std::sin(t)` right? 
    float waveOffset = 190.0f * std::sin(t * 5 + animationTime * 5);
    sf::Vector2f direction = endPoint - startPoint;
    sf::Vector2f perpendicular(-direction.y, direction.x);
    perpendicular = perpendicular / Math::VectorLength(perpendicular);
    
    lerpPosition += perpendicular * waveOffset;
    
    // Apply rotation based on position
    float angle = 360.0f * t + animationTime * 90.0f;
    
    // Update rectangle
    rectangles[i].setPosition(lerpPosition);
    rectangles[i].setRotation(angle);
}

// Draw start and end markers
window.draw(startMarker);
window.draw(endMarker);

// Draw all rectangles
for (const auto& rect : rectangles)
{
    window.draw(rect);
}

// Draw explanatory text
DISPLAY_TEXT("Linear Interpolation (Lerp) Visualization");
DISPLAY_TEXT("Green: Start Point | Red: End Point");
DISPLAY_TEXT("Rectangles show interpolation with t from 0.0 to 1.0");

MAIN_TEMPLATE_GAME_END
#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <cmath>

MAIN_TEMPLATE_GAME_START

// Configuration
constexpr float CIRCLE_RADIUS = 180.0f;
constexpr int PATH_DOTS = 20;

// Center of the display
sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

// Two test vectors (will be normalized for SLERP/NLERP)
sf::Vector2f startVector(1.0f, 0.3f);    // Pointing right-slightly up
sf::Vector2f endVector(-0.6f, 0.8f);     // Pointing left-up

// Scale original vectors for display positions
sf::Vector2f startPoint = center + startVector * CIRCLE_RADIUS;
sf::Vector2f endPoint = center + endVector * CIRCLE_RADIUS;

// Create visual markers for start and end
sf::CircleShape startMarker(10.0f);
startMarker.setFillColor(sf::Color::Green);
startMarker.setOrigin(10.0f, 10.0f);
startMarker.setPosition(startPoint);

sf::CircleShape endMarker(10.0f);
endMarker.setFillColor(sf::Color::Red);
endMarker.setOrigin(10.0f, 10.0f);
endMarker.setPosition(endPoint);

// Create center marker
sf::CircleShape centerMarker(6.0f);
centerMarker.setFillColor(sf::Color::White);
centerMarker.setOrigin(6.0f, 6.0f);
centerMarker.setPosition(center);

// Create unit circle outline
sf::CircleShape unitCircle(CIRCLE_RADIUS);
unitCircle.setFillColor(sf::Color::Transparent);
unitCircle.setOutlineColor(sf::Color(80, 80, 80, 120));
unitCircle.setOutlineThickness(2.0f);
unitCircle.setOrigin(CIRCLE_RADIUS, CIRCLE_RADIUS);
unitCircle.setPosition(center);

// Animation
float animationTime = 0.0f;

GAME_LOOP_START

// Update animation - oscillate between -1 and 1, then map to 0-1
animationTime += deltaTime;
float rawT = std::sin(animationTime); // -1 to 1
std::cout << "rawT: " << rawT << std::endl;
float t = (rawT + 1.0f) * 0.5f; // 0 to 1

// Calculate all three interpolations
sf::Vector2f lerpResult = Math::Lerp(startVector, endVector, t);
sf::Vector2f nlerpResult = Math::Nlerp(startVector, endVector, t);
sf::Vector2f slerpResult = Math::Slerp(startVector, endVector, t);

// Scale results for display
sf::Vector2f lerpPoint = center + lerpResult * CIRCLE_RADIUS;
sf::Vector2f nlerpPoint = center + nlerpResult * CIRCLE_RADIUS;
sf::Vector2f slerpPoint = center + slerpResult * CIRCLE_RADIUS;

// Create animated dots for current positions
sf::CircleShape lerpDot(8.0f);
lerpDot.setFillColor(sf::Color::Yellow);
lerpDot.setOrigin(8.0f, 8.0f);
lerpDot.setPosition(lerpPoint);

sf::CircleShape nlerpDot(8.0f);
nlerpDot.setFillColor(sf::Color::Magenta);
nlerpDot.setOrigin(8.0f, 8.0f);
nlerpDot.setPosition(nlerpPoint);

sf::CircleShape slerpDot(8.0f);
slerpDot.setFillColor(sf::Color::Cyan);
slerpDot.setOrigin(8.0f, 8.0f);
slerpDot.setPosition(slerpPoint);

// Draw background elements
window.draw(unitCircle);
window.draw(centerMarker);

// Draw path dots to show the different trajectories
for (int i = 0; i <= PATH_DOTS; i++)
{
    float pathT = static_cast<float>(i) / PATH_DOTS;
    
    // LERP path (yellow)
    sf::Vector2f lerpPathPoint = center + Math::Lerp(startVector, endVector, pathT) * CIRCLE_RADIUS;
    sf::CircleShape lerpPathDot(2.0f);
    lerpPathDot.setFillColor(sf::Color(255, 255, 0, 150));
    lerpPathDot.setOrigin(2.0f, 2.0f);
    lerpPathDot.setPosition(lerpPathPoint);
    window.draw(lerpPathDot);
    
    // NLERP path (magenta)
    sf::Vector2f nlerpPathPoint = center + Math::Nlerp(startVector, endVector, pathT) * CIRCLE_RADIUS;
    sf::CircleShape nlerpPathDot(2.0f);
    nlerpPathDot.setFillColor(sf::Color(255, 0, 255, 150));
    nlerpPathDot.setOrigin(2.0f, 2.0f);
    nlerpPathDot.setPosition(nlerpPathPoint);
    window.draw(nlerpPathDot);
    
    // SLERP path (cyan)
    sf::Vector2f slerpPathPoint = center + Math::Slerp(startVector, endVector, pathT) * CIRCLE_RADIUS;
    sf::CircleShape slerpPathDot(2.0f);
    slerpPathDot.setFillColor(sf::Color(0, 255, 255, 150));
    slerpPathDot.setOrigin(2.0f, 2.0f);
    slerpPathDot.setPosition(slerpPathPoint);
    window.draw(slerpPathDot);
}

// Draw vectors from center to current positions
sf::Vertex lerpVector[] = {
    sf::Vertex(center, sf::Color::Yellow),
    sf::Vertex(lerpPoint, sf::Color::Yellow)
};
window.draw(lerpVector, 2, sf::Lines);

sf::Vertex nlerpVector[] = {
    sf::Vertex(center, sf::Color::Magenta),
    sf::Vertex(nlerpPoint, sf::Color::Magenta)
};
window.draw(nlerpVector, 2, sf::Lines);

sf::Vertex slerpVector[] = {
    sf::Vertex(center, sf::Color::Cyan),
    sf::Vertex(slerpPoint, sf::Color::Cyan)
};
window.draw(slerpVector, 2, sf::Lines);

// Draw start and end markers on top
window.draw(startMarker);
window.draw(endMarker);

// Draw animated dots on top
window.draw(lerpDot);
window.draw(nlerpDot);
window.draw(slerpDot);

// Display information
DISPLAY_TEXT("LERP vs NLERP vs SLERP Comparison");
DISPLAY_TEXT("Green: Start Vector | Red: End Vector");
DISPLAY_TEXT("Yellow: LERP (straight line, variable length)");
DISPLAY_TEXT("Magenta: NLERP (normalized, uneven arc speed)");
DISPLAY_TEXT("Cyan: SLERP (perfect arc, constant speed)");
DISPLAY_TEXT("Notice: LERP goes inside circle, NLERP/SLERP stay on circle");

MAIN_TEMPLATE_GAME_END
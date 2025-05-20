#include "../../../HelperClass/HelperCollection.h" // Math::Normalize, Math::Dot, etc.
#include <SFML/Graphics.hpp>
#include <iostream>

sf::Vector2f SurfacePos{960.f, 540.f}; // centre of the surface
sf::Vector2f SurfaceNormal{0.f, -1.f}; // surface faces “up”
sf::Color BaseColour{255, 180, 60};    // warm diffuse colour
float Radius = 60.f;

MAIN_TEMPLATE_GAME_START

sf::CircleShape surface(Radius);
surface.setOrigin(Radius, Radius);
surface.setPosition(SurfacePos);

GAME_LOOP_START

// 1. Light direction = vector from surface to mouse
const sf::Vector2f lightDir = Math::Normalize(sf::Vector2f(sf::Mouse::getPosition(window)) - SurfacePos);

// 2. Diffuse intensity = dot(normal, lightDir) clamped to [0,1]
float intensity = std::max(0.f, Math::Dot(SurfaceNormal, lightDir));

// 3. Modulate base colour by intensity
surface.setFillColor(sf::Color(
    static_cast<sf::Uint8>(BaseColour.r * intensity),
    static_cast<sf::Uint8>(BaseColour.g * intensity),
    static_cast<sf::Uint8>(BaseColour.b * intensity)));

// draw
window.draw(surface);

// debug text
DISPLAY_TEXT("Normal:  (" + std::to_string(SurfaceNormal.x) + ", " + std::to_string(SurfaceNormal.y) + ")");
DISPLAY_TEXT("Light dir:  (" + std::to_string(lightDir.x) + ", " + std::to_string(lightDir.y) + ")");
DISPLAY_TEXT("Intensity:  " + std::to_string(intensity));
DISPLAY_TEXT("Move mouse around the disc.  Up-facing normal = green line.");
DISPLAY_TEXT("Yellow line is light direction");
DISPLAY_TEXT("Green line is normal direction");
DISPLAY_TEXT("The the intensity will increase if the `lightDir` is close to `SurfaceNormal` "
             "and decrease if the `lightDir` is far from `SurfaceNormal`");

// helper lines for visualisation
sf::VertexArray lightLine(sf::Lines, 2);
lightLine[0].position = SurfacePos;
lightLine[1].position = SurfacePos + lightDir * 120.f;
lightLine[0].color = lightLine[1].color = sf::Color::Yellow;
window.draw(lightLine);

sf::VertexArray normalLine(sf::Lines, 2);
normalLine[0].position = SurfacePos;
normalLine[1].position = SurfacePos + SurfaceNormal * 120.f;
normalLine[0].color = normalLine[1].color = sf::Color::Green;
window.draw(normalLine);

MAIN_TEMPLATE_GAME_END

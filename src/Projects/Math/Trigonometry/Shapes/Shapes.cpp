#include "../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <string>

sf::Vector2f CirclePos{};
float CircleRadius = 100.0f;

void DrawCircle(sf::RenderWindow& window)
{
    sf::VertexArray filledCircle(sf::LinesStrip);
    filledCircle.append(sf::Vertex(CirclePos, sf::Color::White));

    for (float i = 0.0f; i <= std::numbers::pi * 2; i += 0.01f) // Using smaller step for smoother curve
    {
        sf::Vector2f currVec = {std::cos(i), std::sin(i)};
        sf::Vertex point(CirclePos + currVec * CircleRadius, sf::Color::White); // Add color
        filledCircle.append(point);
    }

    // Draw the points inside the game loop
    window.draw(filledCircle);
}
MAIN_TEMPLATE_GAME_START

CirclePos = {window.getSize().x / 2.0f, window.getSize().y / 2.0f};

GAME_LOOP_START

DrawCircle(window);

//Draw triangle
sf::Vector2f EqTrianglePos = CirclePos - sf::Vector2f{300.0f, 0};
sf::Vector2f A = EqTrianglePos - sf::Vector2f{100.0f, 0};
sf::Vector2f B = EqTrianglePos +  sf::Vector2f{100.0f, 0};
sf::Vector2f C = Math::RotatePointAroundPoint(B,60 ,A);

sf::VertexArray filledTriangle(sf::TriangleFan);
filledTriangle.append(sf::Vertex(A, sf::Color::White));
filledTriangle.append(sf::Vertex(B, sf::Color::White));
filledTriangle.append(sf::Vertex(C, sf::Color::White));

sf::CircleShape pointA(5.0f);
pointA.setFillColor(sf::Color::Green);
pointA.setOrigin(5.0f, 5.0f);
pointA.setPosition(EqTrianglePos);
window.draw(pointA);

window.draw(filledTriangle);

MAIN_TEMPLATE_GAME_END
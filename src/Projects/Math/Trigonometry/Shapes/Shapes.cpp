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

// Draw triangle
{
    sf::Vector2f EqTrianglePos = CirclePos - sf::Vector2f{300.0f, 0};
    sf::Vector2f A = EqTrianglePos - sf::Vector2f{100.0f, 0};
    sf::Vector2f B = EqTrianglePos + sf::Vector2f{100.0f, 0};
    sf::Vector2f C = Math::RotateAroundPoint(B, 60, A);
    sf::Vector2f D = Math::RotateAroundPoint(C, 60, B);

    sf::VertexArray Triangle(sf::LinesStrip);
    Triangle.append(sf::Vertex(A, sf::Color::White));
    Triangle.append(sf::Vertex(B, sf::Color::White));
    Triangle.append(sf::Vertex(C, sf::Color::White));
    Triangle.append(sf::Vertex(D, sf::Color::White));

    window.draw(Triangle);

    // Triangle origin
    sf::CircleShape TriOrigin(5.0f);
    TriOrigin.setFillColor(sf::Color::Green);
    TriOrigin.setOrigin(5.0f, 5.0f);
    TriOrigin.setPosition(EqTrianglePos);
    window.draw(TriOrigin);
}

// pentagon and star
{
    sf::Vector2f PentagonOr = CirclePos + sf::Vector2f{300.0f, 0};
    sf::Vector2f B = PentagonOr + sf::Vector2f{100.0f, 0};
    sf::Vector2f C = Math::RotateAroundPoint(B, 72.0f, PentagonOr);
    sf::Vector2f D = Math::RotateAroundPoint(C, 72.0f, PentagonOr);
    sf::Vector2f E = Math::RotateAroundPoint(D, 72.0f, PentagonOr);
    sf::Vector2f F = Math::RotateAroundPoint(E, 72.0f, PentagonOr);
    sf::Vector2f G = Math::RotateAroundPoint(F, 72.0f, PentagonOr);

    sf::VertexArray Pentagon(sf::LinesStrip);
    Pentagon.append(sf::Vertex(B, sf::Color::White));
    Pentagon.append(sf::Vertex(C, sf::Color::White));
    Pentagon.append(sf::Vertex(D, sf::Color::White));
    Pentagon.append(sf::Vertex(E, sf::Color::White));
    Pentagon.append(sf::Vertex(F, sf::Color::White));
    Pentagon.append(sf::Vertex(G, sf::Color::White));

    window.draw(Pentagon);
}

{
    sf::Vector2f StarOr = CirclePos + sf::Vector2f{600.0f, 0};

    sf::Vector2f B = StarOr + sf::Vector2f{100.0f, 0};
    sf::Vector2f C = Math::RotateAroundPoint(B, 72.0f, StarOr);
    sf::Vector2f D = Math::RotateAroundPoint(C, 72.0f, StarOr);
    sf::Vector2f E = Math::RotateAroundPoint(D, 72.0f, StarOr);
    sf::Vector2f F = Math::RotateAroundPoint(E, 72.0f, StarOr);
    sf::Vector2f G = Math::RotateAroundPoint(F, 72.0f, StarOr);

    sf::VertexArray Star(sf::LineStrip);
    Star.append(sf::Vertex(B, sf::Color::White));
    Star.append(sf::Vertex(D, sf::Color::White));
    Star.append(sf::Vertex(F, sf::Color::White));
    Star.append(sf::Vertex(C, sf::Color::White));
    Star.append(sf::Vertex(E, sf::Color::White));
    Star.append(sf::Vertex(G, sf::Color::White));
    Star.append(sf::Vertex(B, sf::Color::White)); // Close the star

    window.draw(Star);
}

MAIN_TEMPLATE_GAME_END
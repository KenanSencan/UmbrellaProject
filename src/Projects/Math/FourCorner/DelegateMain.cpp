#include <filesystem>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>

int ArbitOrigin = 50;
// Structure to store rectangle information
struct Rectangle
{
    sf::Vector2f position;
    sf::Vector2f size;

    // Calculate the 4 corners of the rectangle
    [[nodiscard]] std::vector<sf::Vector2f> getCorners() const
    {
        return {
            {position.x - ArbitOrigin, position.y - ArbitOrigin}, // Top-left
            {position.x - ArbitOrigin + size.x, position.y - ArbitOrigin}, // Top-right
            {position.x - ArbitOrigin, position.y + size.y - ArbitOrigin}, // Bottom-left
            {position.x + size.x - ArbitOrigin, position.y + size.y - ArbitOrigin} // Bottom-right
        };
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangle Corners Example");
    window.setFramerateLimit(60);

    // Create a rectangle
    Rectangle rect{sf::Vector2f(200.f, 150.f), sf::Vector2f(400.f, 300.f)};

    // Create the visual representation of the rectangle
    sf::RectangleShape rectShape(rect.size);
    rectShape.setPosition(rect.position);
    rectShape.setFillColor(sf::Color(100, 150, 250, 128)); // Semi-transparent blue
    rectShape.setOutlineThickness(2.f);
    rectShape.setOrigin(50,50);
    rectShape.setOutlineColor(sf::Color::White);


    // Calculate corners
    std::vector<sf::Vector2f> corners = rect.getCorners();
    std::vector<std::string> cornerLabels = {"Top-Left", "Top-Right", "Bottom-Left", "Bottom-Right"};
    std::vector<sf::Text> cornerTexts;

    // Create visual markers for the corners
    std::vector<sf::CircleShape> cornerMarkers;
    for (const auto& corner : corners)
    {
        sf::CircleShape marker(5.f);
        marker.setOrigin(5.f, 5.f);
        marker.setPosition(corner);
        marker.setFillColor(sf::Color::Red);
        cornerMarkers.push_back(marker);
    }

    // Print corners to console
    std::cout << "Rectangle corners:" << std::endl;
    for (size_t i = 0; i < corners.size(); i++)
    {
        std::cout << cornerLabels[i] << ": (" << corners[i].x << ", " << corners[i].y << ")" << std::endl;
    }

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(30, 30, 30));

        // Draw the rectangle
        window.draw(rectShape);

        // Draw the corner markers
        for (const auto& marker : cornerMarkers)
        {
            window.draw(marker);
        }

        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

// Simple 2x2 matrix class for rotation
class Matrix2x2 {
public:
    float m00, m01, m10, m11;

    // Initialize identity matrix by default
    Matrix2x2() : m00(1), m01(0), m10(0), m11(1) {}

    // Create rotation matrix
    static Matrix2x2 MakeRotation(float angleRadians) {
        Matrix2x2 mat;
        mat.m00 = cos(angleRadians);
        mat.m01 = -sin(angleRadians);
        mat.m10 = sin(angleRadians);
        mat.m11 = cos(angleRadians);
        return mat;
    }

    // Transform vector to matrix
    sf::Vector2f transform(const sf::Vector2f& v) const {
        return {
            m00 * v.x + m01 * v.y,
            m10 * v.x + m11 * v.y
        };
    }

    // Get a string representation of the matrix
    std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "[" << m00 << ", " << m01 << "]\n";
        ss << "[" << m10 << ", " << m11 << "]";
        return ss.str();
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Vector Rotation with Matrices");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        std::cout << "Couldn't load font. Using fallback method." << std::endl;
    }

    // Setup text
    sf::Text infoText("", font, 18);
    infoText.setFillColor(sf::Color::White);

    // Player setup
    sf::Vector2f playerPos(400.f, 300.f);        // Center of the window
    sf::CircleShape player(20.f);              
    player.setFillColor(sf::Color::Blue);
    player.setOrigin(20.f, 20.f);
    player.setPosition(playerPos);

    // Direction vector initially points right (1,0)
    sf::Vector2f direction(1.f, 0.f);
    
    // Current angle for tracking
    float currentAngle = 0.0f;
    
    // Rotation speed in radians per frame when a key is pressed
    const float rotationSpeed = 0.05f;

    // Main loop
    while (window.isOpen())
    {
        sf::Event evt{};
        while (window.pollEvent(evt))
        {
            if (evt.type == sf::Event::Closed)
                window.close();
        }

        // Check for rotation input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            // Clockwise rotation
            currentAngle -= rotationSpeed;
            
            // Create a rotation matrix
            Matrix2x2 rotMat = Matrix2x2::MakeRotation(-rotationSpeed);
            
            // Apply the rotation
            direction = rotMat.transform(direction);
            
            // Normalize to prevent drift
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction.x /= length;
            direction.y /= length;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            // Counter-clockwise rotation
            currentAngle += rotationSpeed;
            
            // Create a rotation matrix
            Matrix2x2 rotMat = Matrix2x2::MakeRotation(rotationSpeed);
            
            // Apply the rotation
            direction = rotMat.transform(direction);
            
            // Normalize to prevent drift
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction.x /= length;
            direction.y /= length;
        }

        // Clear screen
        window.clear(sf::Color(30, 30, 30));

        // Draw player
        window.draw(player);

        // Draw direction arrow
        sf::VertexArray directionArrow(sf::Lines, 2);
        directionArrow[0].position = playerPos;
        directionArrow[1].position = playerPos + (direction * 100.f);
        directionArrow[0].color = sf::Color::Yellow;
        directionArrow[1].color = sf::Color::Yellow;
        window.draw(directionArrow);

        // Draw rotation matrix
        Matrix2x2 currentRotMatrix = Matrix2x2::MakeRotation(currentAngle);

        // Display information texts
        window.draw(infoText);

        infoText.setString("Direction Vector: (" + 
                          std::to_string(direction.x) + ", " + 
                          std::to_string(direction.y) + ")");
        infoText.setPosition(10.f, 80.f);
        window.draw(infoText);

        infoText.setString("Current Angle: " + 
                          std::to_string(currentAngle) + " radians");
        infoText.setPosition(10.f, 105.f);
        window.draw(infoText);

        infoText.setString("Current Rotation Matrix:");
        infoText.setPosition(10.f, 130.f);
        window.draw(infoText);

        infoText.setString(currentRotMatrix.toString());
        infoText.setPosition(30.f, 155.f);
        window.draw(infoText);

        infoText.setString("Press Q to rotate counter-clockwise");
        infoText.setPosition(10.f, 210.f);
        window.draw(infoText);

        infoText.setString("Press E to rotate clockwise");
        infoText.setPosition(10.f, 235.f);
        window.draw(infoText);

        window.display();
    }

    return 0;
}
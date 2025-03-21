#include <SFML/Graphics.hpp>
#include <cmath> // For distance calculation
#include <functional> // For delegates

// Delegate type definition for item interaction

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangle Interaction Example");
    window.setFramerateLimit(60);

    // Hero rectangle. This will be moved with keys
    sf::RectangleShape hero;
    hero.setSize(sf::Vector2f(50, 50));
    hero.setFillColor(sf::Color::Blue);
    hero.setPosition(400, 300); // Start in the middle of the screen
    constexpr float interactRange = 80.0f;
    float heroSpeed = 4.0f;

    // Pickable item rectangle. This item will be pickable
    sf::RectangleShape item;
    item.setSize(sf::Vector2f(30, 30));
    item.setFillColor(sf::Color::Green);
    item.setPosition(200, 200);

    // Interaction range
    bool inRange = false;

    // Text to display when in range
    sf::Font font;
    font.loadFromFile("../SegoeUI.ttf");
    sf::Text interactionText;
    interactionText.setFont(font);
    interactionText.setString("Press 'E' to interact");
    interactionText.setCharacterSize(18);
    interactionText.setFillColor(sf::Color::White);
    interactionText.setPosition(320, 100);

    // Create interaction delegate
    std::function<void()> onInteract = [&hero]()
    {
        // Change the hero's color when interaction is triggered
        hero.setFillColor(sf::Color::Red);
    };

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            // Check for E key press to start interaction logic
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
            {
                if (inRange)
                {
                    // Call the delegate function
                    onInteract();
                }
            }
        }

        // Hero movement with keyboard
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) hero.move(-heroSpeed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) hero.move(heroSpeed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) hero.move(0, -heroSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) hero.move(0, heroSpeed);

        // Calculate distance between hero and item
        sf::Vector2f heroCenter = hero.getPosition() + sf::Vector2f(hero.getSize().x / 2, hero.getSize().y / 2);
        sf::Vector2f itemCenter = item.getPosition() + sf::Vector2f(item.getSize().x / 2, item.getSize().y / 2);

        float distanceX = heroCenter.x - itemCenter.x;
        float distanceY = heroCenter.y - itemCenter.y;
        float distanceLength = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        // Check if hero is in range of the item
        inRange = (distanceLength < interactRange);

        // Visual feedback for when item is in range
        if (inRange)
        {
            item.setOutlineThickness(2);
            item.setOutlineColor(sf::Color::Yellow);
        }
        else
        {
            item.setOutlineThickness(0);
        }

        window.clear(sf::Color(30, 30, 30));

        // always Draw shapes
        window.draw(hero);
        window.draw(item);

        // Draw interaction text if in range
        if (inRange && font.getInfo().family != "")
        {
            window.draw(interactionText);
        }

        window.display();
    }

    return 0;
}

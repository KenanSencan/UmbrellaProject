#include <SFML/Graphics.hpp>
#include <cmath>
#include <filesystem>
#include <string>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dot Product - Front/Behind Example");
    window.setFramerateLimit(60);

    // For labeling the “in front” / “behind” text
    sf::Font font;
    if (!font.loadFromFile((std::filesystem::current_path().parent_path().parent_path().parent_path().parent_path() / "ETG" / "Resources" / "Fonts" / "SegoeUI.ttf").string())) return EXIT_FAILURE;
    sf::Text infoText("", font, 18);
    infoText.setFillColor(sf::Color::White);

    // Player setup
    sf::Vector2f playerPos(400.f, 300.f);        // Center of the window
    sf::CircleShape player(20.f);                // A circle to represent the player
    player.setFillColor(sf::Color::Blue);
    player.setOrigin(20.f, 20.f);
    player.setPosition(playerPos);

    // The player faces to the right (1,0) as the forward direction
    const sf::Vector2f playerForward(1.f, 0.f);

    while (window.isOpen())
    {
        sf::Event evt{};
        while (window.pollEvent(evt))
        {
            if (evt.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear(sf::Color(30, 30, 30));

        // Enemy is at mouse position
        sf::Vector2f enemyPos = sf::Vector2f(sf::Mouse::getPosition(window));

        // Vector from player to enemy
        sf::Vector2f playerToEnemyRelative = enemyPos - playerPos;

        // Compute the dot product
        float dot = (playerForward.x * playerToEnemyRelative.x) + (playerForward.y * playerToEnemyRelative.y);

        // Determine if the enemy is in front or behind
        // Positive dot => front, Negative dot => behind
        std::string message = (dot >= 0.f) ? "ENEMY IS IN FRONT" : "ENEMY IS BEHIND";

        // Update/draw player
        window.draw(player);

        // Draw forward direction arrow 
        sf::VertexArray forwardArrow(sf::Lines, 2);

        forwardArrow[0].position = playerPos; // tail at player's center
        forwardArrow[1].position = playerPos + (playerForward * 100.f); // 100 units ahead
        forwardArrow[0].color = sf::Color::Yellow;
        forwardArrow[1].color = sf::Color::Yellow;
        window.draw(forwardArrow);

        // Draw the enemy (mouse position) as a red circle
        sf::CircleShape enemy(10.f);
        enemy.setFillColor(sf::Color::Red);
        enemy.setOrigin(10.f, 10.f);
        enemy.setPosition(enemyPos);
        window.draw(enemy);

        //NOTE: TEXT Drawing
        infoText.setString("Enemy Mouse Position: " + std::to_string(enemyPos.x) + " " + std::to_string(enemyPos.y));
        infoText.setPosition(0.f, 0.f);
        window.draw(infoText);

        infoText.setString("playerToEnemy: " + std::to_string(playerToEnemyRelative.x) + " " + std::to_string(playerToEnemyRelative.y));
        infoText.setPosition(0.f, 25.f);
        window.draw(infoText);

        infoText.setString("Dot Product: " + std::to_string(dot));
        infoText.setPosition(0.f, 50.f);
        window.draw(infoText);

        infoText.setString(message);
        infoText.setPosition(0.f, 75.f);
        window.draw(infoText);

        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>
#include <cmath>

bool isWithinFieldOfView(const sf::Vector2f& enemyPos,
                         const sf::Vector2f& enemyForward,
                         const sf::Vector2f& playerPos,
                         const float fovAngleDegrees)
{
    // Vector from the enemy to the player
    const sf::Vector2f toPlayer = playerPos - enemyPos;

    // If the player is very close to the enemy, we might consider them 'seen' automatically
    float distanceSquared = toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y;
    if (distanceSquared < 1.f)
        return true;

    // Normalize the vectors (so their length is 1)
    auto normalize = [](const sf::Vector2f& v)
    {
        const float len = std::sqrt(v.x * v.x + v.y * v.y);
        return (len > 0.f) ? sf::Vector2f(v.x / len, v.y / len) : sf::Vector2f(0.f, 0.f);
    };

    sf::Vector2f forwardNorm = normalize(enemyForward);
    sf::Vector2f toPlayerNorm = normalize(toPlayer);

    // Dot product of normalized vectors gives the cosine of the angle between them
    const float dot = (forwardNorm.x * toPlayerNorm.x + forwardNorm.y * toPlayerNorm.y);

    // Convert FOV angle to a cosine. For example, if FOV is 60°, cos(60°) = 0.5
    float cosFOV = std::cos(fovAngleDegrees * 3.1415926535f / 180.f);

    // If dot >= cosFOV, the player is inside that field of view
    // Also ensure dot > 0 to confirm player is in front (not behind)
    return (dot >= cosFOV && dot > 0.f);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dot Product FOV Example");
    window.setFramerateLimit(60);

    // Enemy setup
    sf::Vector2f enemyPos(400, 300);
    sf::Vector2f enemyForward(1.f, 0.f);
    const float enemyFOV = 60.f;
    const float fovRadius = 200.0f; // Radius for FOV visualization

    sf::CircleShape enemyShape(15);
    enemyShape.setOrigin(15, 15);
    enemyShape.setFillColor(sf::Color::Red);
    enemyShape.setPosition(enemyPos);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        sf::Vector2f playerPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool canSeePlayer = isWithinFieldOfView(enemyPos, enemyForward, playerPos, enemyFOV);

        // Draw FOV visualization
        float enemyAngle = std::atan2(enemyForward.y, enemyForward.x) * 180.0f / 3.1415926535f;
        float halfFOV = enemyFOV / 2.0f;
        int numSegments = 30;

        sf::VertexArray fovVisual(sf::TriangleFan, numSegments + 2);
        fovVisual[0] = sf::Vertex(enemyPos, sf::Color(255, 255, 0, 50));

        for (int i = 0; i <= numSegments; ++i)
        {
            float angle = (enemyAngle - halfFOV) + (enemyFOV * i) / numSegments;
            float radian = angle * 3.1415926535f / 180.0f;
            sf::Vector2f point(
                enemyPos.x + std::cos(radian) * fovRadius,
                enemyPos.y + std::sin(radian) * fovRadius
            );
            fovVisual[i + 1] = sf::Vertex(point, sf::Color(255, 255, 0, 50));
        }

        window.draw(fovVisual);
        window.draw(enemyShape);

        // Draw direction line
        sf::Vertex directionLine[] = {
            sf::Vertex(enemyPos, sf::Color::Yellow),
            sf::Vertex(enemyPos + enemyForward * 50.f, sf::Color::Yellow)
        };
        window.draw(directionLine, 2, sf::Lines);

        // Draw player
        sf::CircleShape playerShape(5);
        playerShape.setOrigin(5, 5);
        playerShape.setFillColor(canSeePlayer ? sf::Color::Green : sf::Color::Blue);
        playerShape.setPosition(playerPos);
        window.draw(playerShape);

        window.display();
    }

    return 0;
}

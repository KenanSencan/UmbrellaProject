#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <string>

// Global variables for collision visualization
sf::Vector2f currentCollisionNormal;
bool isColliding = false;

// Ball class
class Ball
{
  public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius;
    sf::Color color;
    float mass;

    Ball(sf::Vector2f pos, sf::Vector2f vel, float r, sf::Color col, float m = 1.0f) : position(pos), velocity(vel), radius(r), color(col), mass(m)
    {
    }

    void Update(float deltaTime)
    {
        position += velocity * deltaTime;
    }

    void Draw(sf::RenderWindow& window) const
    {
        sf::CircleShape shape(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(position);
        shape.setFillColor(color);

        // Draw velocity vector
        sf::VertexArray velocityLine(sf::Lines, 2);
        velocityLine[0].position = position;
        velocityLine[1].position = position + velocity * 0.3f;
        velocityLine[0].color = sf::Color(255, 255, 255, 100);
        velocityLine[1].color = sf::Color(255, 255, 255, 100);

        sf::VertexArray CollNorm(sf::Lines, 2);
        CollNorm[0].position = position;
        CollNorm[1].position = position + currentCollisionNormal * 250.f;
        CollNorm[0].color = sf::Color(sf::Color::Red);
        CollNorm[1].color = sf::Color(sf::Color::Red);

        window.draw(shape);
        window.draw(velocityLine);
        window.draw(CollNorm);
    }

    void BounceOffWalls(float width, float height)
    {
        // Left wall
        if (position.x - radius < 0)
        {
            position.x = radius;
            velocity = Math::VectorReflection(velocity, sf::Vector2f(1.f, 0.f)); // Normal points right
        }
        // Right wall
        if (position.x + radius > width)
        {
            position.x = width - radius;
            velocity = Math::VectorReflection(velocity, sf::Vector2f(-1.f, 0.f)); // Normal points left
        }

        // Top wall
        if (position.y - radius < 0)
        {
            position.y = radius;
            velocity = Math::VectorReflection(velocity, sf::Vector2f(0.f, 1.f)); // Normal points down
        }
        // Bottom wall
        if (position.y + radius > height)
        {
            position.y = height - radius;
            velocity = Math::VectorReflection(velocity, sf::Vector2f(0.f, -1.f)); // Normal points up
        }
    }
};

// Check if two balls are colliding
bool BallsColliding(const Ball& ball1, const Ball& ball2)
{
    float distance = Math::Length(ball2.position - ball1.position);
    return distance < (ball1.radius + ball2.radius);
}

// Handle collision between two balls - Simple reflection version
void HandleBallCollisionSimple(Ball& ball1, Ball& ball2)
{
    // Calculate collision normal (from ball1 to ball2)
    sf::Vector2f collisionNormal = ball2.position - ball1.position;
    float distance = Math::Length(collisionNormal);

    // Prevent division by zero
    if (distance == 0.0f)
    {
        ball2.position.x += 1.0f;
        distance = 1.0f;
        collisionNormal = sf::Vector2f(1.0f, 0.0f);
    }
    else
    {
        collisionNormal = Math::Normalize(collisionNormal);
    }

    // Store for visualization
    currentCollisionNormal = collisionNormal;
    isColliding = true;

    // Separate balls to prevent overlap
    float overlap = (ball1.radius + ball2.radius) - distance;
    if (overlap > 0)
    {
        ball1.position -= collisionNormal * (overlap * 0.5f);
        ball2.position += collisionNormal * (overlap * 0.5f);
    }

    // Simple reflection approach - treat each ball as bouncing off a wall
    // Ball1 sees ball2 as a wall with normal pointing towards ball2
    // Ball2 sees ball1 as a wall with normal pointing towards ball1 (opposite)
    ball1.velocity = Math::VectorReflection(ball1.velocity, collisionNormal);
    ball2.velocity = Math::VectorReflection(ball2.velocity, -collisionNormal);
}

// Handle collision between two balls - Physics accurate version
void HandleBallCollisionPhysics(Ball& ball1, Ball& ball2)
{
    // Calculate collision normal (from ball1 to ball2)
    sf::Vector2f collisionNormal = ball2.position - ball1.position;
    float distance = Math::Length(collisionNormal);

    // Prevent division by zero
    if (distance == 0.0f)
    {
        ball2.position.x += 1.0f;
        distance = 1.0f;
        collisionNormal = sf::Vector2f(1.0f, 0.0f);
    }
    else
    {
        collisionNormal = Math::Normalize(collisionNormal);
    }

    // Store for visualization
    currentCollisionNormal = collisionNormal;
    isColliding = true;

    // Separate balls to prevent overlap
    float overlap = (ball1.radius + ball2.radius) - distance;
    if (overlap > 0)
    {
        float totalMass = ball1.mass + ball2.mass;
        float push1 = (overlap * ball2.mass / totalMass);
        float push2 = (overlap * ball1.mass / totalMass);

        ball1.position -= collisionNormal * push1;
        ball2.position += collisionNormal * push2;
    }

    // Calculate relative velocity
    sf::Vector2f relativeVelocity = ball1.velocity - ball2.velocity;

    // Calculate relative velocity along collision normal
    float velocityAlongNormal = Math::Dot(relativeVelocity, collisionNormal);

    // Don't resolve if velocities are separating
    if (velocityAlongNormal > 0)
        return;

    // Calculate restitution (bounciness)
    float restitution = 0.9f;

    // Calculate impulse scalar
    float impulse = 2 * velocityAlongNormal / (ball1.mass + ball2.mass);

    // Apply impulse to velocities
    ball1.velocity -= impulse * ball2.mass * collisionNormal * restitution;
    ball2.velocity += impulse * ball1.mass * collisionNormal * restitution;
}

// Draw collision normal visualization
void DrawCollisionNormal(sf::RenderWindow& window, const Ball& ball1, const Ball& ball2, const sf::Vector2f& normal)
{
    float normalLength = 200.0f;

    // Draw normal from ball1 (pointing towards ball2)
    sf::VertexArray normal1(sf::Lines, 2);
    normal1[0].position = ball1.position;
    normal1[1].position = ball1.position + normal * normalLength;
    normal1[0].color = sf::Color::Green;
    normal1[1].color = sf::Color::Green;
    window.draw(normal1);

    // Draw arrow head for ball1 normal
    sf::Vector2f arrowEnd1 = ball1.position + normal * normalLength;
    sf::Vector2f arrowPerp(-normal.y, normal.x);
    sf::VertexArray arrow1(sf::Triangles, 3);
    arrow1[0].position = arrowEnd1;
    arrow1[1].position = arrowEnd1 - normal * 10.0f + arrowPerp * 5.0f;
    arrow1[2].position = arrowEnd1 - normal * 10.0f - arrowPerp * 5.0f;
    arrow1[0].color = arrow1[1].color = arrow1[2].color = sf::Color::Green;
    window.draw(arrow1);

    // Draw normal from ball2 (pointing away from ball1, so negative)
    sf::VertexArray normal2(sf::Lines, 2);
    normal2[0].position = ball2.position;
    normal2[1].position = ball2.position - normal * normalLength;
    normal2[0].color = sf::Color::Red;
    normal2[1].color = sf::Color::Red;
    window.draw(normal2);

    // Draw arrow head for ball2 normal
    sf::Vector2f arrowEnd2 = ball2.position - normal * normalLength;
    sf::VertexArray arrow2(sf::Triangles, 3);
    arrow2[0].position = arrowEnd2;
    arrow2[1].position = arrowEnd2 + normal * 10.0f + arrowPerp * 5.0f;
    arrow2[2].position = arrowEnd2 + normal * 10.0f - arrowPerp * 5.0f;
    arrow2[0].color = arrow2[1].color = arrow2[2].color = sf::Color::Red;
    window.draw(arrow2);

    // Draw connecting line between balls
    sf::VertexArray connection(sf::Lines, 2);
    connection[0].position = ball1.position;
    connection[1].position = ball2.position;
    connection[0].color = sf::Color(255, 255, 255, 50);
    connection[1].color = sf::Color(255, 255, 255, 50);
    window.draw(connection);
}

MAIN_TEMPLATE_GAME_START

// Create smaller window
window.create(sf::VideoMode(800, 600), "Ball Collision - Using Math::VectorReflection");
window.setFramerateLimit(170);

// Create two balls with different properties
Ball ball1(sf::Vector2f(200.f, 300.f),  // Position
           sf::Vector2f(475.f, -150.f), // Velocity
           30.f,                        // Radius
           sf::Color::Cyan,             // Color
           1.0f                         // Mass
);

Ball ball2(sf::Vector2f(600.f, 300.f),  // Position
           sf::Vector2f(-100.f, -30.f), // Velocity
           40.f,                        // Radius
           sf::Color::Magenta,          // Color
           1.5f                         // Mass (heavier)
);

// Collision state tracking
bool wasColliding = false;
int collisionCount = 0;
bool useSimpleReflection = false; // Toggle between simple and physics-based

GAME_LOOP_START

// Toggle collision mode with Space
if (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Space)
{
    useSimpleReflection = !useSimpleReflection;
}

// Update balls
ball1.Update(deltaTime);
ball2.Update(deltaTime);

// BounceOffWalls using VectorReflection
ball1.BounceOffWalls(window.getSize().x, window.getSize().y);
ball2.BounceOffWalls(window.getSize().x, window.getSize().y);

// Reset collision flag
isColliding = false;

// Check ball-to-ball collision
if (BallsColliding(ball1, ball2))
{
    if (!wasColliding)
    {
        collisionCount++;
        wasColliding = true;
    }
    
    // Use selected collision method
    if (useSimpleReflection)
    {
        HandleBallCollisionSimple(ball1, ball2);
    }
    else
    {
        HandleBallCollisionPhysics(ball1, ball2);
    }
}
else
{
    wasColliding = false;
}

// Draw everything
ball1.Draw(window);
ball2.Draw(window);

// Draw collision visualization
if (isColliding)
{
    DrawCollisionNormal(window, ball1, ball2, currentCollisionNormal);

    sf::Vector2f midpoint = (ball1.position + ball2.position) * 0.5f;
    sf::CircleShape collisionMarker(10.f);
    collisionMarker.setOrigin(10.f, 10.f);
    collisionMarker.setPosition(midpoint);
    collisionMarker.setFillColor(sf::Color(255, 255, 0, 150));
    window.draw(collisionMarker);
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
{
    ball1.velocity *= 0.99f; // Increase speed
}

else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
{
    ball1.velocity /= 0.99f; // Decrease speed
}

// Display information
DISPLAY_TEXT("Ball Collision Using Math::VectorReflection");
DISPLAY_TEXT("Mode: " + std::string(useSimpleReflection ? "Simple Reflection" : "Physics-Based"));
DISPLAY_TEXT("Press SPACE to toggle collision mode");
DISPLAY_TEXT("");
DISPLAY_TEXT("Cyan Ball: Mass = " + std::to_string(ball1.mass) + ", Velocity = (" + std::to_string(int(ball1.velocity.x)) + ", " + std::to_string(int(ball1.velocity.y)) + ")");
DISPLAY_TEXT("Magenta Ball: Mass = " + std::to_string(ball2.mass) + ", Velocity = (" + std::to_string(int(ball2.velocity.x)) + ", " + std::to_string(int(ball2.velocity.y)) + ")");
DISPLAY_TEXT("Collision Count: " + std::to_string(collisionCount));

if (useSimpleReflection)
{
    DISPLAY_TEXT("");
    DISPLAY_TEXT("Simple Reflection Mode:");
    DISPLAY_TEXT("- Uses Math::VectorReflection(velocity, normal)");
    DISPLAY_TEXT("- Each ball reflects as if hitting a wall");
    DISPLAY_TEXT("- Mass is ignored");
}
else
{
    DISPLAY_TEXT("");
    DISPLAY_TEXT("Physics-Based Mode:");
    DISPLAY_TEXT("- Considers mass and momentum");
    DISPLAY_TEXT("- More realistic collision response");
}

// Show collision normal values when colliding
if (isColliding)
{
    DISPLAY_TEXT("");
    DISPLAY_TEXT("Collision Normal: (" + std::to_string(currentCollisionNormal.x) + ", " + std::to_string(currentCollisionNormal.y) + ")");
}

// Show energy conservation
float totalEnergy = 0.5f * ball1.mass * Math::LengthSquared(ball1.velocity) + 0.5f * ball2.mass * Math::LengthSquared(ball2.velocity);
DISPLAY_TEXT("Total Kinetic Energy: " + std::to_string(int(totalEnergy)));

MAIN_TEMPLATE_GAME_END
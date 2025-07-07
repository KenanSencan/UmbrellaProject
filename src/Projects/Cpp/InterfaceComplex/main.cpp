#include "../../../Projects/Math/HelperClass/HelperCollection.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Player.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <memory>
#include <typeinfo>
#include <vector>

MAIN_TEMPLATE_GAME_START

// Using smart pointers for automatic memory management
std::vector<std::unique_ptr<GameObject>> gameObjects;
std::vector<std::unique_ptr<Projectile>> projectiles;

// Create player
gameObjects.push_back(std::make_unique<Player>(960.f, 540.f));

// Get reference to player for input handling
Player* player = dynamic_cast<Player*>(gameObjects[0].get());

// Spawn timer for enemies
float enemySpawnTimer = 0.f;

GAME_LOOP_START

// Handle input
if (player && !player->ShouldBeRemoved())
{
    sf::Vector2f inputVel(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        inputVel.y -= 300.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        inputVel.y += 300.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        inputVel.x -= 300.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        inputVel.x += 300.f;
    player->SetVelocity(inputVel);

    // Shoot projectiles with mouse
    static bool mouseWasPressed = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseWasPressed)
    {
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        sf::Vector2f dir = Math::Normalize(mousePos - player->GetPosition());
        projectiles.push_back(std::make_unique<Projectile>(player->GetPosition().x, player->GetPosition().y, dir * 500.f));
    }
    mouseWasPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

// Spawn enemies
enemySpawnTimer += deltaTime;
if (enemySpawnTimer > 1.f)
{
    enemySpawnTimer = 0.f;
    float x = static_cast<float>(rand() % 1920);
    float y = static_cast<float>(rand() % 1080);
    gameObjects.push_back(std::make_unique<Enemy>(x, y));
}

// Update all game objects
for (auto& obj : gameObjects)
{
    obj->Update(deltaTime);
}
for (auto& proj : projectiles)
{
    proj->Update(deltaTime);
}

// Check collisions between collidable objects
for (size_t i = 0; i < gameObjects.size(); ++i)
{
    for (size_t j = i + 1; j < gameObjects.size(); ++j)
    {
        if (gameObjects[i]->IsCollidingWith(gameObjects[j].get()))
        {
            gameObjects[i]->OnCollision(gameObjects[j].get());
            gameObjects[j]->OnCollision(gameObjects[i].get());
        }
    }
}

// Remove dead objects
std::erase_if(gameObjects, [](const std::unique_ptr<GameObject>& obj) { return obj->ShouldBeRemoved(); });
std::erase_if(projectiles, [](const std::unique_ptr<Projectile>& proj) { return proj->ShouldBeRemoved(); });

// Draw everything
for (const auto& obj : gameObjects)
{
    obj->Draw(window);
}
for (const auto& proj : projectiles)
{
    proj->Draw(window);
}

// UI
DISPLAY_TEXT("Advanced Interface Example");
DISPLAY_TEXT("WASD to move, Click to shoot");
DISPLAY_TEXT("Game Objects: " + std::to_string(gameObjects.size()));
DISPLAY_TEXT("Projectiles: " + std::to_string(projectiles.size()));

if (player && !player->ShouldBeRemoved())
{
    DISPLAY_TEXT("Player Health: " + std::to_string(player->GetHealth()));
}
else
{
    DISPLAY_TEXT("GAME OVER!");
}

// Demonstrate interface queries
DISPLAY_TEXT("\nInterface Demonstration:");
for (const auto& obj : gameObjects)
{
    std::string info = "Object: ";

    // Check which interfaces are implemented
    if (dynamic_cast<const IMovable*>(obj.get()))
        info += "[Movable] ";
    if (dynamic_cast<const ICollidable*>(obj.get()))
        info += "[Collidable] ";
    if (dynamic_cast<const IUpdateable*>(obj.get()))
        info += "[Updateable] ";
    if (dynamic_cast<const IDrawable*>(obj.get()))
        info += "[Drawable] ";

    DISPLAY_TEXT(info);
}

MAIN_TEMPLATE_GAME_END
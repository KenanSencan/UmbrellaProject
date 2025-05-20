#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

sf::Vector2f HeroDirection(1.f, 0.f);
sf::Vector2f HeroToEnemyDir{};
constexpr float rotationSpeed = 0.05f;

sf::CircleShape hero;
sf::CircleShape enemy;

float preciseness = 0.9971f; // 45 degrees
float LooknessDot;

bool MakeLookAtEnemyTrigger()
{
    HeroToEnemyDir = Math::Normalize(enemy.getPosition() - hero.getPosition());
    LooknessDot = Math::Dot(HeroToEnemyDir, HeroDirection);
    return LooknessDot >= preciseness;
}

void DrawHeroDirLine(sf::RenderWindow& window)
{
    sf::VertexArray forwardArrow(sf::Lines, 2);
    forwardArrow[0].position = hero.getPosition();
    forwardArrow[1].position = hero.getPosition() + (HeroDirection * 100.f);
    window.draw(forwardArrow);
}

void DrawHeroToEnemyDir(sf::RenderWindow& window)
{
    sf::VertexArray forwardArrow(sf::Lines, 2);
    forwardArrow[0].position = hero.getPosition();
    forwardArrow[1].position = hero.getPosition() + (HeroToEnemyDir * 100.f);
    forwardArrow[0].color = sf::Color::Yellow;
    forwardArrow[1].color = sf::Color::Yellow;
    window.draw(forwardArrow);
}

MAIN_TEMPLATE_GAME_START

hero = Object::CreateCircleShape({400.f, 300.f}, 20.f, sf::Color::Blue);
enemy = Object::CreateCircleShape(sf::Vector2f(sf::Mouse::getPosition(window)), 5.f, sf::Color::Red);

GAME_LOOP_START

enemy.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));

if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
{
    HeroDirection = Math::RotateVector(rotationSpeed, HeroDirection);
    HeroDirection = Math::Normalize(HeroDirection);
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
{
    HeroDirection = Math::RotateVector(-rotationSpeed, HeroDirection);
    HeroDirection = Math::Normalize(HeroDirection);
}

DrawHeroDirLine(window);
DrawHeroToEnemyDir(window);

if (MakeLookAtEnemyTrigger())
{
    enemy.setFillColor(sf::Color::Green);
    DISPLAY_TEXT("Hero looking at enemy.");
}
else
{
    enemy.setFillColor(sf::Color::Red);
    DISPLAY_TEXT("Hero NOT looking at enemy.");
}

window.draw(enemy);
window.draw(hero);

DISPLAY_TEXT("Enemy is mouse if you wondering that");
DISPLAY_TEXT("Yellow line is hero to enemy direction");
DISPLAY_TEXT("preciseness: " + std::to_string(preciseness) + " If dot product >= than " + std::to_string(preciseness) + " then it's looking.");
DISPLAY_TEXT("It's not that hard dot calculated with: Dot(HeroToEnemyDir, HeroDirection)")
DISPLAY_TEXT("Then LooknessDot >= preciseness ")
DISPLAY_TEXT("Hero Direction: " + std::to_string(HeroDirection.x) + " " + std::to_string(HeroDirection.y));
DISPLAY_TEXT("Hero to Enemy: " + std::to_string(HeroToEnemyDir.x) + " " + std::to_string(HeroToEnemyDir.y));
DISPLAY_TEXT("Lookness Dot product: " + std::to_string(LooknessDot));


MAIN_TEMPLATE_GAME_END
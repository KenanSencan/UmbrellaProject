#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>

// Enemy class with simple health and a multicast delegate for when damage is taken.
class Enemy
{
    int health = 100;

public:
    // Multicast delegate: subscribers will be notified whenever damage is taken.
    std::vector<std::function<void(int)>> OnDamageTaken;

    void takeDamage(const int damage)
    {
        health -= damage;
        std::cout << "Enemy took " << damage << " damage, remaining health: " << health << std::endl;
        // Notify all subscribers.
        for (auto& callback : OnDamageTaken)
        {
            callback(damage);
        }
    }
};

// Hero class with a delegate for its attack action.
class Hero
{
public:
    // Delegate for attack action. It will be assigned a function that processes the attack.
    std::function<void(int)> OnAttack;

    // When hero attacks, we invoke the delegate.
    void attack(const int damage) const
    {
        if (OnAttack)
        {
            OnAttack(damage);
        }
        else
        {
            std::cout << "No attack delegate assigned!" << std::endl;
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Delegate Example");
    window.setFramerateLimit(60);

    Enemy enemy;
    Hero hero;

    // Assign delegate using a lambda capturing the enemy instance.
    hero.OnAttack = [&enemy](const int damage)
    {
        enemy.takeDamage(damage);
    };

    // Subscribe an additional callback to the enemy's multicast delegate.
    enemy.OnDamageTaken.emplace_back([](const int dmg)
    {
        std::cout << "Additional callback: Enemy was hit for " << dmg << " damage." << std::endl;
    });

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                // Press E to perform an attack (using the current delegate).
                if (event.key.code == sf::Keyboard::E)
                {
                    hero.attack(10);
                }
                // Press D to reassign the delegate using std::bind.
                else if (event.key.code == sf::Keyboard::D)
                {
                    hero.OnAttack = std::bind(&Enemy::takeDamage, &enemy, std::placeholders::_1);
                    std::cout << "Delegate reassigned using std::bind." << std::endl;
                }
                // Press R to clear additional callbacks from the enemy's multicast delegate.
                else if (event.key.code == sf::Keyboard::R)
                {
                    enemy.OnDamageTaken.clear();
                    std::cout << "Cleared enemy's multicast delegate callbacks." << std::endl;
                }
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}

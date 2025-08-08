#include "../../../HelperClass/HelperCollection.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <list>
#include <memory>
#include <string>

// Custom Single Linked List Implementation
template <typename T> class SingleLinkedList
{
  private:
    struct Node
    {
        T data;
        std::unique_ptr<Node> next;

        Node(const T& value) : data(value), next(nullptr)
        {
        }
    };

    std::unique_ptr<Node> head;
    size_t size = 0;

  public:
    void push_front(const T& value)
    {
        auto newNode = std::make_unique<Node>(value);
        newNode->next = std::move(head);
        head = std::move(newNode);
        size++;
    }

    void remove_if(std::function<bool(const T&)> predicate)
    {
        // Remove from front
        while (head && predicate(head->data))
        {
            head = std::move(head->next);
            size--;
        }

        // Remove from middle/end
        if (head)
        {
            Node* current = head.get();
            while (current->next)
            {
                if (predicate(current->next->data))
                {
                    current->next = std::move(current->next->next);
                    size--;
                }
                else
                {
                    current = current->next.get();
                }
            }
        }
    }

    void for_each(std::function<void(T&)> func)
    {
        Node* current = head.get();
        while (current)
        {
            func(current->data);
            current = current->next.get();
        }
    }

    void for_each_const(std::function<void(const T&)> func) const
    {
        Node* current = head.get();
        while (current)
        {
            func(current->data);
            current = current->next.get();
        }
    }

    size_t getSize() const
    {
        return size;
    }
    void clear()
    {
        head.reset();
        size = 0;
    }
};

// Trail Point structure
struct TrailPoint
{
    sf::Vector2f position;
    float lifetime;
    float maxLifetime;

    TrailPoint(sf::Vector2f pos, float maxLife) : position(pos), lifetime(maxLife), maxLifetime(maxLife)
    {
    }
};

// Trail System using custom linked list
class TrailSystemCustom
{
  private:
    SingleLinkedList<TrailPoint> trail;
    float spawnTimer = 0.0f;
    const float spawnDelay = 0.02f; // Spawn every 20ms

  public:
    void update(float deltaTime, const sf::Vector2f& mousePos)
    {
        // Update existing points
        trail.for_each([deltaTime](TrailPoint& point) { point.lifetime -= deltaTime; });

        // Remove dead points (O(n) but efficient for linked list)
        trail.remove_if([](const TrailPoint& point) { return point.lifetime <= 0.0f; });

        // Add new points
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnDelay)
        {
            trail.push_front(TrailPoint(mousePos, 1.0f)); // 1 second lifetime
            spawnTimer = 0.0f;
        }
    }

    void draw(sf::RenderWindow& window) const
    {
        trail.for_each_const([&window](const TrailPoint& point) {
            float alpha = point.lifetime / point.maxLifetime;
            float radius = 5.0f + (1.0f - alpha) * 15.0f; // Expand as it fades

            sf::CircleShape circle(radius);
            circle.setOrigin(radius, radius);
            circle.setPosition(point.position);

            sf::Color color = sf::Color::Cyan;
            color.a = static_cast<sf::Uint8>(alpha * 255);
            circle.setFillColor(color);

            window.draw(circle);
        });
    }

    size_t getSize() const
    {
        return trail.getSize();
    }
};

// Trail System using std::list
class TrailSystemSTL
{
  private:
    std::list<TrailPoint> trail;
    float spawnTimer = 0.0f;
    const float spawnDelay = 0.02f;

  public:
    void update(float deltaTime, const sf::Vector2f& mousePos)
    {
        // Update existing points
        for (auto& point : trail)
        {
            point.lifetime -= deltaTime;
        }

        // Remove dead points (efficient with std::list)
        trail.remove_if([](const TrailPoint& point) { return point.lifetime <= 0.0f; });

        // Add new points
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnDelay)
        {
            trail.push_front(TrailPoint(mousePos, 1.0f));
            spawnTimer = 0.0f;
        }
    }

    void draw(sf::RenderWindow& window) const
    {
        for (const auto& point : trail)
        {
            float alpha = point.lifetime / point.maxLifetime;
            float radius = 5.0f + (1.0f - alpha) * 10.0f;

            sf::CircleShape circle(radius);
            circle.setOrigin(radius, radius);
            circle.setPosition(point.position);

            sf::Color color = sf::Color::Magenta;
            color.a = static_cast<sf::Uint8>(alpha * 255);
            circle.setFillColor(color);

            window.draw(circle);
        }
    }

    size_t getSize() const
    {
        return trail.size();
    }
};

// Projectile System - Another great use case for linked lists
struct Projectile
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime;

    Projectile(sf::Vector2f pos, sf::Vector2f vel) : position(pos), velocity(vel), lifetime(2.0f)
    {
    }
};

class ProjectileSystem
{
  private:
    std::list<Projectile> projectiles;
    float fireTimer = 0.0f;
    const float fireRate = 0.1f; // 10 shots per second

  public:
    void update(float deltaTime, bool isFiring, const sf::Vector2f& origin, const sf::Vector2f& target)
    {
        // Update projectiles
        for (auto& proj : projectiles)
        {
            proj.position += proj.velocity * deltaTime;
            proj.lifetime -= deltaTime;
        }

        // Remove expired projectiles
        projectiles.remove_if([](const Projectile& proj) { return proj.lifetime <= 0.0f; });

        // Fire new projectiles
        fireTimer += deltaTime;
        if (isFiring && fireTimer >= fireRate)
        {
            sf::Vector2f direction = Math::Normalize(target - origin);
            float speed = 500.0f;
            projectiles.push_back(Projectile(origin, direction * speed));
            fireTimer = 0.0f;
        }
    }

    void draw(sf::RenderWindow& window) const
    {
        for (const auto& proj : projectiles)
        {
            sf::CircleShape bullet(3.0f);
            bullet.setOrigin(3.0f, 3.0f);
            bullet.setPosition(proj.position);
            bullet.setFillColor(sf::Color::Yellow);
            window.draw(bullet);
        }
    }

    size_t getSize() const
    {
        return projectiles.size();
    }
};

MAIN_TEMPLATE_GAME_START

// Initialize systems
TrailSystemCustom customTrail;
TrailSystemSTL stlTrail;
ProjectileSystem projectiles;

sf::Clock deltaClock;
sf::Vector2f mouseWorldPos;

// UI Text
sf::Font font;
if (!font.loadFromFile(findFontPath()))
{
    std::cerr << "Failed to load font!" << std::endl;
}

GAME_LOOP_START

// Get mouse position
mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

// Update systems
customTrail.update(deltaTime, mouseWorldPos);
stlTrail.update(deltaTime, mouseWorldPos + sf::Vector2f(0, 50)); // Offset for visibility

// Update projectiles (fire towards center when left mouse is pressed)
bool isFiring = sf::Mouse::isButtonPressed(sf::Mouse::Left);
sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
projectiles.update(deltaTime, isFiring, mouseWorldPos, center);

// Draw everything
customTrail.draw(window);
stlTrail.draw(window);
projectiles.draw(window);

// Draw center target
sf::CircleShape target(10.0f);
target.setOrigin(10.0f, 10.0f);
target.setPosition(center);
target.setFillColor(sf::Color::Red);
window.draw(target);

// Update and draw info text
std::string info = "Move mouse to create trails\n";
info += "Left click to fire projectiles\n\n";
info += "Custom Linked List (Cyan): " + std::to_string(customTrail.getSize()) + " points\n";
info += "STL List (Magenta): " + std::to_string(stlTrail.getSize()) + " points\n";
info += "Projectiles: " + std::to_string(projectiles.getSize()) + " active\n\n";
info += "Why Linked Lists?\n";
info += "- Constant time insertion/deletion\n";
info += "- No memory reallocation\n";
info += "- Perfect for dynamic collections";
infoText.setString(info);
window.draw(infoText);

MAIN_TEMPLATE_GAME_END
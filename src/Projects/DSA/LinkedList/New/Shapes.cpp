#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <list>          // Doubly-linked list
#include <forward_list>  // Singly-linked list
#include <string>

// Trail Point structure
struct TrailPoint {
    sf::Vector2f position;
    float lifetime;
    float maxLifetime;
    sf::Color baseColor;
    
    TrailPoint(sf::Vector2f pos, float maxLife, sf::Color color) : position(pos), lifetime(maxLife), maxLifetime(maxLife), baseColor(color) {}
};

// Trail System using std::forward_list (Singly-linked)
class TrailSystemForwardList {
private:
    std::forward_list<TrailPoint> trail;
    float spawnTimer = 0.0f;
    const float spawnDelay = 0.02f;
    size_t count = 0;  // Manual count since forward_list doesn't have size()
    
public:
    void update(float deltaTime, const sf::Vector2f& mousePos) {
        // Update existing points
        for (auto& point : trail) {
            point.lifetime -= deltaTime;
        }
        
        // Remove dead points - forward_list requires special handling
        size_t removed = 0;
        trail.remove_if([&removed](const TrailPoint& point) {
            if (point.lifetime <= 0.0f) {
                removed++;
                return true;
            }
            return false;
        });
        count -= removed;
        
        // Add new points at front (most efficient for forward_list)
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnDelay) {
            trail.push_front(TrailPoint(mousePos, 1.0f, sf::Color::Cyan));
            spawnTimer = 0.0f;
            count++;
        }
    }
    
    void draw(sf::RenderWindow& window) const {
        for (const auto& point : trail) {
            float alpha = point.lifetime / point.maxLifetime;
            float radius = 5.0f + (1.0f - alpha) * 15.0f;
            
            sf::CircleShape circle(radius);
            circle.setOrigin(radius, radius);
            circle.setPosition(point.position);
            
            sf::Color color = point.baseColor;
            color.a = static_cast<sf::Uint8>(alpha * 255);
            circle.setFillColor(color);
            
            window.draw(circle);
        }
    }
    
    size_t getSize() const { return count; }
};

// Trail System using std::list (Doubly-linked)
class TrailSystemList {
private:
    std::list<TrailPoint> trail;
    float spawnTimer = 0.0f;
    const float spawnDelay = 0.02f;
    
public:
    void update(float deltaTime, const sf::Vector2f& mousePos) {
        // Update existing points
        for (auto& point : trail) {
            point.lifetime -= deltaTime;
        }
        
        // Remove dead points (can iterate both directions)
        trail.remove_if([](const TrailPoint& point) {
            return point.lifetime <= 0.0f;
        });
        
        // Add new points (can add at front or back efficiently)
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnDelay) {
            // Alternate between front and back insertion to show capability
            if (trail.size() % 2 == 0) {
                trail.push_front(TrailPoint(mousePos, 1.0f, sf::Color::Magenta));
            } else {
                trail.push_back(TrailPoint(mousePos, 1.0f, sf::Color::Yellow));
            }
            spawnTimer = 0.0f;
        }
    }
    
    void draw(sf::RenderWindow& window) const {
        // Can iterate forward or backward
        for (const auto& point : trail) {
            float alpha = point.lifetime / point.maxLifetime;
            float radius = 5.0f + (1.0f - alpha) * 10.0f;
            
            sf::CircleShape circle(radius);
            circle.setOrigin(radius, radius);
            circle.setPosition(point.position);
            
            sf::Color color = point.baseColor;
            color.a = static_cast<sf::Uint8>(alpha * 255);
            circle.setFillColor(color);
            
            window.draw(circle);
        }
    }
    
    size_t getSize() const { return trail.size(); }
};

// Enemy wave system - Shows splice() advantage of std::list
struct Enemy {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float health;
    bool isElite;
    
    Enemy(sf::Vector2f pos, bool elite = false) 
        : position(pos), health(elite ? 100.0f : 50.0f), isElite(elite) {
        velocity = Math::Normalize(sf::Vector2f(rand() % 200 - 100, rand() % 200 - 100)) * 50.0f;
    }
};

class EnemyWaveSystem {
private:
    std::list<Enemy> activeEnemies;
    std::list<Enemy> incomingWave;
    float waveTimer = 0.0f;
    const float waveDelay = 3.0f;
    
public:
    void update(float deltaTime, sf::Vector2f playerPos) {
        // Update active enemies
        for (auto& enemy : activeEnemies) {
            // Simple AI: move towards player
            sf::Vector2f toPlayer = Math::Normalize(playerPos - enemy.position);
            enemy.velocity = enemy.velocity * 0.95f + toPlayer * 100.0f * 0.05f;
            enemy.position += enemy.velocity * deltaTime;
            
            // Simulate damage
            enemy.health -= deltaTime * 10.0f;
        }
        
        // Remove dead enemies
        activeEnemies.remove_if([](const Enemy& enemy) {
            return enemy.health <= 0.0f;
        });
        
        // Spawn waves
        waveTimer += deltaTime;
        if (waveTimer >= waveDelay) {
            // Prepare next wave
            for (int i = 0; i < 5; ++i) {
                float angle = (i / 5.0f) * 2 * std::numbers::pi;
                sf::Vector2f spawnPos = playerPos + Math::vectorFromAngle(angle) * 300.0f;
                incomingWave.push_back(Enemy(spawnPos, i == 0)); // First is elite
            }
            
            // Efficiently move entire wave to active list using splice
            activeEnemies.splice(activeEnemies.end(), incomingWave);
            waveTimer = 0.0f;
        }
    }
    
    void draw(sf::RenderWindow& window) const {
        for (const auto& enemy : activeEnemies) {
            float radius = enemy.isElite ? 15.0f : 10.0f;
            sf::CircleShape shape(radius);
            shape.setOrigin(radius, radius);
            shape.setPosition(enemy.position);
            
            // Health-based color
            float healthPercent = enemy.health / (enemy.isElite ? 100.0f : 50.0f);
            sf::Color color = enemy.isElite ? sf::Color::Red : sf::Color::Green;
            color.a = static_cast<sf::Uint8>(healthPercent * 255);
            shape.setFillColor(color);
            
            window.draw(shape);
        }
    }
    
    size_t getSize() const { return activeEnemies.size(); }
};

MAIN_TEMPLATE_GAME_START

// Initialize systems
TrailSystemForwardList forwardListTrail;
TrailSystemList listTrail;
EnemyWaveSystem enemies;

sf::Clock deltaClock;
sf::Vector2f mouseWorldPos;

// Player representation
sf::CircleShape player(8.0f);
player.setOrigin(8.0f, 8.0f);
player.setFillColor(sf::Color::White);

// UI Text
sf::Font font;
if (!font.loadFromFile(findFontPath())) {
    std::cerr << "Failed to load font!" << std::endl;
}


GAME_LOOP_START

// Get mouse position
mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

// Update player
player.setPosition(mouseWorldPos);

// Update systems
forwardListTrail.update(deltaTime, mouseWorldPos);
listTrail.update(deltaTime, mouseWorldPos + sf::Vector2f(0, 50)); // Offset

enemies.update(deltaTime, mouseWorldPos);

// Draw everything
forwardListTrail.draw(window);
listTrail.draw(window);
enemies.draw(window);
window.draw(player);

// Update and draw info text
std::string info = "=== C++ Standard Linked Lists Demo ===\n\n";
info += "std::forward_list (Singly-linked):\n";
info += "  - Cyan trail: " + std::to_string(forwardListTrail.getSize()) + " points\n";
info += "  - Memory: ~40% less than std::list\n";
info += "  - Operations: push_front only\n\n";

info += "std::list (Doubly-linked):\n";
info += "  - Magenta/Yellow trail: " + std::to_string(listTrail.getSize()) + " points\n";
info += "  - Can traverse both directions\n";
info += "  - Operations: push_front, push_back, splice\n\n";

info += "Enemy Wave System (std::list):\n";
info += "  - Active enemies: " + std::to_string(enemies.getSize()) + "\n";
info += "  - Uses splice() to move entire waves\n\n";

info += "Key Differences:\n";
info += "• forward_list: Less memory, forward-only\n";
info += "• list: Bidirectional, more features\n";
info += "• Both: O(1) insertion/removal";

infoText.setString(info);
window.draw(infoText);

MAIN_TEMPLATE_GAME_END
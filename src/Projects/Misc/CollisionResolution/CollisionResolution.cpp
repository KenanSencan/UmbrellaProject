#include <SFML/Graphics.hpp>
#include <functional>

sf::FloatRect GetBounds(const sf::Texture& tex, const sf::Vector2f pos)
{
    return {
        pos.x - tex.getSize().x / 2.f, //X coordinate of the top-left corner
        pos.y - tex.getSize().y / 2.f, //Y coordinate of the top-left corner
        static_cast<float>(tex.getSize().x), //usual width size
        static_cast<float>(tex.getSize().y), //usual height size
    };
}

class Hero
{
public:
    // Delegate for collision handling
    float radius = 25.f; // expands collision box
    std::function<void()> OnCollision;
    sf::Texture Texture;
    sf::Vector2f Pos;
    sf::Color Color = sf::Color::White;
    float Rotation{};
    sf::FloatRect GetBounds() const { return ::GetBounds(Texture, Pos); }


    sf::Sprite CreateSprite() const
    {
        sf::Sprite sprite(Texture);
        sprite.setOrigin(Texture.getSize().x / 2.f, Texture.getSize().y / 2.f);
        sprite.setPosition(Pos);
        sprite.setRotation(Rotation);
        sprite.setColor(Color);
        return sprite;
    }
};

class Enemy
{
public:
    float radius = 10.f;
    sf::Texture Texture;
    sf::Vector2f Pos;
    sf::Color Color = sf::Color::White;
    float Rotation{};
    sf::FloatRect GetBounds() const { return ::GetBounds(Texture, Pos); }


    sf::Sprite CreateSprite() const
    {
        sf::Sprite sprite(Texture);
        sprite.setOrigin(Texture.getSize().x / 2.f, Texture.getSize().y / 2.f);
        sprite.setPosition(Pos);
        sprite.setRotation(Rotation);
        sprite.setColor(Color);
        return sprite;
    }
};

void DrawExtendedBounds(sf::RenderWindow& window, const sf::FloatRect& rect, const float radius, const sf::Color color)
{
    // Draw the original rectangle bounds
    sf::RectangleShape originalRect; //Providing just (X: left, Y: Top) pos and width, Height enough to draw a rectangle. 
    originalRect.setPosition(rect.left, rect.top);
    originalRect.setSize(sf::Vector2f(rect.width, rect.height));
    originalRect.setFillColor(sf::Color::Transparent);
    originalRect.setOutlineColor(sf::Color::White);
    originalRect.setOutlineThickness(1.0f);
    window.draw(originalRect);

    // Draw the extended bounds rectangle
    sf::RectangleShape extendedRect;
    extendedRect.setPosition(rect.left - radius, rect.top - radius);
    extendedRect.setSize(sf::Vector2f(rect.width + (2 * radius), rect.height + (2 * radius))); //Multiplied by 2 just because it's radius
    extendedRect.setFillColor(sf::Color::Transparent);
    extendedRect.setOutlineColor(color);
    extendedRect.setOutlineThickness(1.0f);
    window.draw(extendedRect);
}

// Updated collision function that considers both entities' radii
bool CheckCollision(const sf::FloatRect& rect1, const float radius1, const sf::FloatRect& rect2, const float radius2)
{
    // Create expanded rectangles for both entities
    const sf::FloatRect expandedRect1 = {
        rect1.left - radius1,
        rect1.top - radius1,
        rect1.width + (2 * radius1),
        rect1.height + (2 * radius1)
    };

    const sf::FloatRect expandedRect2 = {
        rect2.left - radius2,
        rect2.top - radius2,
        rect2.width + (2 * radius2),
        rect2.height + (2 * radius2)
    };

    // Check if the original rectangles intersect (this is a quick early-out check)
    if (rect1.intersects(rect2))
    {
        return true; // The actual sprites are overlapping
    }

    // Check if the expanded rectangles intersect
    return expandedRect1.intersects(expandedRect2);

    // Alternative computation: we could also check if either expanded rectangle 
    // intersects with the other's original rectangle
    // return expandedRect1.intersects(rect2) || rect1.intersects(expandedRect2);
}

int main()
{
    sf::RenderWindow window{sf::VideoMode(800, 600), "Hero vs Enemy Collision"};
    window.setFramerateLimit(60);

    // Load a texture for hero and enemy (use your own images here)
    sf::Texture heroTexture, enemyTexture;

    sf::RenderTexture HerorenderTex;
    HerorenderTex.create(75, 40);
    HerorenderTex.clear(sf::Color::Green);
    HerorenderTex.display();

    sf::RenderTexture EnemyRenderTex;
    EnemyRenderTex.create(100, 100);
    EnemyRenderTex.clear(sf::Color::Cyan);
    EnemyRenderTex.display();

    heroTexture = HerorenderTex.getTexture();
    enemyTexture = EnemyRenderTex.getTexture();

    // Create Hero
    Hero hero;
    hero.Texture = (heroTexture);
    hero.Pos = {100.f, 300.f};

    // Create Enemy
    Enemy enemy;
    enemy.Texture = (enemyTexture);
    enemy.Pos = {500.f, 300.f};

    // Load font
    sf::Font font;
    font.loadFromFile("../SegoeUI.ttf");

    // Collision text
    sf::Text collisionText;
    collisionText.setFont(font);
    collisionText.setString("");
    collisionText.setCharacterSize(30);
    collisionText.setFillColor(sf::Color::Red);
    collisionText.setPosition(300.f, 50.f);

    // Assign the collision delegate to the hero
    hero.OnCollision = [&hero, &collisionText]()
    {
        // Change hero color
        hero.Color = (sf::Color::Red);
        // Show collision text
        collisionText.setString("Collision!");
    };

    // Rectangle shapes for drawing bounding boxes
    sf::RectangleShape heroBoundsRect, enemyBoundsRect;
    heroBoundsRect.setFillColor(sf::Color::Transparent);
    heroBoundsRect.setOutlineColor(sf::Color::Green);
    heroBoundsRect.setOutlineThickness(1.f);

    enemyBoundsRect.setFillColor(sf::Color::Transparent);
    enemyBoundsRect.setOutlineColor(sf::Color::Blue);
    enemyBoundsRect.setOutlineThickness(1.f);

    bool isColliding = false;
    bool itemPickUp = false; //If colliding we can enable item pick up 

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
            {
                if (isColliding && !itemPickUp)
                {
                    itemPickUp = true;
                    collisionText.setString("Item picked up!");
                    // You could change the enemy's visibility or remove it here
                    enemy.Color = sf::Color(255, 255, 255, 100); // Semi-transparent
                }
            }
        }

        // Simple movement for hero (WASD)
        const float speed = 200.f;
        float dt = 1.f / 60.f; // since we use a fixed framerate limit
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) hero.Pos += {0, -speed * dt};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) hero.Pos += {0, speed * dt};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) hero.Pos += {-speed * dt, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) hero.Pos += {speed * dt, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) hero.Rotation += {1.f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) hero.Rotation += {-1.f};

        //Get bounds for collision check
        sf::FloatRect heroBounds = hero.GetBounds();
        sf::FloatRect enemyBounds = enemy.GetBounds();

        // Check collision with both radii
        if (!itemPickUp && CheckCollision(heroBounds, hero.radius, enemyBounds, enemy.radius))
        {
            if (!isColliding)
            {
                // Only call OnCollision once each time we start colliding
                isColliding = true;
                hero.OnCollision();
            }
        }
        else if (!itemPickUp)
        {
            // Not colliding now
            isColliding = false;
            hero.Color = (sf::Color::White);
            collisionText.setString("");
        }

        // Update bounding box shapes
        heroBoundsRect.setPosition(heroBounds.left, heroBounds.top);
        heroBoundsRect.setSize({heroBounds.width, heroBounds.height});

        enemyBoundsRect.setPosition(enemyBounds.left, enemyBounds.top);
        enemyBoundsRect.setSize({enemyBounds.width, enemyBounds.height});

        // Draw everything
        window.clear(sf::Color(50, 50, 50));
        window.draw(hero.CreateSprite());
        window.draw(enemy.CreateSprite());
        window.draw(heroBoundsRect);
        window.draw(enemyBoundsRect);

        // Draw both entities' extended bounds
        DrawExtendedBounds(window, heroBounds, hero.radius, sf::Color::Yellow);
        DrawExtendedBounds(window, enemyBounds, enemy.radius, sf::Color::Magenta);

        window.draw(collisionText);
        window.display();
    }

    return 0;
}

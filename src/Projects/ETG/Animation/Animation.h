#pragma once;
#include <SFML/Graphics/Texture.hpp>

class Animation
{
private:
    float EachFrameSpeed;
    float AnimTimeLeft;
    int CurrentFrame = 0;
    int FrameX;
    int FrameY;
    bool Active = true;

public:
    sf::Texture Texture;
    sf::Vector2f Origin;
    std::vector<sf::Rect<int>> Frames;
    bool IsValid = true;

    //NOTE: Rule of Five: Destructor, Copy Constructor, Copy Assignment, Move Constructor, Move Assignment
    Animation(const sf::Texture& texture, float eachFrameSpeed, int frameX, int frameY, int row = 1);
    ~Animation() = default;
    Animation(const Animation& other) = default; // Copy constructor
    Animation(Animation&& other) = default; // Move constructor
    Animation& operator=(const Animation& other) = default; // Copy assignment operator

    ///Empty constructor. The point of this is let member Animation variables default constructed.
    ///So that I can easily initialize in Initialize() function instead of directly through constructor
    Animation() = default;

    /// \brief decrement `AnimTimeLeft` If AnimTimeLeft is 0, increment CurrentFrame, restart CurrentFrame counter
    void Update();
    void Draw(sf::Vector2f position, float layerDepth);
    void Draw(const sf::Texture& texture, sf::Vector2f position, sf::Color color, float rotation, sf::Vector2f origin, float scale);

    /// Restart the animation. Set `CurrentFrame` = 0, `AnimTimeLeft` = `EachFrameSpeed`
    void Restart();
    sf::Texture GetCurrentFrameAsTexture() const;
    bool IsAnimationFinished() const;
};

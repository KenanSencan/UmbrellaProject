#include "Unit.h"
#include <cmath>
#include <iostream>
#include <vector>

// --- Configuration ---
constexpr float UNIT_RADIUS = 15.f;
constexpr float FOLLOWER_MOVE_SPEED = 170.f;    // pixels per second
constexpr float FOLLOWER_ROTATION_SPEED = 2.5f; // radians per second
constexpr float ALIGNMENT_THRESHOLD = 0.98f;    // Dot product value (closer to 1 is more aligned)
constexpr float FOLLOW_DISTANCE = 60.f;

MAIN_TEMPLATE_GAME_START

Unit leader({(float)window.getSize().x / 2, (float)window.getSize().y / 2}, sf::Color::Green);

// Initialize followers
std::vector<Unit> followers;
for (int i = 0; i < 5; ++i)
{
    followers.emplace_back(leader.position - sf::Vector2f((i + 1) * FOLLOW_DISTANCE, 0.f), sf::Color::Blue);
    followers.back().SetForwardVector(leader.forwardVector);
}

GAME_LOOP_START

// Set leader's direction to mouse
auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
sf::Vector2f directionToMouse = mousePos - leader.position;

if (Math::VectorLength(directionToMouse) > 5.f)
{
    leader.SetForwardVector(Math::Normalize(directionToMouse));
}

DISPLAY_TEXT(std::to_string(Math::RadianToDegree(Math::getAngle(directionToMouse)) + 180.f))
DISPLAY_TEXT("FOLLOWER 0 forward vec: " + std::to_string(followers[0].forwardVector.x) + " " + std::to_string(followers[0].forwardVector.y))

for (int i = 0; i < followers.size(); ++i)
{
    Unit& follower = followers[i];

    // 1. Determine Ideal Position (simple line astern formation)
    //Play with XY at here https://www.geogebra.org/geometry/cegvp83v
    sf::Vector2f idealPosition;
    if (i == 0)
    {
        idealPosition = leader.position - leader.forwardVector * FOLLOW_DISTANCE;
    } 
    else
    {
        idealPosition = followers[i - 1].position - followers[i - 1].forwardVector * FOLLOW_DISTANCE;
    }

    //2. Move towards Ideal pos
    sf::Vector2f IdealPosToFollowPos = idealPosition - follower.position;

    //Avoid jittering if it's already too close
    if (Math::VectorLength(IdealPosToFollowPos) > 1.f)
    {
        const sf::Vector2f moveVector = Math::Normalize(IdealPosToFollowPos) * FOLLOWER_MOVE_SPEED * deltaTime;
        follower.SetPosition(follower.position + moveVector);
    }

    //3. determine alignment and rotate vector
    sf::Vector2f targetForward = (i == 0) ? leader.forwardVector : followers[i - 1].forwardVector;

    // NOTE: Remember the use case of dot product from the project 'LookAtTrigger'. We need to know somehow how much one vector close to another. This alignment checks that
    float alignment = Math::Dot(follower.forwardVector, targetForward);

    //-2 check for invalid dot product before normalization of one vec
    if (alignment < ALIGNMENT_THRESHOLD && alignment > -2.f)
    {
        float lerpDelta = std::min(FOLLOWER_ROTATION_SPEED * deltaTime, 1.0f);
        DISPLAY_TEXT("Lerp delta: " + std::to_string(lerpDelta));
        sf::Vector2f newForward = Math::Lerp(follower.forwardVector, targetForward, lerpDelta);
        follower.SetForwardVector(Math::Normalize(newForward));
    }

    follower.UpdateForwardLine();
}

leader.draw(window);
for (auto& follower : followers)
{
    follower.draw(window);
}

MAIN_TEMPLATE_GAME_END
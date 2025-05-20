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

Unit leader({(float)window.getSize().x / 2, (float)window.getSize().y / 2}, sf::Color::Green, true);

std::vector<Unit> followers;
for (int i = 0; i < 3; ++i)
{
    followers.emplace_back(leader.position - sf::Vector2f((i + 1) * FOLLOW_DISTANCE, 0.f), sf::Color::Blue);

    // Initial alignment attempt (can be more sophisticated)
    followers.back().setForwardVector(leader.forwardVector);
}

GAME_LOOP_START

// --- Leader Update ---
auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
sf::Vector2f directionToMouse = mousePos - leader.position;

// Check length to avoid jitter when mouse is on leader
if (Math::VectorLength(directionToMouse) > 5.f)
{
    leader.setForwardVector(Math::Normalize(directionToMouse));
}

DISPLAY_TEXT(std::to_string(Math::RadianToDegree(Math::getAngle(directionToMouse)) + 180.f))

for (int i = 0; i < followers.size(); ++i)
{
    Unit& follower = followers[i];

    // 1. Determine Ideal Position for the first one (simple line astern formation)
    sf::Vector2f idealPosition;
    if (i == 0)
    {
        idealPosition = leader.position - (leader.forwardVector * FOLLOW_DISTANCE);
    }
    else
    {
        // Follower follows the one in front
        idealPosition = followers[i - 1].position - (followers[i - 1].forwardVector * FOLLOW_DISTANCE);
    }

    // 2. Move towards Ideal Position (new calculated position - current follower position)
    sf::Vector2f IdealPosToFollowPos = idealPosition - follower.position;

    // Threshold to stop jittering (no point to move if it's already very close)
    if (Math::VectorLength(IdealPosToFollowPos) > 1.0f)
    {
        const sf::Vector2f moveVector = Math::Normalize(IdealPosToFollowPos) * FOLLOWER_MOVE_SPEED * deltaTime; //Same calculation as if this is velocity
        follower.setPosition(follower.position + moveVector);
    }

    // 3. Determine Alignment & Rotate if Necessary
    sf::Vector2f targetForward = (i == 0) ? leader.forwardVector : followers[i - 1].forwardVector;
    
    // --- DOT PRODUCT FOR ALIGNMENT ---
    // NOTE: Remember the use case of dot product from the project 'LookAtTrigger'. We need to know somehow how much one vector close to another. This alignment checks that
    float alignment = Math::Dot(follower.forwardVector, targetForward); // Assuming targetForward is already normalized
    
    DISPLAY_TEXT("Follower " + std::to_string(i) + " Alignment: " + std::to_string(alignment));
    
    // -2 check for invalid dot product before normalization of one vec
    if (alignment < ALIGNMENT_THRESHOLD && alignment > -2.0f)
    {
        // Follower needs to rotate to align with the target's forward vector.
        // Simple way: Lerp angle or directly lerp vectors and normalize
        float rotationDelta = std::min(FOLLOWER_ROTATION_SPEED * deltaTime, 1.0f);
        DISPLAY_TEXT("Lerp delta: " + std::to_string(rotationDelta));
        sf::Vector2f newForward = Math::lerp(follower.forwardVector, targetForward, rotationDelta);
        follower.setForwardVector(Math::Normalize(newForward));
    }
    // Keep updating forward line even if not rotating (position might have changed)
    follower.updateForwardLine();
}

leader.draw(window);
for (auto& follower : followers)
{
    follower.draw(window);
}

// --- Debug Text --- (your DISPLAY_TEXT macro)
DISPLAY_TEXT("Move mouse around GREEN leader.");
DISPLAY_TEXT("BLUE followers try to align using DOT PRODUCT.");
DISPLAY_TEXT("If dot product < " + std::to_string(ALIGNMENT_THRESHOLD) + ", follower rotates.");

MAIN_TEMPLATE_GAME_END
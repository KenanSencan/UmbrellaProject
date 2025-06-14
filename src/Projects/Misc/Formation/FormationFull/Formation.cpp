#include "Unit.h"
#include <cmath>
#include <iostream>
#include <vector>

// --- Configuration ---
constexpr float UNIT_RADIUS = 15.f;
constexpr float FOLLOWER_BASE_MOVE_SPEED = 170.f;     // Base pixels per second
constexpr float FOLLOWER_ROTATION_SPEED = 2.5f;       // Radians per second
constexpr float ALIGNMENT_THRESHOLD = 0.98f;          // Dot product value (closer to 1 is more aligned)
constexpr float BASE_FOLLOW_DISTANCE = 60.f;          // Base following distance
constexpr float MIN_SEPARATION_DISTANCE = UNIT_RADIUS * 10.0f;  // Minimum separation for collision
constexpr float MAX_AVOID_DISTANCE = UNIT_RADIUS * 5.0f;       // Distance at which avoidance begins
constexpr float AVOIDANCE_WEIGHT = 2.5f;              // Avoidance priority weight
constexpr float FORMATION_RECOVERY_WEIGHT = 0.8f;     // How strongly to move back to formation
constexpr float PREDICTION_TIME_HORIZON = 1.f;       // Look ahead time in seconds
constexpr float SPEED_SCALING_FACTOR = 0.3f;          // How much speed affects follow 
constexpr float MAX_SPEED_SCALE = 1.8f;               // Maximum scaling of follow distance
constexpr int NUM_FOLLOWERS = 10;                     // Number of followers in formation

MAIN_TEMPLATE_GAME_START

Unit leader({(float)window.getSize().x / 2, (float)window.getSize().y / 2}, sf::Color::Green, true);
sf::Vector2f leaderLastPos = leader.position;
float leaderSpeed = 0.f;

std::vector<Unit> followers;
for (int i = 0; i < NUM_FOLLOWERS; ++i)
{
    followers.emplace_back(leader.position - sf::Vector2f((i + 1) * BASE_FOLLOW_DISTANCE, 0.f), sf::Color::Blue);
    followers.back().setForwardVector(leader.forwardVector);
    followers.back().setIdealPosition(followers.back().position); // Initial ideal position
}

GAME_LOOP_START

// --- Leader Update ---
auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
sf::Vector2f directionToMouse = mousePos - leader.position;

// Calculate leader's speed for dynamic following distance
sf::Vector2f leaderMovement = leader.position - leaderLastPos;
leaderSpeed = Math::VectorLength(leaderMovement) / deltaTime;
leaderLastPos = leader.position;

// Update leader velocity for prediction
leader.setVelocity(leaderMovement / deltaTime);

// Check length to avoid jitter when mouse is on leader
if (Math::VectorLength(directionToMouse) > 5.f)
{
    leader.setForwardVector(Math::Normalize(directionToMouse));
}

// Dynamic follow distance based on leader speed
float speedMultiplier = std::min(MAX_SPEED_SCALE, 1.0f + (leaderSpeed * SPEED_SCALING_FACTOR / FOLLOWER_BASE_MOVE_SPEED));
float currentFollowDistance = BASE_FOLLOW_DISTANCE * speedMultiplier;

DISPLAY_TEXT("Leader Speed: " + std::to_string(leaderSpeed) + " - Follow Distance: " + std::to_string(currentFollowDistance));

for (int i = 0; i < followers.size(); ++i)
{
    Unit& follower = followers[i];

    // 1. Determine Ideal Position with dynamic follow distance
    sf::Vector2f idealPosition;
    if (i == 0)
    {
        idealPosition = leader.position - (leader.forwardVector * currentFollowDistance);
    }
    else
    {
        idealPosition = followers[i - 1].position - (followers[i - 1].forwardVector * currentFollowDistance);
    }
    
    // Store ideal position for formation recovery
    follower.setIdealPosition(idealPosition);

    // 2. Calculate movement toward ideal position
    sf::Vector2f positionDifference = idealPosition - follower.position;
    sf::Vector2f moveDirection = Math::Normalize(positionDifference);
    
    // 3. Calculate collision avoidance with look-ahead prediction
    sf::Vector2f avoidanceVector(0.f, 0.f);
    bool collisionPredicted = false;
    
    // Predict collision with leader
    if (follower.willCollideWith(leader, PREDICTION_TIME_HORIZON, MIN_SEPARATION_DISTANCE))
    {
        avoidanceVector += follower.getAvoidanceVector(leader, MAX_AVOID_DISTANCE);
        collisionPredicted = true;
    }
    
    // Predict collisions with other followers
    for (int j = 0; j < followers.size(); ++j)
    {
        if (i != j && follower.willCollideWith(followers[j], PREDICTION_TIME_HORIZON, MIN_SEPARATION_DISTANCE))
        {
            avoidanceVector += follower.getAvoidanceVector(followers[j], MAX_AVOID_DISTANCE);
            collisionPredicted = true;
        }
    }
    
    // 4. Calculate formation recovery force (stronger the further from ideal position)
    float formationDeviation = follower.formationDeviation;
    float recoveryFactor = std::min(1.0f, formationDeviation / (currentFollowDistance * 0.5f));
    sf::Vector2f formationRecoveryVector = moveDirection * recoveryFactor * FORMATION_RECOVERY_WEIGHT;
    
    // 5. Combine movement forces with dynamic weights
    sf::Vector2f finalMoveDirection;
    
    if (collisionPredicted)
    {
        // When collision predicted, prioritize avoidance but include formation recovery
        finalMoveDirection = Math::Normalize(moveDirection + avoidanceVector * AVOIDANCE_WEIGHT + formationRecoveryVector);
        DISPLAY_TEXT("Follower " + std::to_string(i) + " avoiding collision!");
    }
    else if (formationDeviation > currentFollowDistance * 0.1f) // Only apply recovery if needed
    {
        // When not in formation but no collision, focus on formation recovery
        finalMoveDirection = Math::Normalize(moveDirection + formationRecoveryVector);
    }
    else
    {
        finalMoveDirection = moveDirection;
    }
    
    // 6. Apply movement with dynamic speed based on deviation
    float moveSpeed = FOLLOWER_BASE_MOVE_SPEED;
    
    // Increase speed when far from formation
    if (formationDeviation > currentFollowDistance * 0.5f)
    {
        float speedBoost = std::min(1.5f, 1.0f + (formationDeviation / currentFollowDistance));
        moveSpeed *= speedBoost;
    }
    
    // Apply actual movement if needed
    if (Math::VectorLength(positionDifference) > 1.0f)
    {
        sf::Vector2f moveVector = finalMoveDirection * moveSpeed * deltaTime;
        follower.setPosition(follower.position + moveVector);
        follower.setVelocity(moveVector / deltaTime); // Update velocity for future predictions
    }
    else
    {
        follower.setVelocity({0.f, 0.f});
    }

    // 7. Determine Alignment & Rotate with smoother transitions
    sf::Vector2f targetForward = (i == 0) ? leader.forwardVector : followers[i - 1].forwardVector;
    
    float alignment = Math::Dot(follower.forwardVector, targetForward);
    
    // Add debug information
    if (i == 0) {
        DISPLAY_TEXT("Follower " + std::to_string(i) + " Deviation: " + std::to_string(formationDeviation));
    }
    
    // Rotation with adaptive speed based on misalignment
    if (alignment < ALIGNMENT_THRESHOLD && alignment > -2.0f)
    {
        // Adapt rotation speed: faster when more misaligned
        float alignmentFactor = 1.0f - (alignment / ALIGNMENT_THRESHOLD);
        float adaptiveRotationSpeed = FOLLOWER_ROTATION_SPEED * (0.5f + alignmentFactor);
        
        float rotationDelta = std::min(adaptiveRotationSpeed * deltaTime, 1.0f);
        sf::Vector2f newForward = Math::Lerp(follower.forwardVector, targetForward, rotationDelta);
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

// --- Debug Text ---
DISPLAY_TEXT("Move mouse around GREEN leader.");
DISPLAY_TEXT("BLUE followers adapt with advanced collision avoidance.");
DISPLAY_TEXT("Formation adjusts based on leader speed: " + std::to_string(int(speedMultiplier * 100)) + "%");

MAIN_TEMPLATE_GAME_END
#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <string>

MAIN_TEMPLATE_GAME_START

// Center point for our vectors
sf::Vector2f center(960.0f, 540.0f);

// Two direction vectors we'll interpolate between
sf::Vector2f directionA = Math::Normalize(sf::Vector2f(1.0f, 0.0f));    // Right
sf::Vector2f directionB = Math::Normalize(sf::Vector2f(-0.5f, 0.866f)); // Upper-left (120 degrees)

// Scale for visualization (length of the vectors)
float vectorLength = 300.0f;

// Current interpolation value (0 to 1)
float t = 0.0f;
float speed = 0.3f; // How fast t changes

// Line to visualize the vectors
sf::Vertex vectorsLerp[6];  // Three lines: dirA, dirB, and lerp result
sf::Vertex vectorsNlerp[6]; // Three lines: dirA, dirB, and nlerp result

// Setup colors
sf::Color colorA(0, 255, 0);       // Green for dirA
sf::Color colorB(0, 0, 255);       // Blue for dirB
sf::Color colorLerp(sf::Color::Yellow);  // Yellow for lerp result
sf::Color colorNlerp{sf::Color::Magenta}; // Magenta for nlerp result

sf::Vector2f Start = {1, 4};
sf::Vector2f End = {2, 6};
auto slerp2 = Math::Slerp(Start, End, 0.5f);

GAME_LOOP_START


// Update t value (oscillate between 0 and 1)
t += speed * deltaTime;
if (t > 1.0f)
{
    t = 1.0f;
    speed = -speed;
}
else if (t < 0.0f)
{
    t = 0.0f;
    speed = -speed;
}

// Calculate interpolation results
sf::Vector2f resultLerp = Math::Lerp(directionA, directionB, t);
sf::Vector2f resultNlerp = Math::Nlerp(directionA, directionB, t);

// Calculate lengths for display
float lerpLength = Math::Length(resultLerp);
float nlerpLength = Math::Length(resultNlerp);

// Set up the line vertices for directions A and B (same for both visualizations)
// Direction A (green)
vectorsLerp[0] = sf::Vertex(center, colorA);
vectorsLerp[1] = sf::Vertex(center + directionA * vectorLength, colorA);
vectorsNlerp[0] = sf::Vertex(center, colorA);
vectorsNlerp[1] = sf::Vertex(center + directionA * vectorLength, colorA);

// Direction B (blue)
vectorsLerp[2] = sf::Vertex(center, colorB);
vectorsLerp[3] = sf::Vertex(center + directionB * vectorLength, colorB);
vectorsNlerp[2] = sf::Vertex(center, colorB);
vectorsNlerp[3] = sf::Vertex(center + directionB * vectorLength, colorB);

// LERP Result (yellow)
vectorsLerp[4] = sf::Vertex(center, colorLerp);
vectorsLerp[5] = sf::Vertex(center + resultLerp * vectorLength, colorLerp);

// NLERP Result (magenta)
vectorsNlerp[4] = sf::Vertex(center, colorNlerp);
vectorsNlerp[5] = sf::Vertex(center + resultNlerp * vectorLength, colorNlerp);

// Draw everything
window.clear(sf::Color(30, 30, 30));

window.draw(vectorsLerp, 6, sf::Lines);
// window.draw(vectorsNlerp, 6, sf::Lines);

// Display information
DISPLAY_TEXT("It's not that complicated. nlerp just normalizes the result of lerp. ");
DISPLAY_TEXT("Green: Direction A | Blue: Direction B");
DISPLAY_TEXT("TOP - Regular LERP (Yellow) - Length: " + std::to_string(lerpLength));
DISPLAY_TEXT("BOTTOM - Normalized LERP (Magenta) - Length: " + std::to_string(nlerpLength));
DISPLAY_TEXT("");
DISPLAY_TEXT("NLERP = Normalize(Lerp(A, B, t))");
DISPLAY_TEXT("- Interpolates between direction vectors");
DISPLAY_TEXT("- Maintains constant length (always 1.0)");
DISPLAY_TEXT("- Computationally cheaper than SLERP");
DISPLAY_TEXT("- Used for direction interpolation in games");

MAIN_TEMPLATE_GAME_END
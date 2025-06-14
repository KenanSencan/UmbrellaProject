#pragma once

#include <SFML/System/Vector2.hpp>

//I couldn't think of any other name for this namespace yet
namespace OtherHelper 
{
struct FourCorner
{
    sf::Vector2f TopLeft{};
    sf::Vector2f TopRight{};
    sf::Vector2f BottomLeft{};
    sf::Vector2f BottomRight{};
};

[[nodiscard]] static FourCorner CalculateFourCorner(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& origin, const sf::Vector2f& scale = {1.f, 1.f})
{
    FourCorner Corners;

    //Calculate the scaled size firstly
    const sf::Vector2f scaledSize = {size.x * scale.x, size.y * scale.y};

    //Calculate corners using the scaled size
    Corners.TopLeft = {pos.x, pos.y}; // Top-left
    Corners.TopRight = {pos.x + scaledSize.x, pos.y}; // Top-right
    Corners.BottomLeft = {pos.x, pos.y + scaledSize.y}; // Bottom-left
    Corners.BottomRight = {pos.x + scaledSize.x, pos.y + scaledSize.y}; // Bottom-right

    //Origin also always affected by scale
    sf::Vector2f scaledOrigin = {origin.x * scale.x, origin.y * scale.y};
    Corners.TopLeft -= scaledOrigin;
    Corners.TopRight -= scaledOrigin;
    Corners.BottomLeft -= scaledOrigin;
    Corners.BottomRight -= scaledOrigin;
    return Corners;
}
}
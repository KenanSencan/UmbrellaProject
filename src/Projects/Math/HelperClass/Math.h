#pragma once
#include <stdexcept>
#include <complex>
#include <SFML/System/Vector2.hpp>

class Math
{
public:
    static inline float Dot(sf::Vector2f a, sf::Vector2f b);

    static float DegreeToRadian(const float degree)
    {
        return (degree * std::numbers::pi) / 180.f;
    }
    
    static float RadianToDegree(const float radian)
    {
        return radian * 180.0f / std::numbers::pi;
    }

    template <typename T>
    static inline T VectorSizeSquared(const sf::Vector2<T>& Vector)
    {
        return Vector.x * Vector.x + Vector.y * Vector.y;
    }
    
    template <typename T>
    static float VectorLength(sf::Vector2<T> vector)
    {
        return std::sqrt(VectorSizeSquared(vector));
    }

    template<typename T>
    static float LengthSquared(sf::Vector2<T> vector)
    {
        return VectorSizeSquared(vector) * VectorSizeSquared(vector);
    };

    //I won't bother myself with writing here, "I could have done with dot product + accos" 
    static float getAngle(const sf::Vector2f& vector)
    {
        return std::atan2(vector.y, vector.x);
    }

    //From angle create a vector
    static sf::Vector2f vectorFromAngle(const float angleRad)
    {
        return {std::cos(angleRad), std::sin(angleRad)};
    }

    
    //length: Compute the magnitude of the vector using the formula sqrt(x^2 + y^2)
    // Division: Divide the vector components by the magnitude to scale it to a unit vector (length 1).
    template <typename T>
    static sf::Vector2<T> Normalize(const sf::Vector2<T>& Vector)
    {
        const float length = VectorLength(Vector);
        // if (length == 0) throw std::runtime_error("length is 0. Vector is: " + std::to_string(Vector.x) + " " + std::to_string(Vector.y));

        return Vector / length;
    }

    static float Length(const sf::Vector2f& vector)
    {
        return std::sqrt(vector.x * vector.x + vector.y * vector.y);
    }

    [[nodiscard]] static sf::Vector2f RotateVector(const float rotation, const sf::Vector2f& offset)
    {
        float x = offset.x; // Save current x
        float y = offset.y; // Save current y

        return {static_cast<float>(x * cos(rotation) - y * sin(rotation)), static_cast<float>(x * sin(rotation) + y * cos(rotation))};
    }

    static
    sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float d)
    {
        return start + (end - start) * d;
    }

};

inline float Math::Dot(const sf::Vector2f a, const sf::Vector2f b)
{
    return (a.x * b.x) + (a.y * b.y);
}

#pragma once
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <complex>
#include <numbers>

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

    template <typename T>
    static float LengthSquared(sf::Vector2<T> vector)
    {
        return VectorSizeSquared(vector) * VectorSizeSquared(vector);
    }

    // I won't bother myself with writing here, "I could have done with dot product + accos"
    static float getAngle(const sf::Vector2f& vector)
    {
        return std::atan2(vector.y, vector.x);
    }

    // From angle create a vector
    static sf::Vector2f vectorFromAngle(const float angleRad)
    {
        return {std::cos(angleRad), std::sin(angleRad)};
    }

    // length: Compute the magnitude of the vector using the formula sqrt(x^2 + y^2)
    //  Division: Divide the vector components by the magnitude to scale it to a unit vector (length 1).
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

    [[nodiscard]] static sf::Vector2f RotateVector(const float rotationRad, const sf::Vector2f& offset)
    {
        float x = offset.x; // Save current x
        float y = offset.y; // Save current y

        return {static_cast<float>(x * cos(rotationRad) - y * sin(rotationRad)), static_cast<float>(x * sin(rotationRad) + y * cos(rotationRad))};
    }

    [[nodiscard]] static sf::Vector2f RotatePointAroundPoint(const sf::Vector2f& pointToRotate, const float angleDegrees, const sf::Vector2f& centerOfRotation)
    {
        // Convert degrees to radians
        const float angleRadians = DegreeToRadian(angleDegrees);

        // Step 1: Translate to origin //TODO: Study translations when we move to matrix
        const sf::Vector2f translated = pointToRotate - centerOfRotation;

        // Step 2: Rotate around origin
        sf::Vector2f rotated;
        rotated.x = translated.x * cos(angleRadians) - translated.y * sin(angleRadians);
        rotated.y = translated.x * sin(angleRadians) + translated.y * cos(angleRadians);

        // Step 3: Translate back
        return centerOfRotation + rotated;
    }

    // NOTE: lerps
    static sf::Vector2f Lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float d)
    {
        return start + (end - start) * d;
    }

    // Given a value, find what percentage (0-1) it is between start and end)
    static float InverseLerp(const float start, const float end, const float value)
    {
        // Prevent division by zero
        if (std::abs(end - start) < 0.000001f)
            return 0.0f;

        // Calculate how far value is between start and end (0.0 to 1.0)
        return (value - start) / (end - start);
    }

    // Inverted scalar inverse lerp: returns 1.0 when at start, 0.0 when at end
    static float InvertedInverseLerp(const float start, const float end, const float value)
    {
        return 1.0f - InverseLerp(start, end, value);
    }

    // Vector inverse lerp: returns how far vector is along the path from start to end
    static float InverseLerp(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Vector2f& value)
    {
        sf::Vector2f direction = end - start;
        sf::Vector2f valueOffset = value - start;

        // Project valueOffset onto direction
        float projectionLength = Math::Dot(valueOffset, direction) / Math::Dot(direction, direction);

        return projectionLength;
    }

    // Normalized Linear Interpolation (NLERP)
    // Linearly interpolates and then normalizes the result
    static sf::Vector2f Nlerp(const sf::Vector2f& start, const sf::Vector2f& end, const float t)
    {
        // Linear interpolation
        sf::Vector2f result = Lerp(start, end, t);

        // Normalize the result
        return Normalize(result);
    }

    static sf::Vector2f Slerp(sf::Vector2f& start, sf::Vector2f& end, const float t)
    {
        start = Normalize(start);
        end = Normalize(end);

        float dot = Dot(start, end);
        dot = std::clamp(dot, -1.0f, 1.0f);

        // Calculate the FULL angle between vectors
        const float theta = std::acos(dot);

        // Early exit for nearly parallel vectors
        if (std::abs(dot) > 0.9995f)
        {
            return Nlerp(start, end, t);
        }

        // Use the FULL theta in the rotation formula, with t applied to the weights. I wrote formula in obsidian
        const float sinTheta = std::sin(theta);
        const float weight1 = std::sin((1.0f - t) * theta) / sinTheta;
        const float weight2 = std::sin(t * theta) / sinTheta;

        return start * weight1 + end * weight2;
    }

    // NOTE: interpolations but not lerp
    //  Smoothstep - Cubic Hermite interpolation (smoothly transitions from 0 to 1)
    static float smoothstep(const float t)
    {
        // Clamp input to 0-1 range for safety
        const float x = std::clamp(t, 0.0f, 1.0f);

        // Cubic falloff curve: t * t * (3 - 2 * t)
        return x * x * (3.0f - 2.0f * x);
    }

    // Smoothstep with range - Applies smoothstep to a value within a specified range
    static float smoothstep(const float edge0, const float edge1, const float x)
    {
        // First convert x to 0-1 range based on edges (inverse lerp)
        const float t = InverseLerp(edge0, edge1, x);

        // Then apply smoothstep to this normalized value
        return smoothstep(t);
    }
};

inline float Math::Dot(const sf::Vector2f a, const sf::Vector2f b)
{
    return (a.x * b.x) + (a.y * b.y);
}

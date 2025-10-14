// Vector2.h - Simple 2D Vector Math
#pragma once
#include <cmath>
#include <functional>

struct Vector2 {
    float x, y;
    
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
    
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    
    float distance(const Vector2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    
    Vector2 normalized() const {
        float len = length();
        if (len > 0) {
            return Vector2(x / len, y / len);
        }
        return *this;
    }
    
    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }
};

struct Vector2Hash {
    std::size_t operator()(const Vector2& v) const {
        return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
    }
};

// Rect.h - Rectangle for bounds checking
#pragma once

struct Rect {
    float x, y, width, height;
    
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}
    
    bool intersects(const Rect& other) const {
        return x < other.x + other.width &&
               x + width > other.x &&
               y < other.y + other.height &&
               y + height > other.y;
    }
    
    bool contains(const Vector2& point) const {
        return point.x >= x && point.x <= x + width &&
               point.y >= y && point.y <= y + height;
    }
    
    Vector2 center() const {
        return Vector2(x + width / 2, y + height / 2);
    }
    
    Rect expanded(float amount) const {
        return Rect(x - amount, y - amount, 
                   width + amount * 2, height + amount * 2);
    }
};
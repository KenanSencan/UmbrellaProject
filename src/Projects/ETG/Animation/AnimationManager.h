#pragma once

#include <any>
#include <unordered_map>
#include <variant>
#include <string>
#include "Animation.h"
#include "../Managers/Globals.h" // If you need to reference SFML window, etc.
#include "../Managers/StateEnums.h"

//Variant necessary to provide hashing based on the given type.   
using AnimationKey = std::variant<std::string, int, ETG::RunEnum, ETG::IdleEnum, ETG::DashEnum>; //, Study later to include: std::any

// 3) Custom hash + equality
//Based on given key of variant, convert it to hash
struct AnimationKeyHash
{
    std::size_t operator()(const AnimationKey& key) const
    {
        return std::visit([]<typename T0>(T0&& arg) -> std::size_t
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, std::string>)
            {
                return std::hash<std::string>{}(arg);
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                return std::hash<int>{}(arg);
            }
            else if constexpr (std::is_enum_v<T>)
            {
                using UnderlyingT = std::underlying_type_t<T>;
                return std::hash<UnderlyingT>{}(static_cast<UnderlyingT>(arg));
            }
            else
            {
                static_assert(always_false<T>::value, "Unhandled key type");
            }
            return -1;
        }, key);
    }

private:
    template <typename>
    struct always_false : std::false_type
    {
    };
};

struct AnimationKeyEqual
{
    bool operator()(const AnimationKey& lhs, const AnimationKey& rhs) const
    {
        return lhs == rhs; // variant type supports operator==
    }
};

class AnimationManager
{
public:
    std::unordered_map<AnimationKey, Animation, AnimationKeyHash, AnimationKeyEqual> AnimationDict;
    std::unordered_map<AnimationKey, sf::Vector2f, AnimationKeyHash, AnimationKeyEqual> GunOrigin;

    // For storing whichever key was last used
    AnimationKey LastKey;

    sf::Texture LastTexture;

public:
    // Add an animation to the dictionary
    template <typename T>
    void AddAnimation(T key, const Animation& animation);

    // Update a specific animation
    template <typename T>
    void Update(T key);

    // Draw the last key’s animation
    void Draw(sf::Vector2f position, float layerDepth);

    // Overloaded draw for more complex parameters
    void Draw(const sf::Texture& texture,
              sf::Vector2f position,
              sf::Color color,
              float rotation,
              sf::Vector2f origin,
              float scale);

    // Optionally set origin
    template <typename T>
    void SetOrigin(T key, sf::Vector2f origin);

    // Return the current frame as a texture for the last key
    sf::Texture GetCurrentFrameAsTexture();

    // Check if the current animation has finished
    bool IsAnimationFinished();
};

// <---------- Implementation ---------->

template <typename T>
void AnimationManager::AddAnimation(T key, const Animation& animation)
{
    // Convert user-supplied key to AnimationKey
    const AnimationKey variantKey = key; // Will compile if T is int, std::string, or EnemyIdle
    AnimationDict[variantKey] = animation;
    LastKey = variantKey;
}

template <typename T>
void AnimationManager::Update(T key)
{
    const AnimationKey variantKey = key;

    // If the animation key exists
    if (AnimationDict.contains(variantKey))
    {
        Animation& anim = AnimationDict[variantKey];
        LastTexture = anim.Texture;
        anim.Update();
        LastKey = variantKey;
    }
    else
    {
        // If the key doesn't exist, restart the last animation
        AnimationDict[LastKey].Restart();
    }
}

inline void AnimationManager::Draw(const sf::Vector2f position, const float layerDepth)
{
    // Draw the animation for LastKey
    auto it = AnimationDict.find(LastKey);
    if (it != AnimationDict.end())
    {
        it->second.Draw(position, layerDepth);
    }
}

inline void AnimationManager::Draw(const sf::Texture& texture,
                                   const sf::Vector2f position,
                                   const sf::Color color,
                                   const float rotation,
                                   const sf::Vector2f origin,
                                   const float scale)
{
    auto it = AnimationDict.find(LastKey);
    if (it != AnimationDict.end())
    {
        it->second.Draw(texture, position, color, rotation, origin, scale);
    }
}

template <typename T>
void AnimationManager::SetOrigin(T key, const sf::Vector2f origin)
{
    const AnimationKey variantKey = key;
    auto it = AnimationDict.find(variantKey);
    if (it != AnimationDict.end())
    {
        it->second.Origin = origin;
    }
}

inline sf::Texture AnimationManager::GetCurrentFrameAsTexture()
{
    auto it = AnimationDict.find(LastKey);
    if (it != AnimationDict.end())
    {
        return it->second.GetCurrentFrameAsTexture();
    }
    // Return an empty texture if no last key found
    return sf::Texture{};
}

inline bool AnimationManager::IsAnimationFinished()
{
    auto it = AnimationDict.find(LastKey);
    if (it != AnimationDict.end())
    {
        return it->second.IsAnimationFinished();
    }
    return true; // Or false, depending on your preference
}

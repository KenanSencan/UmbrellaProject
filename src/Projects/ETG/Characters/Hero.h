#pragma once
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "Components/AnimationComponent.h"
#include "../Managers/Misc.h"
#include "../Managers/StateEnums.h"

namespace ETG
{
    class Hero
    {
    public:
        static sf::Vector2f HeroPosition;
        static float MouseAngle;
        static Direction CurrentDirection;

        explicit Hero(sf::Vector2f Position);
        void Update();
        void DrawHeroBounds() const;
        void Draw();
        sf::FloatRect HeroBounds() const;

    private:
        AnimationComponent AnimationComp;
        float Speed{100.f};
        bool IsDashing{};
        sf::Texture HandTex;
        sf::Vector2f HandPos;
        sf::Vector2f RelativeHandLoc{8, 4};
        std::unordered_map<std::pair<int, int>, Direction, PairHash> DirectionMap{};

        void SetMouseHandle();
        void SetRanges();
        Direction GetDirectionFromAngle() const;
        DashEnum GetDashDirectionEnum();
        static RunEnum GetRunEnum(Direction currDir);
        static IdleEnum GetIdleDirectionEnum(Direction currDir);
        void SetHandTexLoc();
    };
}
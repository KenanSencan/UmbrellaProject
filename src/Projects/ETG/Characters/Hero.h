#pragma once
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include "../Animation/AnimationManager.h"
#include "../Managers/StateEnums.h"

namespace ETG
{
    class Hero
    {
    public:
        static sf::Vector2f Position;
        sf::Vector2f RelativeOrigin{0,0};

        //Hero state
        HeroStateEnum HeroState{};
        using animState = std::variant<RunEnum, DashEnum, IdleEnum>;
        AnimationKey AnimState{IdleEnum::Idle_Back};

    private:
        float Speed{300.f};
        std::unordered_map<HeroStateEnum, AnimationManager> AnimManagerDict{};
        float Timer{}, DashTimer{};
        bool IsDashing{}, StartDashTimer{}, DashOnce{};
        int BeforeLerpX{}, BeforeLerpY{};
        int DashAmount{}, DashCoolDown{};
        sf::Texture RedPixel;
        float Rotation{}, angleInDegrees{};

        //Hand
        sf::Vector2f HandPos;
        sf::Texture HandTex;
        
        //Gun will come here

        //Direction enums
        IdleEnum IdleEnum{};
        DashEnum DashEnum{};
        RunEnum RunEnum{};
        float HandRelativeLocX{6.0};

    public:
        explicit Hero(sf::Vector2f Position);
        void Update();
        void Draw();


    private:
        void SetAnimations();

    };

}

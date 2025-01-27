#pragma once
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include "../Animation/AnimationManager.h"
#include "../Managers/Misc.h"
#include "../Managers/StateEnums.h"

namespace ETG
{
    class Hero
    {
    public:
        static sf::Vector2f HeroPosition;

        //Half of the current texture's width and height
        sf::Vector2f RelativeOrigin{0,0};

        //Hero state
        HeroStateEnum HeroState{};
        using animState = std::variant<RunEnum, DashEnum, IdleEnum>;
        AnimationKey AnimState{IdleEnum::Idle_Back};
        std::string DebugRectString; // Add this line
        sf::IntRect CurrTexRect;

        static float MouseAngle;
        static Direction CurrentDirection;

    private:
        float Speed{100.f};
        std::unordered_map<HeroStateEnum, AnimationManager> AnimManagerDict{};
        float Timer{}, DashTimer{};
        bool IsDashing{}, StartDashTimer{}, DashOnce{};
        int BeforeLerpX{}, BeforeLerpY{};
        int DashAmount{}, DashCoolDown{};
        sf::Texture RedPixel;
        sf::Texture CurrentTex;
        float Rotation{}, angleInDegrees{};

        //Hand
        sf::Vector2f HandPos;
        sf::Texture HandTex;
        sf::Vector2f HandOrigin;
        sf::Vector2f RelativeHandLoc{8,4};
        
        //Gun will come here

        //Direction enums
        IdleEnum IdleEnum{};
        DashEnum DashEnum{};
        RunEnum RunEnum{};
        float HandRelativeLocX{6.0};

        //Directions
        std::unordered_map<std::pair<int,int>,Direction, PairHash> DirectionMap{};
    
    public:
        explicit Hero(sf::Vector2f Position);
        void SetMouseHandle();
        void GetIdleDirectionEnum(ETG::Direction currDir);
        void GetRunEnum(ETG::Direction currDir);
        void Update();
        void Draw();

        //Flip hero and hand sprites
        void FlipSprites(AnimationManager::AnimationMap::mapped_type& CurrAnimState);

        //Get hero bounds 
        sf::FloatRect HeroBounds() const;

        void SetHandTexLoc();

    private:
        void SetAnimations();
        //Draw hero bounds
        void DrawBounds() const;

        void SetRanges();
        Direction GetDirectionFromAngle() const;
        static enum DashEnum GetDashDirectionEnum();
    };

}

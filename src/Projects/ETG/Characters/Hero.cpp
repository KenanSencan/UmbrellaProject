#include "Hero.h"
#include <iostream>

#include "../Managers/DebugTexts.h"
#include "../Managers/InputManager.h"

sf::Vector2f ETG::Hero::HeroPosition = {0.f, 0.f};

ETG::Hero::Hero(const sf::Vector2f Position) : HandPos({}), HandTex({})
{
    ETG::Hero::HeroPosition = Position;

    //Set hand tex file
    if (!HandTex.loadFromFile((std::filesystem::current_path().parent_path() / "Resources" / "Player" / "rogue_hand_001.PNG").string()))
        std::cerr << "Failed to load texture " << std::endl;

    SetAnimations();
}

void ETG::Hero::Update()
{
    //Set the HeroState to an enum later on. Relying on literal string like this unacceptable
    //Set the HeroState based on the hero's status 
    if (InputManager::IsMoving() && !IsDashing)
    {
        HeroState = HeroStateEnum::Run;
        HeroPosition += Globals::Normalize(InputManager::direction) * Speed * Globals::FrameTick;
    }
    else if (!InputManager::IsMoving() && !IsDashing) HeroState = HeroStateEnum::Idle;
    else if (IsDashing) HeroState = HeroStateEnum::Dash;

    //Set AnimState based on the HeroState
    if (HeroState == HeroStateEnum::Dash) AnimState = DashEnum::Dash_Back;
    else if (HeroState == HeroStateEnum::Run) AnimState = RunEnum::Run_Back;
    else if (HeroState == HeroStateEnum::Idle) AnimState = IdleEnum::Idle_Back;

    //Update the animation based on the AnimState
    AnimManagerDict[HeroState].Update(AnimState);

    //Set Origin to currently playing set animation's last texture's predefined origin. 
    RelativeOrigin = AnimManagerDict[HeroState].AnimationDict[AnimManagerDict[HeroState].LastKey].Origin;
    HandPos = HeroPosition + RelativeOrigin;

    //Origin: 11.5 12
    //NOTE: commented print. At here create red rectangle around hero.
    CurrTexRect = AnimManagerDict[HeroState].AnimationDict[AnimState].CurrRect;

    SetHandTexLoc();
    
    DebugRectString = " Current Texture: Left: " + std::to_string(CurrTexRect.left) +
        ", Top: " + std::to_string(CurrTexRect.top) +
        ", Width: " + std::to_string(CurrTexRect.width) +
        ", Height: " + std::to_string(CurrTexRect.height) +
        ", Pos X: " + std::to_string(CurrTexRect.getPosition().x) +
        " Pos Y: " + std::to_string(CurrTexRect.getPosition().y) +
        ", Size X: " + std::to_string(CurrTexRect.getSize().x) +
        ", Size Y: " + std::to_string(CurrTexRect.getSize().y);
}

void ETG::Hero::Draw()
{
    AnimManagerDict[HeroState].Draw(HeroPosition, 0.01f);
    Globals::Renderer::SimpleDraw(HandTex, HandPos);

    Globals::DrawSinglePixelAtLoc(HeroPosition); //Center of the hero texture
    if (Globals::DrawSinglePixelAtLoc({HeroPosition.x + 7,HeroPosition.y +  4})) return;

    DrawBounds();
}

sf::FloatRect ETG::Hero::HeroBounds() const
{
    return {
        HeroPosition.x - (float)CurrTexRect.width / 2.f, //Top-left X (RectLeft) 
        HeroPosition.y - (float)CurrTexRect.height / 2.f, //Top-left Y (RectTop)
        static_cast<float>(CurrTexRect.width), //Width (RectWidth)
        static_cast<float>(CurrTexRect.height) //Height (RectHeight) 
    };
}

void ETG::Hero::DrawBounds() const
{
    const sf::FloatRect Bounds = HeroBounds();
    sf::RectangleShape boundsBox(sf::Vector2f(Bounds.width, Bounds.height));
    boundsBox.setPosition(Bounds.left, Bounds.top);
    boundsBox.setOutlineColor(sf::Color::Red);
    boundsBox.setOutlineThickness(1.f);
    boundsBox.setFillColor(sf::Color::Transparent);
    Globals::Window->draw(boundsBox);
}

void ETG::Hero::SetHandTexLoc()
{
    HandPos = {HeroPosition.x + RelativeHandLoc.x,HeroPosition.y +  RelativeHandLoc.y};

    //Set origin
    //TODO: Doing like this feels so wrong. I should create a base class for objects that I should set their origins locations and it will streamline and manage overall drawable objects easier.
    //TODO: They need to have variables like, SetVisibility, RelatieLocation, SetAttachment, Every drawable types will inherit this base class
    HandPos.x -= (float)HandTex.getSize().x /2;
    HandPos.y -= (float)HandTex.getSize().y /2;
};

void ETG::Hero::SetAnimations()
{
    const auto runAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Resources/Player/Run/Back", "rogue_run_back_hands_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Run/BackWard", "rogue_run_backward_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Run/Forward", "rogue_run_forward_hands_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Run/Front", "rogue_run_front_hands_00", "PNG", 0.15f),
    };

    const auto idleAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Resources/Player/Idle/Back", "rogue_idle_back_hand_left_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Idle/BackWard", "rogue_idle_backwards_hands2_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Idle/Front", "rogue_idle_front_hand_left_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Idle/Right", "rogue_idle_hands_00", "PNG", 0.15f),
    };

    const auto dashAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Resources/Player/Dash/Back", "rogue_dodge_back_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Dash/BackWard", "rogue_dodge_left_back_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Dash/Front", "rogue_dodge_front_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Dash/Left", "rogue_dodge_left_00", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Dash/Right", "rogue_dodge_left_00", "PNG", 0.15f),
    };

    auto animManagerRun = AnimationManager{};
    auto animManagerIdle = AnimationManager{};
    auto animManagerDash = AnimationManager{};

    for (int i = 0; i < runAnims.size(); ++i)
    {
        animManagerRun.AddAnimation(RunEnumValues[i], runAnims[i]);
        animManagerRun.SetOrigin(RunEnumValues[i], sf::Vector2f{(float)runAnims[i].FrameRects[0].width / 2, (float)runAnims[i].FrameRects[0].height / 2}); //I am hoping I made correct origin
    }
    AnimManagerDict[HeroStateEnum::Run] = animManagerRun;

    for (int i = 0; i < idleAnims.size(); ++i)
    {
        animManagerIdle.AddAnimation(IdleEnumValues[i], idleAnims[i]);
        animManagerIdle.SetOrigin(IdleEnumValues[i], sf::Vector2f{(float)idleAnims[i].FrameRects[0].width / 2, (float)idleAnims[i].FrameRects[0].height / 2}); //I am hoping I made correct origin
    }
    AnimManagerDict[HeroStateEnum::Idle] = animManagerIdle;

    for (int i = 0; i < dashAnims.size(); ++i)
    {
        animManagerDash.AddAnimation(DashEnumValues[i], dashAnims[i]);
        animManagerDash.SetOrigin(DashEnumValues[i], sf::Vector2f{(float)dashAnims[i].FrameRects[0].width / 2, (float)dashAnims[i].FrameRects[0].height / 2}); //I am hoping I made correct origin
    }
    AnimManagerDict[HeroStateEnum::Dash] = animManagerDash;
}
;

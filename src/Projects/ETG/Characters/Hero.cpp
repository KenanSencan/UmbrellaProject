#include "Hero.h"
#include <iostream>

#include "../Managers/DebugTexts.h"
#include "../Managers/InputManager.h"
#include "../Managers/Misc.h"

sf::Vector2f ETG::Hero::HeroPosition = {0.f, 0.f};
float ETG::Hero::MouseAngle = 0;
ETG::Direction ETG::Hero::CurrentDirection{};

ETG::Hero::Hero(const sf::Vector2f Position) : HandPos({}), HandTex({})
{
    HeroPosition = Position;

    //Set hand tex file
    if (!HandTex.loadFromFile((std::filesystem::current_path().parent_path() / "Resources" / "Player" / "rogue_hand_001.PNG").string()))
        std::cerr << "Failed to load texture " << std::endl;

    SetAnimations();
    SetRanges();
}

void ETG::Hero::SetMouseHandle()
{
    MouseAngle = Misc::RadiansToDegrees(InputManager::GetMouseAngleRelativeToHero());
    if (MouseAngle < 0) MouseAngle += 360;
}

void ETG::Hero::FlipSprites(AnimationManager::AnimationMap::mapped_type& CurrAnimState)
{
    //Firstly flip hero and hand
    if (CurrentDirection == Direction::Right || CurrentDirection == Direction::FrontHandRight || CurrentDirection == Direction::BackDiagonalRight || CurrentDirection == Direction::BackHandRight)
    {
        CurrAnimState.flipX = 1.f;
        RelativeHandLoc = {8.f, 5.f};
    }
    else
    {
        CurrAnimState.flipX = -1.f;
        RelativeHandLoc = {-8.f, 5.f};
    }

    //Flip hand again
    if (CurrentDirection == Direction::BackDiagonalRight) RelativeHandLoc = RelativeHandLoc = {-8.f, 5.f};
    if (CurrentDirection == Direction::BackDiagonalLeft) RelativeHandLoc = RelativeHandLoc = {8.f, 5.f};
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


    //Set Origin to currently playing set animation's last texture's predefined origin. 
    RelativeOrigin = AnimManagerDict[HeroState].AnimationDict[AnimManagerDict[HeroState].LastKey].Origin;

    //Set current text and it's rect
    auto& CurrAnimState = AnimManagerDict[HeroState].AnimationDict[AnimState];
    CurrTexRect = CurrAnimState.CurrRect;
    CurrentTex = CurrAnimState.GetCurrentFrameAsTexture();
    SetHandTexLoc();

    DebugRectString = " Current Texture: Left: " + std::to_string(CurrTexRect.left) +
        ", Top: " + std::to_string(CurrTexRect.top) +
        ", Width: " + std::to_string(CurrTexRect.width) +
        ", Height: " + std::to_string(CurrTexRect.height) +
        ", Pos X: " + std::to_string(CurrTexRect.getPosition().x) +
        " Pos Y: " + std::to_string(CurrTexRect.getPosition().y) +
        ", Size X: " + std::to_string(CurrTexRect.getSize().x) +
        ", Size Y: " + std::to_string(CurrTexRect.getSize().y);

    //Set AnimState based on the HeroState and mouse position.
    SetMouseHandle();
    CurrentDirection = GetDirectionFromAngle();

    if (HeroState == HeroStateEnum::Dash)
    {
        AnimState = GetDashDirectionEnum();
    }
    else if (HeroState == HeroStateEnum::Run)
    {
        GetRunEnum(CurrentDirection);
    }
    else if (HeroState == HeroStateEnum::Idle)
    {
        GetIdleDirectionEnum(CurrentDirection);
    }

    // Set hand location and flip character
    FlipSprites(CurrAnimState);

    //Always update the animation lastly
    AnimManagerDict[HeroState].Update(AnimState);
}

void ETG::Hero::Draw()
{
    AnimManagerDict[HeroState].Draw(HeroPosition, 0.01f);
    Globals::Renderer::SimpleDraw(HandTex, HandPos);

    Globals::DrawSinglePixelAtLoc(HeroPosition); //Center of the hero texture
    if (Globals::DrawSinglePixelAtLoc({HeroPosition.x + 7, HeroPosition.y + 4})) return;

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
    HandPos = HeroPosition + RelativeHandLoc;

    //Set origin
    //TODO: Doing like this feels so wrong. I should create a base class for objects that I should set their origins locations and it will streamline and manage overall drawable objects easier.
    //TODO: They need to have variables like, SetVisibility, RelativeLocation, SetAttachment, Every drawable types will inherit this base class
    HandPos.x -= (float)HandTex.getSize().x / 2;
    HandPos.y -= (float)HandTex.getSize().y / 2;
}

void ETG::Hero::SetAnimations()
{
    const auto runAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Resources/Player/Run/Back", "rogue_run_back_hands_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Run/BackWard", "rogue_run_backward_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Run/Forward", "rogue_run_forward_hands_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Run/Front", "rogue_run_front_hands_001", "PNG", 0.15f),
    };

    const auto idleAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Resources/Player/Idle/Back", "rogue_idle_back_hand_left_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Idle/BackWard", "rogue_idle_backwards_hands2_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Idle/Front", "rogue_idle_front_hand_left_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Idle/Right", "rogue_idle_hands_001", "PNG", 0.15f),
    };

    const auto dashAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Resources/Player/Dash/Back", "rogue_dodge_back_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Dash/BackWard", "rogue_dodge_left_back_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Dash/Front", "rogue_dodge_front_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Dash/Left", "rogue_dodge_left_001", "PNG", 0.15f),
        Animation::CreateSpriteSheet("Resources/Player/Dash/Right", "rogue_dodge_left_001", "PNG", 0.15f),
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

void ETG::Hero::SetRanges()
{
    DirectionMap[{0, 55}] = Direction::Right;
    DirectionMap[{55, 75}] = Direction::FrontHandRight;
    DirectionMap[{75, 115}] = Direction::FrontHandLeft;
    DirectionMap[{115, 190}] = Direction::Left;
    DirectionMap[{190, 245}] = Direction::BackDiagonalLeft;
    DirectionMap[{245, 265}] = Direction::BackHandLeft;
    DirectionMap[{265, 290}] = Direction::BackHandRight;
    DirectionMap[{290, 310}] = Direction::BackDiagonalRight;
    DirectionMap[{310, 360}] = Direction::Right; // Handle the wrap-around from 310-0
}

ETG::Direction ETG::Hero::GetDirectionFromAngle() const
{
    for (const auto& entry : DirectionMap)
    {
        const int rangeStart = entry.first.first;
        const int rangeEnd = entry.first.second;

        if (MouseAngle >= rangeStart && MouseAngle < rangeEnd)
        {
            return entry.second;
        }
    }

    throw std::out_of_range("Mouse angle is out of defined ranges");
}

ETG::DashEnum ETG::Hero::GetDashDirectionEnum()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) return DashEnum::Dash_BackWard;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) return DashEnum::Dash_BackWard;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) return DashEnum::Dash_BackWard;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) return DashEnum::Dash_BackWard;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) return DashEnum::Dash_Left;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) return DashEnum::Dash_Right;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) return DashEnum::Dash_Back;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) return DashEnum::Dash_Front;
    return DashEnum::Unknown;
}

void ETG::Hero::GetRunEnum(const Direction currDir)
{
    if (currDir == Direction::BackHandRight || currDir == Direction::BackHandLeft) AnimState = RunEnum::Run_Back;
    if (currDir == Direction::BackDiagonalRight || currDir == Direction::BackDiagonalLeft) AnimState = RunEnum::Run_BackWard;
    if (currDir == Direction::Right || currDir == Direction::Left) AnimState = RunEnum::Run_Forward;
    if (currDir == Direction::FrontHandRight || currDir == Direction::FrontHandLeft) AnimState = RunEnum::Run_Front;
}

void ETG::Hero::GetIdleDirectionEnum(const Direction currDir)
{
    if (currDir == Direction::BackHandRight || currDir == Direction::BackHandLeft) AnimState = IdleEnum::Idle_Back;
    if (currDir == Direction::BackDiagonalRight || currDir == Direction::BackDiagonalLeft) AnimState = IdleEnum::Idle_BackWard;
    if (currDir == Direction::Right || currDir == Direction::Left) AnimState = IdleEnum::Idle_Right;
    if (currDir == Direction::FrontHandRight || currDir == Direction::FrontHandLeft) AnimState = IdleEnum::Idle_Front;
}

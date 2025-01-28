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
    if (!HandTex.loadFromFile((std::filesystem::current_path().parent_path() / "Resources" / "Player" / "rogue_hand_001.PNG").string()))
        std::cerr << "Failed to load hand texture" << std::endl;
    SetRanges();
}

void ETG::Hero::SetMouseHandle()
{
    MouseAngle = Misc::RadiansToDegrees(InputManager::GetMouseAngleRelativeToHero());
    if (MouseAngle < 0) MouseAngle += 360;
}

void ETG::Hero::Update()
{
    HeroStateEnum heroState = HeroStateEnum::Idle;
    AnimationKey animState = IdleEnum::Idle_Back;

    if (InputManager::IsMoving() && !IsDashing)
    {
        heroState = HeroStateEnum::Run;
        HeroPosition += Globals::Normalize(InputManager::direction) * Speed * Globals::FrameTick;
    }
    else if (IsDashing)
    {
        heroState = HeroStateEnum::Dash;
    }

    SetMouseHandle();
    CurrentDirection = GetDirectionFromAngle();

    if (heroState == HeroStateEnum::Dash)
    {
        animState = GetDashDirectionEnum();
    }
    else if (heroState == HeroStateEnum::Run)
    {
        animState = GetRunEnum(CurrentDirection);
    }
    else if (heroState == HeroStateEnum::Idle)
    {
        animState = GetIdleDirectionEnum(CurrentDirection);
    }

    AnimationComp.Update(heroState, animState);
    RelativeHandLoc = AnimationComp.FlipSprites(CurrentDirection);
    SetHandTexLoc();
}

void ETG::Hero::Draw()
{
    AnimationComp.Draw(HeroPosition);
    Globals::Renderer::SimpleDraw(HandTex, HandPos);
    Globals::DrawSinglePixelAtLoc(HeroPosition);

    // Debug bounds drawing
    DrawHeroBounds();
}

sf::FloatRect ETG::Hero::HeroBounds() const
{
    const auto& currTexRect = AnimationComp.CurrTexRect;
    return {
        HeroPosition.x - currTexRect.width / 2.f,
        HeroPosition.y - currTexRect.height / 2.f,
        static_cast<float>(currTexRect.width),
        static_cast<float>(currTexRect.height)
    };
}

void ETG::Hero::DrawHeroBounds() const
{
    const sf::FloatRect bounds = HeroBounds();
    sf::RectangleShape boundsBox(sf::Vector2f(bounds.width, bounds.height));
    boundsBox.setPosition(bounds.left, bounds.top);
    boundsBox.setOutlineColor(sf::Color::Red);
    boundsBox.setOutlineThickness(1.f);
    boundsBox.setFillColor(sf::Color::Transparent);
    Globals::Window->draw(boundsBox);
}

void ETG::Hero::SetHandTexLoc()
{
    HandPos = HeroPosition + RelativeHandLoc;
    HandPos.x -= static_cast<float>(HandTex.getSize().x) / 2;
    HandPos.y -= static_cast<float>(HandTex.getSize().y) / 2;
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
    DirectionMap[{310, 360}] = Direction::Right;
}

ETG::Direction ETG::Hero::GetDirectionFromAngle() const
{
    for (const auto& entry : DirectionMap)
    {
        if (MouseAngle >= entry.first.first && MouseAngle < entry.first.second)
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

ETG::RunEnum ETG::Hero::GetRunEnum(const Direction currDir)
{
    if (currDir == Direction::BackHandRight || currDir == Direction::BackHandLeft) return RunEnum::Run_Back;
    if (currDir == Direction::BackDiagonalRight || currDir == Direction::BackDiagonalLeft) return RunEnum::Run_BackWard;
    if (currDir == Direction::Right || currDir == Direction::Left) return RunEnum::Run_Forward;
    if (currDir == Direction::FrontHandRight || currDir == Direction::FrontHandLeft) return RunEnum::Run_Front;
    return RunEnum::Run_Forward; // Default case
}

ETG::IdleEnum ETG::Hero::GetIdleDirectionEnum(const Direction currDir)
{
    if (currDir == Direction::BackHandRight || currDir == Direction::BackHandLeft) return IdleEnum::Idle_Back;
    if (currDir == Direction::BackDiagonalRight || currDir == Direction::BackDiagonalLeft) return IdleEnum::Idle_BackWard;
    if (currDir == Direction::Right || currDir == Direction::Left) return IdleEnum::Idle_Right;
    if (currDir == Direction::FrontHandRight || currDir == Direction::FrontHandLeft) return IdleEnum::Idle_Front;
    return IdleEnum::Idle_Back; // Default case
}
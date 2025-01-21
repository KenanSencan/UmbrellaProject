#include "Hero.h"

#include <iostream>

#include "../Managers/InputManager.h"

sf::Vector2f ETG::Hero::Position = {0.f, 0.f};

ETG::Hero::Hero(const sf::Vector2f Position) : HandPos({}), HandTex({})
{
    ETG::Hero::Position = Position;

    //Set hand tex file
    if (!HandTex.loadFromFile((std::filesystem::current_path().parent_path() / "Resources" / "Player" / "rogue_hand_001.PNG").string()))
        std::cerr << "Failed to load texture " << std::endl;
    
    SetAnimations();
}

void ETG::Hero::Update()
{
    //Set the currentState to an enum later on. Relying on literal string like this unacceptable
    //Set the current state based on the hero's status 
    if (InputManager::IsMoving() && !IsDashing)
    {
        currentState = "Run";
        Position += Globals::Normalize(InputManager::direction) * Speed * Globals::FrameTick;
    }
    else if (!InputManager::IsMoving() && !IsDashing) currentState = "Idle";
    else if (IsDashing) currentState = "Dash";

    //Give keys for AnimManagerDict based on the currentState
    if (currentState == "Dash")
    {
        AnimManagerDict[currentState].Update(DashEnum::Dash_Back);
    }
    else if (currentState == "Run")
    {
        AnimManagerDict[currentState].Update(RunEnum::Run_Back);
    }
    else if (currentState == "Idle")
    {
        AnimManagerDict[currentState].Update(IdleEnum::Idle_Back);
    }

    //Set Origin to currently playing set animation's last texture's predefined origin. 
    RelativeOrigin = AnimManagerDict[currentState].AnimationDict[AnimManagerDict[currentState].LastKey].Origin;
    HandPos = Position + RelativeOrigin + sf::Vector2f(6.0 * 25, 1 * 25);
    //11.5 12
}

void ETG::Hero::Draw()
{
    AnimManagerDict[currentState].Draw(Position, 0.01f);

    //Temporarily 
    if (VisualizeOrigin()) return;
    Globals::Renderer::SimpleDraw(HandTex,HandPos);
    
}

void ETG::Hero::SetAnimations()
{
    //1. Somehow ensure origin is centered at the hero's center sprite. For now somehow left increasing so much. Fix that but I guess I will not need. SInce Height and Width already known
    //print: Height: 24 Width: 23 Top: 0 left: 115.           Height and Width are known but Top and Left. Learn these

    //Draw a circle to in origin to indicate the drawn origin


    //2. Make the player playable. Convert everything to player properly
    //3. I am sure there's a better way to handle textures. I made CreateSpriteSheet but in profiler, profile the optimization of CreateSpriteSheet. If necessary create new Sprite yourself.
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
        animManagerRun.SetOrigin(RunEnumValues[i], sf::Vector2f{(float)runAnims[i].Frames[0].width / 2, (float)runAnims[i].Frames[0].height / 2}); //I am hoping I made correct origin
    }
    AnimManagerDict["Run"] = animManagerRun;

    for (int i = 0; i < idleAnims.size(); ++i)
    {
        animManagerIdle.AddAnimation(IdleEnumValues[i], idleAnims[i]);
        animManagerIdle.SetOrigin(IdleEnumValues[i], sf::Vector2f{(float)idleAnims[i].Frames[0].width / 2, (float)idleAnims[i].Frames[0].height / 2}); //I am hoping I made correct origin
    }
    AnimManagerDict["Idle"] = animManagerIdle;

    for (int i = 0; i < dashAnims.size(); ++i)
    {
        animManagerDash.AddAnimation(DashEnumValues[i], dashAnims[i]);
        animManagerDash.SetOrigin(DashEnumValues[i], sf::Vector2f{(float)dashAnims[i].Frames[0].width / 2, (float)dashAnims[i].Frames[0].height / 2}); //I am hoping I made correct origin
    }
    AnimManagerDict["Dash"] = animManagerDash;
}

bool ETG::Hero::VisualizeOrigin() const
{
    // Create a 1x1 green pixel using sf::Image
    sf::Image greenPixel;
    greenPixel.create(1, 1, sf::Color::Green);

    // Load the image into a texture
    sf::Texture tex;
    if (!tex.loadFromImage(greenPixel))
    {
        return true; // Return true if texture creation fails
    }

    // Set up the sprite with the 1x1 green texture
    sf::Sprite frame;
    frame.setTexture(tex);
    frame.setScale(25.f, 25.f); // Scale to make it visually larger
    frame.setPosition(Position + RelativeOrigin); // Position it at the specified location

    // Draw the sprite
    Globals::Window->draw(frame);
    return false;
}

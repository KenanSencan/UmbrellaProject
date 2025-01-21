#include "Animation.h"
#include "../Managers/Globals.h"

Animation::Animation(const sf::Texture& texture, const float eachFrameSpeed, const int frameX, const int frameY, const int row)
    : EachFrameSpeed(eachFrameSpeed), AnimTimeLeft(EachFrameSpeed), FrameX(frameX), FrameY(frameY), Texture(texture)
{
    const int frameXSize = Texture.getSize().x / frameX;
    const int frameYSize = Texture.getSize().y / frameY;

    for (int i = 0; i < frameX; ++i)
    {
        int frameWidth = i * frameXSize; //32 64 96 128
        int frameHeight = (row - 1) * frameYSize; //32 32 32 32

        // Frames.emplace_back(frameXSize,frameYSize,frameWidth,frameHeight); // this is wrong
        Frames.emplace_back(frameWidth, frameHeight, frameXSize, frameYSize);
    }
}

void Animation::Update()
{
    if (!Active) return;
    AnimTimeLeft -= ETG::Globals::FrameTick;
    if (AnimTimeLeft <= 0)
    {
        CurrentFrame++;
        CurrentFrame = CurrentFrame >= FrameX ? 0 : CurrentFrame; //restart if CurrentFrame reached last element 
        AnimTimeLeft = EachFrameSpeed;
    }

    //Set current frame
    CurrRect = Frames[CurrentFrame];
}

void Animation::Draw(const sf::Vector2f position, float layerDepth) const
{
    sf::Sprite frame;
    frame.setTexture(Texture);
    frame.setTextureRect(Frames[CurrentFrame]);
    frame.setPosition(position);
    frame.setColor(sf::Color::White);
    frame.setRotation(0.f);
    frame.setOrigin(Origin);
    frame.setScale(25.f, 25.f); //Idk why I should set it 5f

    ETG::Globals::Window->draw(frame);
}

void Animation::Draw(const sf::Texture& texture, const sf::Vector2f position, const sf::Color color, const float rotation, const sf::Vector2f origin, const float scale) const
{
    sf::Sprite frame;
    frame.setTexture(Texture);
    frame.setTextureRect(Frames[CurrentFrame]);
    frame.setPosition(position);
    frame.setColor(color);
    frame.rotate(rotation);
    frame.setOrigin(origin);
    frame.setScale(scale, scale);

    ETG::Globals::Window->draw(frame);
}

void Animation::Restart()
{
    CurrentFrame = 0;
    AnimTimeLeft = EachFrameSpeed;
}

sf::Texture Animation::GetCurrentFrameAsTexture() const
{
    //Get the source rectangle for the current frame 
    const sf::IntRect sourceRectangle = Frames[CurrentFrame];

    //Copy the texture's pixel data into an sf::Image
    const sf::Image image = Texture.copyToImage();

    //Create a new sf::Image containing only the current frame
    sf::Image frameImage;
    frameImage.create(sourceRectangle.width, sourceRectangle.height);
    frameImage.copy(image, 0, 0, sourceRectangle);

    //Create a new texture from the frame image
    sf::Texture frameTexture;
    frameTexture.loadFromImage(frameImage);

    return frameTexture;
}

bool Animation::IsAnimationFinished() const
{
    return CurrentFrame == FrameX - 1;
}

Animation Animation::CreateSpriteSheet(const std::string& RelativePath, const std::string& FileName, const std::string& Extension, const float eachFrameSpeed)
{
    //Initial setup
    std::vector<sf::Image> imageArr;
    int counter = 1;
    int totalWidth = 0, maxHeight = 0;
    // const std::string AnimPath = (std::filesystem::current_path().parent_path() / "Resources" / "Player" / "Idle" / "Back" / "rogue_idle_back_hand_left_00").string();
    const std::string basePath = (std::filesystem::current_path().parent_path() / RelativePath / FileName).string();
    std::string filePath = basePath + std::to_string(counter) + "." += Extension;

    //Check firstly if filepath is valid
    if (!std::filesystem::exists(filePath)) throw std::runtime_error("File not found at: " + filePath);

    //Load all the given textures 
    while (true)
    {
        sf::Image singleImage;
        filePath = basePath + std::to_string(counter) + "." += Extension;
        if (!std::filesystem::exists(filePath)) break;

        singleImage.loadFromFile(filePath);
        imageArr.push_back(singleImage);
        counter++;

        totalWidth += int(singleImage.getSize().x);
        maxHeight = std::max(maxHeight, int(singleImage.getSize().y));
    }

    //Create the spritesheet as image
    sf::Image spriteImage;
    spriteImage.create(totalWidth, maxHeight);

    //Copy images into the image
    unsigned int xOffset = 0;
    for (auto& image : imageArr)
    {
        spriteImage.copy(image,xOffset,0);
        xOffset += image.getSize().x;
    }

    //convert image into texture and draw it
    sf::Texture spriteTex;
    spriteTex.loadFromImage(spriteImage);

    auto anim = Animation{spriteTex,eachFrameSpeed,int(imageArr.size()),1};
    return anim;
}

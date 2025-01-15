//
// Created by selviniah on 13/01/25.
//

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
    AnimTimeLeft -= ETG::Globals::TotalSeconds;
    if (AnimTimeLeft <= 0)
    {
        CurrentFrame++;
        CurrentFrame = CurrentFrame >= FrameX ? 0 : CurrentFrame; //restart if CurrentFrame reached last element 
        AnimTimeLeft = EachFrameSpeed;
    }
}

void Animation::Draw(const sf::Vector2f position, float layerDepth)
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

void Animation::Draw(const sf::Texture& texture, const sf::Vector2f position, const sf::Color color, const float rotation, const sf::Vector2f origin, const float scale)
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

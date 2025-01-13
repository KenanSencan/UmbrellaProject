#include "Globals.h"
#include <filesystem>

namespace ETG::Globals
{
    float TotalSeconds = 0.0f;
    std::shared_ptr<sf::RenderWindow> Window = nullptr;
    sf::Font Font;
    sf::Vector2u ScreenSize;

    void Update(const sf::Time& elapsedTime)
    {
        TotalSeconds = elapsedTime.asSeconds();
    }

    void Initialize(const std::shared_ptr<sf::RenderWindow>& window)
    {
        Window = window;
        ScreenSize = window->getSize();

        //Load font
        if (!Font.loadFromFile((std::filesystem::current_path().parent_path() / "Resources" / "Fonts" / "SegoeUI.ttf").string()))
        {
            throw std::runtime_error("Failed to load font");
        }
    }
}

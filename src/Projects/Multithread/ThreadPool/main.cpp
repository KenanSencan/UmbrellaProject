#include "../../HelperClass/HelperCollection.h"
#include "NPC.h"
#include "Player.h"
#include "ThreadPool.h"
#include <iostream>

MAIN_TEMPLATE_GAME_START

std::cout << std::unitbuf;

// Create thread pool with number of hardware threads
ThreadPool threadPool(std::thread::hardware_concurrency());
std::cout << "Created ThreadPool with " << threadPool.size() << " threads" << std::endl;

// Initialize player and NPCs (passing thread pool reference)
Player player(sf::Vector2f(400.0f, 300.0f), sf::Vector2f(50.0f, 50.0f));
std::vector<NPC> npcs;
npcs.emplace_back(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(40.0f, 40.0f), threadPool);
npcs.emplace_back(sf::Vector2f(600.0f, 200.0f), sf::Vector2f(40.0f, 40.0f), threadPool);
npcs.emplace_back(sf::Vector2f(300.0f, 500.0f), sf::Vector2f(40.0f, 40.0f), threadPool);

GAME_LOOP_START

// Update player
player.Update(deltaTime);

// Update NPCs
for (auto& npc : npcs)
{
    npc.Update();
}

// Draw player
player.Draw(window);

// Draw NPCs
for (auto& npc : npcs)
{
    npc.Draw(window);
}

MAIN_TEMPLATE_GAME_END
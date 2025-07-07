#include "../../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include <map>
#include <iostream>
#include <stdexcept> // For std::runtime_error



// --- Procedural Map Generation Logic ---

// Custom comparator for sf::Vector2i to be used as map key
struct Vector2iCompare {
    bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

enum class DoorDirection {
    HorizontalRight, HorizontalLeft, VerticalUp, VerticalDown, None
};

struct Room {
    int id;
    sf::Vector2i gridPosition;
    DoorDirection entryDir;
    DoorDirection exitDir;
    
    sf::RectangleShape shape;
    sf::Text idText;
    std::vector<sf::RectangleShape> doorShapes;

    bool isStartRoom = false;
    bool isExitRoom = false;

    Room(int _id, sf::Vector2i _gridPos, DoorDirection _entry, DoorDirection _exit)
        : id(_id), gridPosition(_gridPos), entryDir(_entry), exitDir(_exit) {}
};

// --- Global Variables ---
std::vector<Room> generatedRooms;
std::map<sf::Vector2i, Room*, Vector2iCompare> roomGridOccupancy; // Points to room in generatedRooms

const float ROOM_WIDTH = 250.f;
const float ROOM_HEIGHT = 200.f;
const float DOOR_VISUAL_SIZE = 25.f;
const float GRID_SPACING = 50.f; // Visual gap between rooms
const int MAX_ROOMS = 15; // Target number of rooms

sf::View gameView;
bool isPanning = false;
sf::Vector2f lastMousePosView;

std::mt19937 rng(std::random_device{}()); // Random Number Generator

// --- Function Declarations ---
void generateProceduralMap();
DoorDirection getOppositeDirection(DoorDirection dir);
sf::Vector2i getOffsetForDirection(DoorDirection dir);
sf::Vector2f getRoomWorldPosition(sf::Vector2i gridPos);
void setupRoomVisuals(Room& room, const std::string& fontPath);
sf::Color getDirectionColor(DoorDirection dir);
template<typename T> T getRandomElement(const std::vector<T>& vec, std::mt19937& g);
DoorDirection getRandomDirectionFromList(const std::vector<DoorDirection>& possibleDirections, std::mt19937& g);

// --- Main Game ---
MAIN_TEMPLATE_GAME_START // Includes window, infoText, font loading attempt

    gameView = window.getDefaultView(); // Initialize gameView
    generateProceduralMap();

GAME_LOOP_START

        // Event Handling
        if (evt.type == sf::Event::KeyPressed) {
            if (evt.key.code == sf::Keyboard::R) {
                generateProceduralMap();
            }
        }
        if (evt.type == sf::Event::MouseButtonPressed) {
            if (evt.mouseButton.button == sf::Mouse::Middle) { // Or sf::Mouse::Right
                isPanning = true;
                lastMousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window), gameView);
            }
        }
        if (evt.type == sf::Event::MouseButtonReleased) {
            if (evt.mouseButton.button == sf::Mouse::Middle) {
                isPanning = false;
            }
        }
        if (evt.type == sf::Event::MouseMoved) {
            if (isPanning) {
                const sf::Vector2f currentMousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window), gameView);
                sf::Vector2f delta = lastMousePosView - currentMousePosView;
                gameView.move(delta);
                // lastMousePosView is already in view coordinates, no need to update it here until next click.
                // Correction: lastMousePosView should be updated to current to make panning continuous
                // lastMousePosView = currentMousePosView; // This makes it relative, above is fine for fixed start
            }
        }
         if (evt.type == sf::Event::MouseWheelScrolled) {
            if (evt.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                float zoomFactor = (evt.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f; // Closer : Farther
                gameView.zoom(zoomFactor);
            }
        }


        // Update (if any dynamic elements)

        // Drawing
        window.setView(gameView); // Apply panning/zooming

        for (const auto& room : generatedRooms) {
            window.draw(room.shape);
            for(const auto& doorShape : room.doorShapes) {
                window.draw(doorShape);
            }
            window.draw(room.idText);
        }
        
        // Draw connecting lines (optional, can be messy)
        for(size_t i = 0; i < generatedRooms.size(); ++i) {
            const auto& room = generatedRooms[i];
            if (room.exitDir != DoorDirection::None) {
                // Find connected room
                sf::Vector2i nextRoomGridPos = room.gridPosition + getOffsetForDirection(room.exitDir);
                if (roomGridOccupancy.count(nextRoomGridPos)) {
                    //Room* nextRoom = roomGridOccupancy[nextRoomGridPos]; // if we needed nextRoom properties
                    
                    sf::Vector2f p1 = room.shape.getPosition(); // center
                    sf::Vector2f p2 = getRoomWorldPosition(nextRoomGridPos); // center of next

                    if(room.exitDir == DoorDirection::HorizontalRight) p1.x += ROOM_WIDTH/2.f;
                    else if(room.exitDir == DoorDirection::HorizontalLeft) p1.x -= ROOM_WIDTH/2.f;
                    else if(room.exitDir == DoorDirection::VerticalDown) p1.y += ROOM_HEIGHT/2.f;
                    else if(room.exitDir == DoorDirection::VerticalUp) p1.y -= ROOM_HEIGHT/2.f;

                    DoorDirection entryToNext = getOppositeDirection(room.exitDir);
                     if(entryToNext == DoorDirection::HorizontalRight) p2.x += ROOM_WIDTH/2.f;
                    else if(entryToNext == DoorDirection::HorizontalLeft) p2.x -= ROOM_WIDTH/2.f;
                    else if(entryToNext == DoorDirection::VerticalDown) p2.y += ROOM_HEIGHT/2.f;
                    else if(entryToNext == DoorDirection::VerticalUp) p2.y -= ROOM_HEIGHT/2.f;

                    sf::Vertex line[] = { sf::Vertex(p1, sf::Color::White), sf::Vertex(p2, sf::Color::White) };
                    window.draw(line, 2, sf::Lines);
                }
            }
        }


        window.setView(window.getDefaultView()); // Reset view for UI text
        DISPLAY_TEXT("Press R to regenerate. Middle mouse button to pan. Scroll to zoom.");
        DISPLAY_TEXT("Rooms generated: " + std::to_string(generatedRooms.size()));


MAIN_TEMPLATE_GAME_END

// --- Function Implementations ---

void generateProceduralMap() {
    generatedRooms.clear();
    roomGridOccupancy.clear();
    
    int currentRoomId = 0;
    std::string fontPath = (std::filesystem::current_path().parent_path() / "SegoeUI.ttf").string();


    // 1. Create Start Room
    sf::Vector2i startGridPos(0, 0);
    std::vector<DoorDirection> initialPossibleExits = {
        DoorDirection::HorizontalRight, DoorDirection::HorizontalLeft, 
        DoorDirection::VerticalUp, DoorDirection::VerticalDown
    };
    // Check if any initial exit is blocked (shouldn't be for the very first room unless map bounds exist)
    DoorDirection startExitDir = DoorDirection::None;
    if (!initialPossibleExits.empty()) {
         startExitDir = getRandomDirectionFromList(initialPossibleExits, rng);
    }
    if (startExitDir == DoorDirection::None && MAX_ROOMS > 1) { // Should not happen if MAX_ROOMS > 1 for start
        std::cerr << "Could not pick a valid start exit." << std::endl;
        // Pick a default if none (e.g. right)
        startExitDir = DoorDirection::HorizontalRight;
    }


    Room startRoom(currentRoomId++, startGridPos, DoorDirection::None, startExitDir);
    startRoom.isStartRoom = true;
    generatedRooms.push_back(startRoom);
    roomGridOccupancy[startGridPos] = &generatedRooms.back();

    Room* currentChainLeaf = &generatedRooms.back();

    // 2. Generate subsequent rooms in a chain
    for (int i = 1; i < MAX_ROOMS; ++i) {
        if (currentChainLeaf->exitDir == DoorDirection::None) {
            break; // Previous room was already marked as an end point for the chain
        }

        DoorDirection prevExitDir = currentChainLeaf->exitDir;
        sf::Vector2i nextGridPos = currentChainLeaf->gridPosition + getOffsetForDirection(prevExitDir);

        if (roomGridOccupancy.count(nextGridPos)) {
            // Collision: desired spot is already occupied. Make currentChainLeaf an end room.
            currentChainLeaf->exitDir = DoorDirection::None;
            break; 
        }

        DoorDirection nextEntryDir = getOppositeDirection(prevExitDir);
        
        std::vector<DoorDirection> possibleExits;
        DoorDirection allDirs[] = {DoorDirection::VerticalUp, DoorDirection::VerticalDown, DoorDirection::HorizontalLeft, DoorDirection::HorizontalRight};
        
        for (DoorDirection potentialExit : allDirs) {
            if (potentialExit == getOppositeDirection(nextEntryDir)) {
                continue; // Don't immediately go back
            }
            // Check if this exit would lead into an *already existing* room that isn't the one we just came from
            sf::Vector2i posAfterPotentialExit = nextGridPos + getOffsetForDirection(potentialExit);
            if (roomGridOccupancy.count(posAfterPotentialExit)) {
                continue; // Avoid direct loops or immediate collision on next step
            }
            possibleExits.push_back(potentialExit);
        }
        
        DoorDirection nextExitDir = DoorDirection::None;
        if (!possibleExits.empty()) {
            nextExitDir = getRandomDirectionFromList(possibleExits, rng);
        }

        // If this is the last room in the planned chain, it must be an exit room
        if (i == MAX_ROOMS - 1) {
            nextExitDir = DoorDirection::None;
        }
        
        Room newRoom(currentRoomId++, nextGridPos, nextEntryDir, nextExitDir);
        generatedRooms.push_back(newRoom);
        roomGridOccupancy[nextGridPos] = &generatedRooms.back();
        currentChainLeaf = &generatedRooms.back();
    }

    // Finalize: mark the very last room as the official exit if it's not already an end
    if (!generatedRooms.empty()) {
        generatedRooms.back().isExitRoom = true; // The last room created is the exit
        if (generatedRooms.back().exitDir != DoorDirection::None) { // Ensure it is an end type
             generatedRooms.back().exitDir = DoorDirection::None;
        }
    }
    
    // 3. Setup visuals for all rooms
    for (auto& room : generatedRooms) {
        setupRoomVisuals(room, fontPath);
    }
}

void setupRoomVisuals(Room& room, const std::string& fontPath) {
    room.shape.setSize(sf::Vector2f(ROOM_WIDTH, ROOM_HEIGHT));
    room.shape.setOrigin(ROOM_WIDTH / 2.f, ROOM_HEIGHT / 2.f);
    room.shape.setPosition(getRoomWorldPosition(room.gridPosition));
    room.shape.setFillColor(sf::Color(80, 80, 80));
    room.shape.setOutlineThickness(3.f);

    if (room.isStartRoom) room.shape.setOutlineColor(sf::Color::Green);
    else if (room.isExitRoom) room.shape.setOutlineColor(sf::Color::Red);
    else room.shape.setOutlineColor(sf::Color::White);

    room.idText = Object::CreateText(std::to_string(room.id), fontPath, 24, sf::Color::Cyan);
    sf::FloatRect textBounds = room.idText.getLocalBounds();
    room.idText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    room.idText.setPosition(room.shape.getPosition());

    // Door Visuals
    room.doorShapes.clear();
    auto createDoorShape = [&](DoorDirection dir, bool isEntry) {
        sf::RectangleShape doorShape(sf::Vector2f(DOOR_VISUAL_SIZE, DOOR_VISUAL_SIZE));
        doorShape.setFillColor(getDirectionColor(dir));
        sf::Vector2f roomPos = room.shape.getPosition(); // Center of the room
        float halfW = ROOM_WIDTH / 2.f;
        float halfH = ROOM_HEIGHT / 2.f;
        float doorHalf = DOOR_VISUAL_SIZE / 2.f;

        switch (dir) {
            case DoorDirection::HorizontalRight: doorShape.setPosition(roomPos.x + halfW - doorHalf, roomPos.y - doorHalf); break;
            case DoorDirection::HorizontalLeft:  doorShape.setPosition(roomPos.x - halfW, roomPos.y - doorHalf); break; // Adjusted: door origin is top-left
            case DoorDirection::VerticalUp:      doorShape.setPosition(roomPos.x - doorHalf, roomPos.y - halfH); break;
            case DoorDirection::VerticalDown:    doorShape.setPosition(roomPos.x - doorHalf, roomPos.y + halfH - doorHalf); break;
            case DoorDirection::None: return; // No visual for 'None' direction
        }
        if (isEntry) doorShape.setOutlineColor(sf::Color::Yellow); else doorShape.setOutlineColor(sf::Color(150,150,150));
        doorShape.setOutlineThickness(2.f);
        room.doorShapes.push_back(doorShape);
    };
    
    if (room.entryDir != DoorDirection::None) createDoorShape(room.entryDir, true);
    if (room.exitDir != DoorDirection::None) createDoorShape(room.exitDir, false);
}


DoorDirection getOppositeDirection(DoorDirection dir) {
    switch (dir) {
        case DoorDirection::HorizontalRight: return DoorDirection::HorizontalLeft;
        case DoorDirection::HorizontalLeft:  return DoorDirection::HorizontalRight;
        case DoorDirection::VerticalUp:      return DoorDirection::VerticalDown;
        case DoorDirection::VerticalDown:    return DoorDirection::VerticalUp;
        default: return DoorDirection::None;
    }
}

sf::Vector2i getOffsetForDirection(DoorDirection dir) {
    switch (dir) {
        case DoorDirection::HorizontalRight: return sf::Vector2i(1, 0);
        case DoorDirection::HorizontalLeft:  return sf::Vector2i(-1, 0);
        case DoorDirection::VerticalUp:      return sf::Vector2i(0, -1);
        case DoorDirection::VerticalDown:    return sf::Vector2i(0, 1);
        default: return sf::Vector2i(0, 0); // Should not happen for actual connections
    }
}

sf::Vector2f getRoomWorldPosition(sf::Vector2i gridPos) {
    // Multiply by room size + spacing to get world coordinates
    // Offset slightly to center the whole map structure around (0,0) view if needed, or start from positive coords
    return sf::Vector2f(
        gridPos.x * (ROOM_WIDTH + GRID_SPACING), 
        gridPos.y * (ROOM_HEIGHT + GRID_SPACING)
    );
}

sf::Color getDirectionColor(DoorDirection dir) {
    switch (dir) {
        case DoorDirection::HorizontalRight: return sf::Color(255, 100, 100); // Reddish
        case DoorDirection::HorizontalLeft:  return sf::Color(100, 100, 255); // Bluish
        case DoorDirection::VerticalUp:      return sf::Color(100, 255, 100); // Greenish
        case DoorDirection::VerticalDown:    return sf::Color(255, 255, 100); // Yellowish
        default: return sf::Color::Transparent;
    }
}


template<typename T>
T getRandomElement(const std::vector<T>& vec, std::mt19937& g) {
    if (vec.empty()) {
        throw std::runtime_error("Cannot get random element from empty vector");
    }
    std::uniform_int_distribution<> distrib(0, vec.size() - 1);
    return vec[distrib(g)];
}

DoorDirection getRandomDirectionFromList(const std::vector<DoorDirection>& possibleDirections, std::mt19937& g) {
    if (possibleDirections.empty()) return DoorDirection::None;
    return getRandomElement(possibleDirections, g);
}
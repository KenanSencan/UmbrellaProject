#include "../../../HelperClass/HelperCollection.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include <map>
#include <set>
#include <queue>
#include <iostream>
#include <stdexcept>
#include <algorithm>

// --- Procedural Map Generation Logic ---

// Custom comparator for sf::Vector2i to be used as map key
struct Vector2iCompare {
    bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

enum class DoorDirection {
    Right, Left, Up, Down, None
};

enum class RoomType {
    Normal, Large, Boss, DeadEnd
};

struct Door {
    DoorDirection direction;
    bool isConnected = false;
    int connectedRoomId = -1;
    sf::RectangleShape visualShape;
};

struct Room {
    int id;
    sf::Vector2i gridPosition;
    RoomType type = RoomType::Normal;
    
    std::vector<Door> doors;
    
    sf::RectangleShape shape;
    sf::Text idText;
    
    bool isStartRoom = false;
    bool isMainPath = false; // Part of the main path to boss
    int branchDepth = 0; // 0 = main path, 1+ = branch depth

    Room(int _id, sf::Vector2i _gridPos) 
        : id(_id), gridPosition(_gridPos) {}
    
    void addDoor(DoorDirection dir) {
        Door door;
        door.direction = dir;
        doors.push_back(door);
    }
    
    Door* getDoor(DoorDirection dir) {
        for (auto& door : doors) {
            if (door.direction == dir) return &door;
        }
        return nullptr;
    }
    
    std::vector<DoorDirection> getUnconnectedDoors() {
        std::vector<DoorDirection> unconnected;
        for (const auto& door : doors) {
            if (!door.isConnected) {
                unconnected.push_back(door.direction);
            }
        }
        return unconnected;
    }
};

// --- Global Variables ---
std::vector<Room> generatedRooms;
std::map<sf::Vector2i, int, Vector2iCompare> roomGridOccupancy; // Maps grid position to room index

const float ROOM_WIDTH = 250.f;
const float ROOM_HEIGHT = 200.f;
const float LARGE_ROOM_WIDTH = 400.f;
const float LARGE_ROOM_HEIGHT = 350.f;
const float DOOR_VISUAL_SIZE = 30.f;
const float GRID_SPACING = 80.f;
const int MIN_ROOMS = 20;
const int MAX_ROOMS = 40;
const int NUM_LARGE_ROOMS = 3;
const int MIN_MAIN_PATH_LENGTH = 8;
const float BRANCH_MERGE_CHANCE = 0.3f;

sf::View gameView;
bool isPanning = false;
sf::Vector2f lastMousePosView;

std::mt19937 rng(std::random_device{}());

// --- Function Declarations ---
void generateProceduralMap();
DoorDirection getOppositeDirection(DoorDirection dir);
sf::Vector2i getOffsetForDirection(DoorDirection dir);
sf::Vector2f getRoomWorldPosition(const Room& room);
void setupRoomVisuals(Room& room, const std::string& fontPath);
sf::Color getDirectionColor(DoorDirection dir);
std::vector<DoorDirection> getValidDirections(sf::Vector2i pos, bool allowMerge = false);
int createRoom(sf::Vector2i gridPos, RoomType type = RoomType::Normal);
bool connectRooms(int room1Id, DoorDirection dir1, int room2Id);
void generateBranch(int startRoomId, DoorDirection startDir, int maxDepth, int currentDepth = 1);
void generateMainPath();
void placeLargeRooms();
void drawConnections(sf::RenderWindow& window);

// --- Main Game ---
MAIN_TEMPLATE_GAME_START

    gameView = window.getDefaultView();
    generateProceduralMap();

GAME_LOOP_START

        // Event Handling
        if (evt.type == sf::Event::KeyPressed) {
            if (evt.key.code == sf::Keyboard::R) {
                generateProceduralMap();
            }
        }
        if (evt.type == sf::Event::MouseButtonPressed) {
            if (evt.mouseButton.button == sf::Mouse::Middle) {
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
            }
        }
        if (evt.type == sf::Event::MouseWheelScrolled) {
            if (evt.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                float zoomFactor = (evt.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                gameView.zoom(zoomFactor);
            }
        }

        // Drawing
        window.setView(gameView);

        // Draw rooms
        for (const auto& room : generatedRooms) {
            window.draw(room.shape);
            for (const auto& door : room.doors) {
                window.draw(door.visualShape);
            }
            window.draw(room.idText);
        }
        
        // Draw connections
        drawConnections(window);

        window.setView(window.getDefaultView());
        DISPLAY_TEXT("Press R to regenerate. Middle mouse to pan. Scroll to zoom.");
        DISPLAY_TEXT("Rooms: " + std::to_string(generatedRooms.size()) + 
                    " | Large Rooms: " + std::to_string(std::count_if(generatedRooms.begin(), generatedRooms.end(), 
                    [](const Room& r) { return r.type == RoomType::Large; })));

MAIN_TEMPLATE_GAME_END

// --- Function Implementations ---

void generateProceduralMap() {
    generatedRooms.clear();
    roomGridOccupancy.clear();
    
    // 1. Generate main path from start to boss
    generateMainPath();
    
    // 2. Place large rooms on the main path
    placeLargeRooms();
    
    // 3. Setup visuals
    std::string fontPath = (std::filesystem::current_path().parent_path() / "SegoeUI.ttf").string();
    for (auto& room : generatedRooms) {
        setupRoomVisuals(room, fontPath);
    }
}

void generateMainPath() {
    // Create start room
    int startId = createRoom(sf::Vector2i(0, 0), RoomType::Normal);
    generatedRooms[startId].isStartRoom = true;
    generatedRooms[startId].isMainPath = true;
    
    // Generate main path
    int currentRoomId = startId;
    int pathLength = 0;
    int targetLength = MIN_MAIN_PATH_LENGTH + (rng() % 5);
    
    while (pathLength < targetLength && generatedRooms.size() < MAX_ROOMS) {
        Room& currentRoom = generatedRooms[currentRoomId];
        std::vector<DoorDirection> validDirs = getValidDirections(currentRoom.gridPosition);
        
        if (validDirs.empty()) break;
        
        // Pick a direction
        std::uniform_int_distribution<> dirDist(0, validDirs.size() - 1);
        DoorDirection chosenDir = validDirs[dirDist(rng)];
        
        // Add door to current room
        currentRoom.addDoor(chosenDir);
        
        // Create next room
        sf::Vector2i nextPos = currentRoom.gridPosition + getOffsetForDirection(chosenDir);
        int nextRoomId = createRoom(nextPos, RoomType::Normal);
        generatedRooms[nextRoomId].isMainPath = true;
        
        // Add opposite door to next room
        generatedRooms[nextRoomId].addDoor(getOppositeDirection(chosenDir));
        
        // Connect the rooms
        connectRooms(currentRoomId, chosenDir, nextRoomId);
        
        currentRoomId = nextRoomId;
        pathLength++;
    }
    
    // Mark last room as boss room
    if (currentRoomId != startId) {
        generatedRooms[currentRoomId].type = RoomType::Boss;
    }
}

void placeLargeRooms() {
    // Find suitable positions on main path for large rooms
    std::vector<int> mainPathRooms;
    for (int i = 0; i < generatedRooms.size(); i++) {
        if (generatedRooms[i].isMainPath && 
            generatedRooms[i].type != RoomType::Boss && 
            !generatedRooms[i].isStartRoom) {
            mainPathRooms.push_back(i);
        }
    }
    
    // Randomly select positions for large rooms
    std::shuffle(mainPathRooms.begin(), mainPathRooms.end(), rng);
    int largeRoomsPlaced = 0;
    
    for (int roomId : mainPathRooms) {
        if (largeRoomsPlaced >= NUM_LARGE_ROOMS) break;
        
        Room& room = generatedRooms[roomId];
        room.type = RoomType::Large;
        
        // Add additional doors for branching (2-4 extra doors)
        std::uniform_int_distribution<> extraDoorsDist(2, 4);
        int extraDoors = extraDoorsDist(rng);
        
        std::vector<DoorDirection> allDirs = {DoorDirection::Up, DoorDirection::Down, 
                                              DoorDirection::Left, DoorDirection::Right};
        
        // Remove already used directions
        for (const auto& door : room.doors) {
            allDirs.erase(std::remove(allDirs.begin(), allDirs.end(), door.direction), allDirs.end());
        }
        
        // Add new doors and generate branches
        std::shuffle(allDirs.begin(), allDirs.end(), rng);
        for (int i = 0; i < std::min(extraDoors, (int)allDirs.size()); i++) {
            room.addDoor(allDirs[i]);
            
            // Generate branch from this door
            std::uniform_int_distribution<> depthDist(3, 8);
            generateBranch(roomId, allDirs[i], depthDist(rng));
        }
        
        largeRoomsPlaced++;
    }
}

void generateBranch(int startRoomId, DoorDirection startDir, int maxDepth, int currentDepth) {
    if (generatedRooms.size() >= MAX_ROOMS || currentDepth > maxDepth) return;
    
    Room& startRoom = generatedRooms[startRoomId];
    sf::Vector2i nextPos = startRoom.gridPosition + getOffsetForDirection(startDir);
    
    // Check if we can merge with existing room
    if (roomGridOccupancy.count(nextPos) && currentDepth > 2) {
        std::uniform_real_distribution<> mergeDist(0.0, 1.0);
        if (mergeDist(rng) < BRANCH_MERGE_CHANCE) {
            int existingRoomId = roomGridOccupancy[nextPos];
            Room& existingRoom = generatedRooms[existingRoomId];
            
            // Check if the existing room can accept this connection
            DoorDirection oppositeDir = getOppositeDirection(startDir);
            if (!existingRoom.getDoor(oppositeDir)) {
                existingRoom.addDoor(oppositeDir);
                connectRooms(startRoomId, startDir, existingRoomId);
                return; // Branch merged successfully
            }
        }
    }
    
    // Create new room if position is free
    if (!roomGridOccupancy.count(nextPos)) {
        int newRoomId = createRoom(nextPos, RoomType::Normal);
        Room& newRoom = generatedRooms[newRoomId];
        newRoom.branchDepth = currentDepth;
        newRoom.addDoor(getOppositeDirection(startDir));
        
        connectRooms(startRoomId, startDir, newRoomId);
        
        // Decide if this room continues branching
        std::uniform_real_distribution<> continueDist(0.0, 1.0);
        float continueChance = 1.0f - (currentDepth / (float)maxDepth) * 0.5f;
        
        if (continueDist(rng) < continueChance) {
            // Add 1-2 more doors for continued branching
            std::vector<DoorDirection> validDirs = getValidDirections(newRoom.gridPosition, true);
            
            // Remove the entrance direction
            validDirs.erase(std::remove(validDirs.begin(), validDirs.end(), 
                          getOppositeDirection(startDir)), validDirs.end());
            
            if (!validDirs.empty()) {
                std::uniform_int_distribution<> numDoorsDist(1, std::min(2, (int)validDirs.size()));
                int numNewDoors = numDoorsDist(rng);
                
                std::shuffle(validDirs.begin(), validDirs.end(), rng);
                for (int i = 0; i < numNewDoors; i++) {
                    newRoom.addDoor(validDirs[i]);
                    generateBranch(newRoomId, validDirs[i], maxDepth, currentDepth + 1);
                }
            }
        } else {
            // This is a dead end
            newRoom.type = RoomType::DeadEnd;
        }
    }
}

int createRoom(sf::Vector2i gridPos, RoomType type) {
    int id = generatedRooms.size();
    generatedRooms.emplace_back(id, gridPos);
    generatedRooms.back().type = type;
    roomGridOccupancy[gridPos] = id;
    return id;
}

bool connectRooms(int room1Id, DoorDirection dir1, int room2Id) {
    Room& room1 = generatedRooms[room1Id];
    Room& room2 = generatedRooms[room2Id];
    
    Door* door1 = room1.getDoor(dir1);
    Door* door2 = room2.getDoor(getOppositeDirection(dir1));
    
    if (door1 && door2) {
        door1->isConnected = true;
        door1->connectedRoomId = room2Id;
        door2->isConnected = true;
        door2->connectedRoomId = room1Id;
        return true;
    }
    return false;
}

std::vector<DoorDirection> getValidDirections(sf::Vector2i pos, bool allowMerge) {
    std::vector<DoorDirection> valid;
    DoorDirection allDirs[] = {DoorDirection::Up, DoorDirection::Down, 
                              DoorDirection::Left, DoorDirection::Right};
    
    for (DoorDirection dir : allDirs) {
        sf::Vector2i nextPos = pos + getOffsetForDirection(dir);
        if (!roomGridOccupancy.count(nextPos) || allowMerge) {
            valid.push_back(dir);
        }
    }
    
    return valid;
}

void setupRoomVisuals(Room& room, const std::string& fontPath) {
    // Set room size based on type
    float width = (room.type == RoomType::Large) ? LARGE_ROOM_WIDTH : ROOM_WIDTH;
    float height = (room.type == RoomType::Large) ? LARGE_ROOM_HEIGHT : ROOM_HEIGHT;
    
    room.shape.setSize(sf::Vector2f(width, height));
    room.shape.setOrigin(width / 2.f, height / 2.f);
    room.shape.setPosition(getRoomWorldPosition(room));
    
    // Set room color based on type
    if (room.isStartRoom) {
        room.shape.setFillColor(sf::Color(50, 100, 50));
        room.shape.setOutlineColor(sf::Color::Green);
    } else if (room.type == RoomType::Boss) {
        room.shape.setFillColor(sf::Color(100, 50, 50));
        room.shape.setOutlineColor(sf::Color::Red);
    } else if (room.type == RoomType::Large) {
        room.shape.setFillColor(sf::Color(60, 60, 100));
        room.shape.setOutlineColor(sf::Color(150, 150, 255));
    } else if (room.type == RoomType::DeadEnd) {
        room.shape.setFillColor(sf::Color(80, 60, 60));
        room.shape.setOutlineColor(sf::Color(200, 150, 150));
    } else if (room.isMainPath) {
        room.shape.setFillColor(sf::Color(70, 70, 70));
        room.shape.setOutlineColor(sf::Color::White);
    } else {
        room.shape.setFillColor(sf::Color(60, 60, 60));
        room.shape.setOutlineColor(sf::Color(180, 180, 180));
    }
    
    room.shape.setOutlineThickness(3.f);
    
    // Setup text
    std::string label = std::to_string(room.id);
    if (room.type == RoomType::Large) label += " (L)";
    if (room.type == RoomType::Boss) label += " (BOSS)";
    if (room.isStartRoom) label += " (START)";
    
    room.idText = Object::CreateText(label, fontPath, 20, sf::Color::Cyan);
    sf::FloatRect textBounds = room.idText.getLocalBounds();
    room.idText.setOrigin(textBounds.left + textBounds.width / 2.f, 
                         textBounds.top + textBounds.height / 2.f);
    room.idText.setPosition(room.shape.getPosition());
    
    // Setup door visuals
    for (auto& door : room.doors) {
        door.visualShape.setSize(sf::Vector2f(DOOR_VISUAL_SIZE, DOOR_VISUAL_SIZE));
        door.visualShape.setOrigin(DOOR_VISUAL_SIZE / 2.f, DOOR_VISUAL_SIZE / 2.f);
        
        sf::Vector2f roomPos = room.shape.getPosition();
        float halfW = width / 2.f;
        float halfH = height / 2.f;
        
        switch (door.direction) {
            case DoorDirection::Right:
                door.visualShape.setPosition(roomPos.x + halfW, roomPos.y);
                break;
            case DoorDirection::Left:
                door.visualShape.setPosition(roomPos.x - halfW, roomPos.y);
                break;
            case DoorDirection::Up:
                door.visualShape.setPosition(roomPos.x, roomPos.y - halfH);
                break;
            case DoorDirection::Down:
                door.visualShape.setPosition(roomPos.x, roomPos.y + halfH);
                break;
        }
        
        // Color based on connection status and room type
        if (!door.isConnected) {
            door.visualShape.setFillColor(sf::Color(100, 0, 0)); // Dark red for unconnected
        } else if (room.isStartRoom && door.direction == getOppositeDirection(DoorDirection::None)) {
            door.visualShape.setFillColor(sf::Color::Yellow); // Entrance
        } else if (room.type == RoomType::Boss) {
            door.visualShape.setFillColor(sf::Color::Green); // Exit
        } else {
            door.visualShape.setFillColor(getDirectionColor(door.direction));
        }
        
        door.visualShape.setOutlineColor(sf::Color::White);
        door.visualShape.setOutlineThickness(2.f);
    }
}

void drawConnections(sf::RenderWindow& window) {
    for (const auto& room : generatedRooms) {
        for (const auto& door : room.doors) {
            if (door.isConnected && door.connectedRoomId > room.id) {
                sf::Vector2f p1 = door.visualShape.getPosition();
                
                const Room& connectedRoom = generatedRooms[door.connectedRoomId];
                Door* oppositeDoor = nullptr;
                for (const auto& d : connectedRoom.doors) {
                    if (d.connectedRoomId == room.id) {
                        oppositeDoor = const_cast<Door*>(&d);
                        break;
                    }
                }
                
                if (oppositeDoor) {
                    sf::Vector2f p2 = oppositeDoor->visualShape.getPosition();
                    
                    sf::Color lineColor = room.isMainPath && connectedRoom.isMainPath ? 
                                         sf::Color::White : sf::Color(150, 150, 150);
                    
                    sf::Vertex line[] = {
                        sf::Vertex(p1, lineColor),
                        sf::Vertex(p2, lineColor)
                    };
                    window.draw(line, 2, sf::Lines);
                }
            }
        }
    }
}

DoorDirection getOppositeDirection(DoorDirection dir) {
    switch (dir) {
        case DoorDirection::Right: return DoorDirection::Left;
        case DoorDirection::Left: return DoorDirection::Right;
        case DoorDirection::Up: return DoorDirection::Down;
        case DoorDirection::Down: return DoorDirection::Up;
        default: return DoorDirection::None;
    }
}

sf::Vector2i getOffsetForDirection(DoorDirection dir) {
    switch (dir) {
        case DoorDirection::Right: return sf::Vector2i(1, 0);
        case DoorDirection::Left: return sf::Vector2i(-1, 0);
        case DoorDirection::Up: return sf::Vector2i(0, -1);
        case DoorDirection::Down: return sf::Vector2i(0, 1);
        default: return sf::Vector2i(0, 0);
    }
}

sf::Vector2f getRoomWorldPosition(const Room& room) {
    float width = (room.type == RoomType::Large) ? LARGE_ROOM_WIDTH : ROOM_WIDTH;
    float height = (room.type == RoomType::Large) ? LARGE_ROOM_HEIGHT : ROOM_HEIGHT;
    
    return sf::Vector2f(
        room.gridPosition.x * (ROOM_WIDTH + GRID_SPACING),
        room.gridPosition.y * (ROOM_HEIGHT + GRID_SPACING)
    );
}

sf::Color getDirectionColor(DoorDirection dir) {
    switch (dir) {
        case DoorDirection::Right: return sf::Color(255, 100, 100);
        case DoorDirection::Left: return sf::Color(100, 100, 255);
        case DoorDirection::Up: return sf::Color(100, 255, 100);
        case DoorDirection::Down: return sf::Color(255, 255, 100);
        default: return sf::Color::Transparent;
    }
}
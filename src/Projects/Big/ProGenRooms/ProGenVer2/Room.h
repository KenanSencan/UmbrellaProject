// Room.h - Room class definition
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Vector2.h"

enum class DoorDirection {
    Right, Left, Up, Down, None
};

enum class RoomType {
    Start, Normal, Large, Boss, Treasure, Shop, Secret, DeadEnd
};

struct Door {
    DoorDirection direction;
    bool isConnected = false;
    int connectedRoomId = -1;
    Vector2 worldPosition;
    
    Door(DoorDirection dir) : direction(dir) {}
};

struct RoomTemplate {
    std::string name;
    Vector2 size;
    RoomType type;
    std::vector<DoorDirection> possibleDoors;
    int weight = 10; // For weighted random selection
    
    RoomTemplate(const std::string& n, Vector2 s, RoomType t, 
                 const std::vector<DoorDirection>& doors, int w = 10)
        : name(n), size(s), type(t), possibleDoors(doors), weight(w) {}
};

class Room {
    
public:
    int id;
    Vector2 position;
    Vector2 size;
    RoomType type;
    int templateIndex;
    static int nextId;
    
    
    std::vector<std::unique_ptr<Door>> doors;
    
    // Pathfinding related
    bool isMainPath = false;
    int branchDepth = 0;
    int distanceFromStart = 0;
    
    // Visual/Debug
    bool hasBeenRelocated = false;
    bool isVisible = false;
    
    Room(Vector2 pos, Vector2 size, RoomType type, int templateIdx = -1)
        : id(nextId++), position(pos), size(size), type(type), 
          templateIndex(templateIdx) {}
    
    // Add a door in the specified direction
    void addDoor(DoorDirection direction) {
        // Check if door already exists
        for (const auto& door : doors) {
            if (door->direction == direction) return;
        }
        doors.push_back(std::make_unique<Door>(direction));
    }
    
    // Get door by direction
    Door* getDoor(DoorDirection direction) {
        for (auto& door : doors) {
            if (door->direction == direction) {
                return door.get();
            }
        }
        return nullptr;
    }
    
    // Get room bounds
    Rect getBounds() const {
        return Rect(position.x - size.x / 2, 
                   position.y - size.y / 2,
                   size.x, size.y);
    }
    
    // Get door world position
    Vector2 getDoorPosition(DoorDirection dir) const {
        Vector2 doorPos = position;
        
        switch (dir) {
            case DoorDirection::Right:
                doorPos.x += size.x / 2;
                break;
            case DoorDirection::Left:
                doorPos.x -= size.x / 2;
                break;
            case DoorDirection::Up:
                doorPos.y -= size.y / 2;
                break;
            case DoorDirection::Down:
                doorPos.y += size.y / 2;
                break;
        }
        
        return doorPos;
    }
    
    // Get available door directions
    std::vector<DoorDirection> getAvailableDirections() const {
        std::vector<DoorDirection> available;
        std::vector<DoorDirection> allDirs = {
            DoorDirection::Up, DoorDirection::Down,
            DoorDirection::Left, DoorDirection::Right
        };
        
        for (auto dir : allDirs) {
            bool exists = false;
            for (const auto& door : doors) {
                if (door->direction == dir) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                available.push_back(dir);
            }
        }
        
        return available;
    }
    
    // Connect this room to another
    bool connectTo(int otherRoomId, DoorDirection direction) {
        Door* door = getDoor(direction);
        if (door) {
            door->isConnected = true;
            door->connectedRoomId = otherRoomId;
            door->worldPosition = getDoorPosition(direction);
            return true;
        }
        return false;
    }
};

int Room::nextId = 0;

// Helper functions
inline DoorDirection getOppositeDirection(DoorDirection dir) {
    switch (dir) {
        case DoorDirection::Right: return DoorDirection::Left;
        case DoorDirection::Left: return DoorDirection::Right;
        case DoorDirection::Up: return DoorDirection::Down;
        case DoorDirection::Down: return DoorDirection::Up;
        default: return DoorDirection::None;
    }
}

inline Vector2 getDirectionOffset(DoorDirection dir, float distance) {
    switch (dir) {
        case DoorDirection::Right: return Vector2(distance, 0);
        case DoorDirection::Left: return Vector2(-distance, 0);
        case DoorDirection::Up: return Vector2(0, -distance);
        case DoorDirection::Down: return Vector2(0, distance);
        default: return Vector2(0, 0);
    }
}
// DungeonGenerator.h - Main dungeon generation system
#pragma once
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <unordered_set>
#include "Room.h"
#include "SpatialGrid.h"
#include "AStar.h"

struct Corridor {
    std::vector<Vector2> path;
    int roomId1, roomId2;
    bool isMainPath;
    
    Corridor(int r1, int r2, bool main = false) 
        : roomId1(r1), roomId2(r2), isMainPath(main) {}
};

struct GenerationSettings {
    // Room counts
    int minRooms = 15;
    int maxRooms = 35;
    int minMainPathLength = 8;
    int maxMainPathLength = 12;
    
    // Spacing
    float roomSpacing = 150.0f;
    float corridorWidth = 16.0f;
    float collisionPadding = 20.0f;
    
    // Probabilities
    float branchProbability = 0.6f;
    float loopProbability = 0.2f;
    float secretRoomProbability = 0.1f;
    
    // Performance
    int maxRelocationAttempts = 50;
    float relocationSearchRadius = 500.0f;
    
    // Seed
    unsigned int seed = 0;
};

class DungeonGenerator {
private:
    GenerationSettings settings;
    std::mt19937 rng;
    
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<std::unique_ptr<Corridor>> corridors;
    std::vector<RoomTemplate> templates;
    
    SpatialGrid spatialGrid;
    AStar pathfinder;
    
    // Generation state
    std::vector<int> mainPath;
    std::unordered_set<int> processedRooms;
    
public:
    DungeonGenerator(const GenerationSettings& settings = GenerationSettings())
        : settings(settings), rng(settings.seed ? settings.seed : std::random_device{}()),
          spatialGrid(settings.roomSpacing), 
          pathfinder(settings.corridorWidth, 0.5f) {
        
        initializeTemplates();
        pathfinder.setRooms(&rooms);
    }
    
    // Initialize room templates
    void initializeTemplates() {
        templates = {
            // Small rooms
            {"Tiny Square", Vector2(40, 40), RoomType::Normal, 
             {DoorDirection::Up, DoorDirection::Down, DoorDirection::Left, DoorDirection::Right}, 15},
            {"Small Horizontal", Vector2(60, 40), RoomType::Normal,
             {DoorDirection::Left, DoorDirection::Right}, 12},
            {"Small Vertical", Vector2(40, 60), RoomType::Normal,
             {DoorDirection::Up, DoorDirection::Down}, 12},
             
            // Medium rooms
            {"Medium Square", Vector2(80, 80), RoomType::Normal,
             {DoorDirection::Up, DoorDirection::Down, DoorDirection::Left, DoorDirection::Right}, 20},
            {"Medium Wide", Vector2(120, 80), RoomType::Normal,
             {DoorDirection::Left, DoorDirection::Right, DoorDirection::Up, DoorDirection::Down}, 15},
            {"Medium Tall", Vector2(80, 120), RoomType::Normal,
             {DoorDirection::Up, DoorDirection::Down, DoorDirection::Left, DoorDirection::Right}, 15},
             
            // Large rooms
            {"Large Square", Vector2(150, 150), RoomType::Large,
             {DoorDirection::Up, DoorDirection::Down, DoorDirection::Left, DoorDirection::Right}, 5},
            {"Large Hall", Vector2(200, 100), RoomType::Large,
             {DoorDirection::Left, DoorDirection::Right, DoorDirection::Up, DoorDirection::Down}, 3},
             
            // Special rooms
            {"Boss Chamber", Vector2(200, 200), RoomType::Boss,
             {DoorDirection::Up, DoorDirection::Down, DoorDirection::Left, DoorDirection::Right}, 1},
            {"Treasure Room", Vector2(60, 60), RoomType::Treasure,
             {DoorDirection::Down}, 5},
            {"Shop", Vector2(100, 80), RoomType::Shop,
             {DoorDirection::Up, DoorDirection::Down, DoorDirection::Left, DoorDirection::Right}, 3}
        };
    }
    
    // Main generation function
    void generate() {
        clear();
        
        // Step 1: Generate main path
        generateMainPath();
        
        // Step 2: Add branch rooms
        generateBranches();
        
        // Step 3: Resolve collisions efficiently
        resolveCollisions();
        
        // Step 4: Add special rooms
        addSpecialRooms();
        
        // Step 5: Create corridors for relocated rooms
        createCorridors();
        
        // Step 6: Optimize and add loops
        optimizeLayout();
    }
    
    // Clear all data
    void clear() {
        rooms.clear();
        corridors.clear();
        mainPath.clear();
        processedRooms.clear();
        spatialGrid.clear();
        Room::nextId = 0;
    }
    
private:
    // Generate the main path from start to boss
    void generateMainPath() {
        // Create start room
        auto& startTemplate = selectTemplate(RoomType::Start);
        auto startRoom = createRoom(Vector2(0, 0), startTemplate);
        startRoom->type = RoomType::Start;
        startRoom->isMainPath = true;
        rooms.push_back(std::move(startRoom));
        spatialGrid.addRoom(*rooms.back());
        mainPath.push_back(0);
        
        // Generate path
        std::uniform_int_distribution<int> pathLengthDist(settings.minMainPathLength, settings.maxMainPathLength);
        int targetLength = pathLengthDist(rng);
        
        int currentRoomId = 0;
        DoorDirection lastDir = DoorDirection::None;
        
        for (int i = 1; i < targetLength; i++) {
            Room* currentRoom = rooms[currentRoomId].get();
            
            // Choose direction (prefer forward movement)
            DoorDirection chosenDir = selectNextDirection(lastDir);
            
            // Calculate next room position
            float spacing = currentRoom->size.length() / 2 + settings.roomSpacing;
            Vector2 offset = getDirectionOffset(chosenDir, spacing);
            Vector2 nextPos = currentRoom->position + offset;
            
            // Select template for next room
            RoomType nextType = (i == targetLength - 1) ? RoomType::Boss : RoomType::Normal;
            auto& nextTemplate = selectTemplate(nextType);
            
            // Create room
            auto nextRoom = createRoom(nextPos, nextTemplate);
            nextRoom->isMainPath = true;
            if (i == targetLength - 1) {
                nextRoom->type = RoomType::Boss;
            }
            
            // Add doors and connect
            currentRoom->addDoor(chosenDir);
            nextRoom->addDoor(getOppositeDirection(chosenDir));
            
            int nextRoomId = rooms.size();
            currentRoom->connectTo(nextRoomId, chosenDir);
            nextRoom->connectTo(currentRoomId, getOppositeDirection(chosenDir));
            
            rooms.push_back(std::move(nextRoom));
            spatialGrid.addRoom(*rooms.back());
            mainPath.push_back(nextRoomId);
            
            currentRoomId = nextRoomId;
            lastDir = chosenDir;
        }
    }
    
    // Generate branch rooms
    void generateBranches() {
        std::uniform_real_distribution<> probDist(0.0, 1.0);
        int branchesAdded = 0;
        int maxBranches = (settings.maxRooms - mainPath.size()) * 0.7f;
        
        // Try to branch from main path rooms
        for (int roomId : mainPath) {
            if (roomId == 0 || roomId == mainPath.back()) continue; // Skip start and boss
            if (branchesAdded >= maxBranches) break;
            
            Room* room = rooms[roomId].get();
            auto availableDirs = room->getAvailableDirections();
            
            for (DoorDirection dir : availableDirs) {
                if (probDist(rng) < settings.branchProbability) {
                    if (addBranchRoom(roomId, dir)) {
                        branchesAdded++;
                        
                        // Sometimes add a chain of rooms
                        if (probDist(rng) < 0.3f) {
                            int chainLength = std::uniform_int_distribution<>(1, 3)(rng);
                            int lastBranchId = rooms.size() - 1;
                            
                            for (int i = 0; i < chainLength; i++) {
                                auto dirs = rooms[lastBranchId]->getAvailableDirections();
                                if (!dirs.empty()) {
                                    DoorDirection nextDir = dirs[rng() % dirs.size()];
                                    if (addBranchRoom(lastBranchId, nextDir)) {
                                        branchesAdded++;
                                        lastBranchId = rooms.size() - 1;
                                    } else {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Add a branch room
    bool addBranchRoom(int sourceRoomId, DoorDirection direction) {
        Room* sourceRoom = rooms[sourceRoomId].get();
        
        // Calculate position
        float spacing = sourceRoom->size.length() / 2 + settings.roomSpacing;
        Vector2 offset = getDirectionOffset(direction, spacing);
        Vector2 newPos = sourceRoom->position + offset;
        
        // Select template
        auto& template_ = selectTemplate(RoomType::Normal);
        
        // Check if position is valid (quick check)
        Rect testBounds(newPos.x - template_.size.x/2, 
                       newPos.y - template_.size.y/2,
                       template_.size.x, template_.size.y);
        
        if (!spatialGrid.isAreaClear(testBounds.expanded(settings.collisionPadding))) {
            return false;
        }
        
        // Create room
        auto newRoom = createRoom(newPos, template_);
        newRoom->branchDepth = sourceRoom->branchDepth + 1;
        
        // Add doors and connect
        sourceRoom->addDoor(direction);
        newRoom->addDoor(getOppositeDirection(direction));
        
        int newRoomId = rooms.size();
        sourceRoom->connectTo(newRoomId, direction);
        newRoom->connectTo(sourceRoomId, getOppositeDirection(direction));
        
        rooms.push_back(std::move(newRoom));
        spatialGrid.addRoom(*rooms.back());
        
        return true;
    }
    
    // Resolve collisions efficiently
    void resolveCollisions() {
        for (size_t i = 1; i < rooms.size(); i++) { // Skip start room
            Room* room = rooms[i].get();
            
            if (room->type == RoomType::Boss) continue; // Don't move boss room
            
            auto potentialCollisions = spatialGrid.getPotentialCollisions(*room);
            
            bool hasCollision = false;
            for (int otherId : potentialCollisions) {
                if (otherId < i) { // Only check against earlier rooms
                    Rect bounds1 = room->getBounds().expanded(settings.collisionPadding);
                    Rect bounds2 = rooms[otherId]->getBounds().expanded(settings.collisionPadding);
                    
                    if (bounds1.intersects(bounds2)) {
                        hasCollision = true;
                        break;
                    }
                }
            }
            
            if (hasCollision) {
                relocateRoom(room);
            }
        }
    }
    
    // Relocate a room using smart search
    void relocateRoom(Room* room) {
        Vector2 originalPos = room->position;
        room->hasBeenRelocated = true;
        
        // Use golden angle spiral for better distribution
        const float goldenAngle = 137.5f * 3.14159f / 180.0f;
        float currentAngle = 0;
        float currentRadius = settings.roomSpacing;
        
        for (int attempt = 0; attempt < settings.maxRelocationAttempts; attempt++) {
            // Calculate test position
            Vector2 testPos = originalPos + Vector2(
                std::cos(currentAngle) * currentRadius,
                std::sin(currentAngle) * currentRadius
            );
            
            // Update room position temporarily
            room->position = testPos;
            
            // Check if new position is valid
            Rect testBounds = room->getBounds().expanded(settings.collisionPadding);
            if (spatialGrid.isAreaClear(testBounds, room->id)) {
                // Check if we can create a corridor
                bool canConnect = false;
                for (const auto& door : room->doors) {
                    if (door->isConnected) {
                        Room* connectedRoom = rooms[door->connectedRoomId].get();
                        Vector2 doorPos = room->getDoorPosition(door->direction);
                        Vector2 targetPos = connectedRoom->getDoorPosition(
                            getOppositeDirection(door->direction));
                        
                        if (pathfinder.hasDirectPath(doorPos, targetPos, room->id, 
                                                     door->connectedRoomId)) {
                            canConnect = true;
                            break;
                        }
                    }
                }
                
                if (canConnect || attempt > settings.maxRelocationAttempts / 2) {
                    // Update spatial grid
                    spatialGrid.updateRoom(*room);
                    return;
                }
            }
            
            // Update spiral position
            currentAngle += goldenAngle;
            if (attempt % 8 == 0) {
                currentRadius += settings.roomSpacing * 0.5f;
            }
        }
        
        // If we couldn't find a good position, place it far away
        room->position = originalPos + Vector2(
            settings.relocationSearchRadius * 2,
            settings.relocationSearchRadius * 2
        );
        spatialGrid.updateRoom(*room);
    }
    
    // Add special rooms
    void addSpecialRooms() {
        std::uniform_real_distribution<> probDist(0.0, 1.0);
        
        // Add treasure rooms
        for (auto& room : rooms) {
            if (room->type == RoomType::Normal && room->branchDepth > 0) {
                if (probDist(rng) < 0.2f) {
                    auto availableDirs = room->getAvailableDirections();
                    if (!availableDirs.empty()) {
                        DoorDirection dir = availableDirs[rng() % availableDirs.size()];
                        addSpecialRoom(room->id, dir, RoomType::Treasure);
                    }
                }
            }
        }
    }
    
    // Add a special room
    bool addSpecialRoom(int sourceRoomId, DoorDirection direction, RoomType type) {
        Room* sourceRoom = rooms[sourceRoomId].get();
        
        auto& template_ = selectTemplate(type);
        float spacing = sourceRoom->size.length() / 2 + settings.roomSpacing;
        Vector2 offset = getDirectionOffset(direction, spacing);
        Vector2 newPos = sourceRoom->position + offset;
        
        auto newRoom = createRoom(newPos, template_);
        newRoom->type = type;
        
        sourceRoom->addDoor(direction);
        newRoom->addDoor(getOppositeDirection(direction));
        
        int newRoomId = rooms.size();
        sourceRoom->connectTo(newRoomId, direction);
        newRoom->connectTo(sourceRoomId, getOppositeDirection(direction));
        
        rooms.push_back(std::move(newRoom));
        spatialGrid.addRoom(*rooms.back());
        
        return true;
    }
    
    // Create corridors for relocated rooms
    void createCorridors() {
        for (auto& room : rooms) {
            if (!room->hasBeenRelocated) continue;
            
            for (auto& door : room->doors) {
                if (door->isConnected) {
                    Room* targetRoom = rooms[door->connectedRoomId].get();
                    Vector2 startPos = room->getDoorPosition(door->direction);
                    Vector2 endPos = targetRoom->getDoorPosition(
                        getOppositeDirection(door->direction));
                    
                    auto path = pathfinder.findPath(startPos, endPos, 
                                                   room->id, door->connectedRoomId);
                    
                    if (!path.empty()) {
                        auto corridor = std::make_unique<Corridor>(
                            room->id, door->connectedRoomId, room->isMainPath);
                        corridor->path = path;
                        corridors.push_back(std::move(corridor));
                    }
                }
            }
        }
    }
    
    // Optimize layout and add loops
    void optimizeLayout() {
        // Add some loop connections for better flow
        std::uniform_real_distribution<> probDist(0.0, 1.0);
        
        for (size_t i = 0; i < rooms.size(); i++) {
            for (size_t j = i + 1; j < rooms.size(); j++) {
                if (probDist(rng) < settings.loopProbability) {
                    Room* room1 = rooms[i].get();
                    Room* room2 = rooms[j].get();
                    
                    float distance = room1->position.distance(room2->position);
                    if (distance < settings.roomSpacing * 3) {
                        // Try to connect if they have available doors
                        auto dirs1 = room1->getAvailableDirections();
                        auto dirs2 = room2->getAvailableDirections();
                        
                        if (!dirs1.empty() && !dirs2.empty()) {
                            // Find best direction pair
                            DoorDirection bestDir1 = dirs1[0];
                            DoorDirection bestDir2 = dirs2[0];
                            float bestDist = 999999;
                            
                            for (auto d1 : dirs1) {
                                Vector2 pos1 = room1->getDoorPosition(d1);
                                for (auto d2 : dirs2) {
                                    Vector2 pos2 = room2->getDoorPosition(d2);
                                    float dist = pos1.distance(pos2);
                                    if (dist < bestDist) {
                                        bestDist = dist;
                                        bestDir1 = d1;
                                        bestDir2 = d2;
                                    }
                                }
                            }
                            
                            // Create connection
                            room1->addDoor(bestDir1);
                            room2->addDoor(bestDir2);
                            room1->connectTo(j, bestDir1);
                            room2->connectTo(i, bestDir2);
                            
                            // Add corridor if needed
                            if (bestDist > settings.roomSpacing) {
                                auto path = pathfinder.findPath(
                                    room1->getDoorPosition(bestDir1),
                                    room2->getDoorPosition(bestDir2),
                                    i, j);
                                
                                if (!path.empty()) {
                                    auto corridor = std::make_unique<Corridor>(i, j, false);
                                    corridor->path = path;
                                    corridors.push_back(std::move(corridor));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Helper functions
    std::unique_ptr<Room> createRoom(const Vector2& position, const RoomTemplate& template_) {
        auto room = std::make_unique<Room>(position, template_.size, 
                                          template_.type, 
                                          &template_ - &templates[0]);
        return room;
    }
    
    RoomTemplate& selectTemplate(RoomType preferredType) {
        std::vector<int> weights;
        std::vector<int> indices;
        
        for (size_t i = 0; i < templates.size(); i++) {
            if (preferredType == RoomType::Start || 
                templates[i].type == preferredType || 
                (preferredType == RoomType::Normal && 
                 templates[i].type != RoomType::Boss && 
                 templates[i].type != RoomType::Treasure)) {
                weights.push_back(templates[i].weight);
                indices.push_back(i);
            }
        }
        
        if (weights.empty()) {
            return templates[0]; // Fallback
        }
        
        std::discrete_distribution<> dist(weights.begin(), weights.end());
        return templates[indices[dist(rng)]];
    }
    
    DoorDirection selectNextDirection(DoorDirection lastDir) {
        std::vector<DoorDirection> dirs = {
            DoorDirection::Right, DoorDirection::Down, 
            DoorDirection::Left, DoorDirection::Up
        };
        
        // Remove opposite of last direction
        if (lastDir != DoorDirection::None) {
            dirs.erase(std::remove(dirs.begin(), dirs.end(), 
                                  getOppositeDirection(lastDir)), dirs.end());
        }
        
        // Weighted selection (prefer continuing in same direction)
        if (lastDir != DoorDirection::None) {
            for (int i = 0; i < 2; i++) { // Add weight to continuing
                dirs.push_back(lastDir);
            }
        }
        
        return dirs[rng() % dirs.size()];
    }
    
public:
    // Getters for visualization
    const std::vector<std::unique_ptr<Room>>& getRooms() const { return rooms; }
    const std::vector<std::unique_ptr<Corridor>>& getCorridors() const { return corridors; }
    const SpatialGrid& getSpatialGrid() const { return spatialGrid; }
    const std::vector<int>& getMainPath() const { return mainPath; }
};
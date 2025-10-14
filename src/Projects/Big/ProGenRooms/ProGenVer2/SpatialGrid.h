// SpatialGrid.h - Efficient spatial partitioning system
#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Vector2.h"
#include "Room.h"

// Grid cell for spatial hashing
struct GridCell {
    int x, y;
    
    GridCell(int x, int y) : x(x), y(y) {}
    
    bool operator==(const GridCell& other) const {
        return x == other.x && y == other.y;
    }
};

struct GridCellHash {
    std::size_t operator()(const GridCell& cell) const {
        return std::hash<int>()(cell.x) ^ (std::hash<int>()(cell.y) << 1);
    }
};

// Spatial grid for efficient collision detection
class SpatialGrid {
private:
    float cellSize;
    std::unordered_map<GridCell, std::vector<int>, GridCellHash> grid;
    std::unordered_map<int, std::vector<GridCell>> roomCells;
    
public:
    SpatialGrid(float cellSize = 100.0f) : cellSize(cellSize) {}
    
    // Convert world position to grid cell
    GridCell worldToGrid(const Vector2& pos) const {
        return GridCell(
            static_cast<int>(std::floor(pos.x / cellSize)),
            static_cast<int>(std::floor(pos.y / cellSize))
        );
    }
    
    // Get all cells that a room occupies
    std::vector<GridCell> getRoomCells(const Room& room) const {
        std::vector<GridCell> cells;
        Rect bounds = room.getBounds();
        
        int minX = static_cast<int>(std::floor(bounds.x / cellSize));
        int maxX = static_cast<int>(std::ceil((bounds.x + bounds.width) / cellSize));
        int minY = static_cast<int>(std::floor(bounds.y / cellSize));
        int maxY = static_cast<int>(std::ceil((bounds.y + bounds.height) / cellSize));
        
        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                cells.push_back(GridCell(x, y));
            }
        }
        
        return cells;
    }
    
    // Add room to spatial grid
    void addRoom(const Room& room) {
        auto cells = getRoomCells(room);
        roomCells[room.id] = cells;
        
        for (const auto& cell : cells) {
            grid[cell].push_back(room.id);
        }
    }
    
    // Remove room from spatial grid
    void removeRoom(int roomId) {
        if (roomCells.find(roomId) != roomCells.end()) {
            for (const auto& cell : roomCells[roomId]) {
                auto& cellRooms = grid[cell];
                cellRooms.erase(
                    std::remove(cellRooms.begin(), cellRooms.end(), roomId),
                    cellRooms.end()
                );
                
                if (cellRooms.empty()) {
                    grid.erase(cell);
                }
            }
            roomCells.erase(roomId);
        }
    }
    
    // Update room position in grid
    void updateRoom(const Room& room) {
        removeRoom(room.id);
        addRoom(room);
    }
    
    // Get potential colliding rooms
    std::unordered_set<int> getPotentialCollisions(const Room& room) const {
        std::unordered_set<int> potentialCollisions;
        auto cells = getRoomCells(room);
        
        for (const auto& cell : cells) {
            auto it = grid.find(cell);
            if (it != grid.end()) {
                for (int roomId : it->second) {
                    if (roomId != room.id) {
                        potentialCollisions.insert(roomId);
                    }
                }
            }
        }
        
        return potentialCollisions;
    }
    
    // Check if a position is clear
    bool isAreaClear(const Rect& bounds, int excludeRoomId = -1) const {
        int minX = static_cast<int>(std::floor(bounds.x / cellSize));
        int maxX = static_cast<int>(std::ceil((bounds.x + bounds.width) / cellSize));
        int minY = static_cast<int>(std::floor(bounds.y / cellSize));
        int maxY = static_cast<int>(std::ceil((bounds.y + bounds.height) / cellSize));
        
        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                auto it = grid.find(GridCell(x, y));
                if (it != grid.end()) {
                    for (int roomId : it->second) {
                        if (roomId != excludeRoomId) {
                            return false;
                        }
                    }
                }
            }
        }
        
        return true;
    }
    
    // Get all occupied cells (for visualization)
    std::vector<GridCell> getAllOccupiedCells() const {
        std::vector<GridCell> cells;
        for (const auto& pair : grid) {
            if (!pair.second.empty()) {
                cells.push_back(pair.first);
            }
        }
        return cells;
    }
    
    // Clear the grid
    void clear() {
        grid.clear();
        roomCells.clear();
    }
};
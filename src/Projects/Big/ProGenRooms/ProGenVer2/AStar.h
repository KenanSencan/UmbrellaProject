// AStar.h - Optimized A* pathfinding
#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include "Vector2.h"
#include "Room.h"

struct PathNode {
    Vector2 position;
    float g, h, f;
    std::shared_ptr<PathNode> parent;
    
    PathNode(const Vector2& pos, float gCost, float hCost, 
             std::shared_ptr<PathNode> par = nullptr)
        : position(pos), g(gCost), h(hCost), f(gCost + hCost), parent(par) {}
};

struct PathNodeCompare {
    bool operator()(const std::shared_ptr<PathNode>& a, 
                   const std::shared_ptr<PathNode>& b) const {
        return a->f > b->f;
    }
};

class AStar {
private:
    float gridSize;
    float turnPenalty;
    const std::vector<std::unique_ptr<Room>>* rooms;
    
    // Convert world position to grid position
    Vector2 toGrid(const Vector2& worldPos) const {
        return Vector2(
            std::round(worldPos.x / gridSize),
            std::round(worldPos.y / gridSize)
        );
    }
    
    // Convert grid position to world position
    Vector2 toWorld(const Vector2& gridPos) const {
        return Vector2(gridPos.x * gridSize, gridPos.y * gridSize);
    }
    
    // Calculate heuristic (Manhattan distance with tie-breaking)
    float heuristic(const Vector2& a, const Vector2& b) const {
        float dx = std::abs(a.x - b.x);
        float dy = std::abs(a.y - b.y);
        return (dx + dy) + 0.001f * std::min(dx, dy); // Tie-breaking
    }
    
    // Check if a position is blocked by rooms
    bool isBlocked(const Vector2& gridPos, int excludeRoom1, int excludeRoom2) const {
        Vector2 worldPos = toWorld(gridPos);
        Rect testBounds(worldPos.x - gridSize/2, worldPos.y - gridSize/2, 
                       gridSize, gridSize);
        
        for (const auto& room : *rooms) {
            if (room->id == excludeRoom1 || room->id == excludeRoom2) {
                continue;
            }
            
            Rect roomBounds = room->getBounds().expanded(5); // Small padding
            if (roomBounds.intersects(testBounds)) {
                return true;
            }
        }
        
        return false;
    }
    
public:
    AStar(float gridSize = 16.0f, float turnPenalty = 0.5f)
        : gridSize(gridSize), turnPenalty(turnPenalty), rooms(nullptr) {}
    
    void setRooms(const std::vector<std::unique_ptr<Room>>* roomList) {
        rooms = roomList;
    }
    
    // Find path between two points
    std::vector<Vector2> findPath(const Vector2& start, const Vector2& end,
                                  int startRoomId = -1, int endRoomId = -1) {
        std::vector<Vector2> path;
        
        if (!rooms) return path;
        
        Vector2 startGrid = toGrid(start);
        Vector2 endGrid = toGrid(end);
        
        // Quick check for very close points
        float distance = start.distance(end);
        if (distance < gridSize * 3) {
            path.push_back(start);
            path.push_back(end);
            return path;
        }
        
        // A* algorithm
        std::priority_queue<std::shared_ptr<PathNode>, 
                           std::vector<std::shared_ptr<PathNode>>,
                           PathNodeCompare> openSet;
        
        std::unordered_map<Vector2, float, Vector2Hash> gScore;
        std::unordered_set<Vector2, Vector2Hash> closedSet;
        
        auto startNode = std::make_shared<PathNode>(startGrid, 0, 
                                                    heuristic(startGrid, endGrid));
        openSet.push(startNode);
        gScore[startGrid] = 0;
        
        int iterations = 0;
        const int maxIterations = 1000;
        
        // Direction vectors for 4-way movement
        std::vector<Vector2> directions = {
            Vector2(1, 0), Vector2(-1, 0), Vector2(0, 1), Vector2(0, -1)
        };
        
        while (!openSet.empty() && iterations < maxIterations) {
            iterations++;
            
            auto current = openSet.top();
            openSet.pop();
            
            // Check if we reached the goal
            if (current->position.distance(endGrid) < 1.5f) {
                // Reconstruct path
                while (current) {
                    path.push_back(toWorld(current->position));
                    current = current->parent;
                }
                std::reverse(path.begin(), path.end());
                return optimizePath(path);
            }
            
            closedSet.insert(current->position);
            
            // Check neighbors
            for (const auto& dir : directions) {
                Vector2 neighbor = current->position + dir;
                
                if (closedSet.count(neighbor)) continue;
                
                if (isBlocked(neighbor, startRoomId, endRoomId)) continue;
                
                float tentativeG = current->g + 1.0f;
                
                // Add turn penalty
                if (current->parent) {
                    Vector2 prevDir = current->position - current->parent->position;
                    if (!(prevDir.x == dir.x && prevDir.y == dir.y)) {
                        tentativeG += turnPenalty;
                    }
                }
                
                if (gScore.count(neighbor) && tentativeG >= gScore[neighbor]) {
                    continue;
                }
                
                gScore[neighbor] = tentativeG;
                float h = heuristic(neighbor, endGrid);
                
                auto newNode = std::make_shared<PathNode>(neighbor, tentativeG, h, current);
                openSet.push(newNode);
            }
        }
        
        // If no path found, return simple L-shaped path
        path.push_back(start);
        path.push_back(Vector2(start.x, end.y));
        path.push_back(end);
        return path;
    }
    
    // Optimize path by removing unnecessary waypoints
    std::vector<Vector2> optimizePath(const std::vector<Vector2>& path) {
        if (path.size() < 3) return path;
        
        std::vector<Vector2> optimized;
        optimized.push_back(path[0]);
        
        for (size_t i = 1; i < path.size() - 1; i++) {
            Vector2 prev = path[i - 1];
            Vector2 curr = path[i];
            Vector2 next = path[i + 1];
            
            // Check if we're changing direction
            Vector2 dir1 = (curr - prev).normalized();
            Vector2 dir2 = (next - curr).normalized();
            
            // Keep point if direction changes
            if (std::abs(dir1.x - dir2.x) > 0.1f || 
                std::abs(dir1.y - dir2.y) > 0.1f) {
                optimized.push_back(curr);
            }
        }
        
        optimized.push_back(path.back());
        return optimized;
    }
    
    // Check if a straight path is possible
    bool hasDirectPath(const Vector2& start, const Vector2& end,
                      int excludeRoom1 = -1, int excludeRoom2 = -1) {
        Vector2 dir = (end - start).normalized();
        float distance = start.distance(end);
        float step = gridSize / 2;
        
        for (float t = 0; t < distance; t += step) {
            Vector2 point = start + dir * t;
            Vector2 gridPoint = toGrid(point);
            
            if (isBlocked(gridPoint, excludeRoom1, excludeRoom2)) {
                return false;
            }
        }
        
        return true;
    }
};
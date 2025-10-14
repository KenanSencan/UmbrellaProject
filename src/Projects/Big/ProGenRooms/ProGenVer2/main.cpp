// main.cpp - SFML visualization of the dungeon generator
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "DungeonGenerator.h"

class DungeonVisualizer {
private:
    sf::RenderWindow window;
    sf::View gameView;
    sf::Font font;

    GenerationSettings settings;
    DungeonGenerator generator;

    // Visual settings
    bool showGrid = false;
    bool showCorridors = true;
    bool showRoomIds = true;
    bool showDebugInfo = true;
    bool isPanning = false;
    
    sf::Vector2f lastMousePos;
    
    // Statistics
    sf::Clock generationClock;
    float lastGenerationTime = 0;
    
public:
    DungeonVisualizer() 
        : window(sf::VideoMode(1920, 1080), "Optimized Dungeon Generator")
    ,gameView(window.getDefaultView())
    ,settings()
    ,generator(settings) {
        
        window.setFramerateLimit(60);
        gameView = window.getDefaultView();
        
        // Load font
        if (!font.loadFromFile("/home/selviniah/Desktop/CMakeSFMLProject/src/Projects/Big/ProGenRooms/ProGenVer2/SegoeUI.ttf")) {
            std::cerr << "Warning: Could not load font, using default\n";
        }
        
        // Initial generation
        regenerate();
    }
    
    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }
    
private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    isPanning = true;
                    lastMousePos = window.mapPixelToCoords(
                        sf::Mouse::getPosition(window), gameView);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    isPanning = false;
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                if (isPanning) {
                    sf::Vector2f currentMousePos = window.mapPixelToCoords(
                        sf::Mouse::getPosition(window), gameView);
                    sf::Vector2f delta = lastMousePos - currentMousePos;
                    gameView.move(delta);
                    lastMousePos = currentMousePos;
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                gameView.zoom(zoomFactor);
            }
        }
    }
    
    void handleKeyPress(sf::Keyboard::Key key) {
        switch (key) {
            case sf::Keyboard::R:
                regenerate();
                break;
            case sf::Keyboard::G:
                showGrid = !showGrid;
                break;
            case sf::Keyboard::C:
                showCorridors = !showCorridors;
                break;
            case sf::Keyboard::I:
                showRoomIds = !showRoomIds;
                break;
            case sf::Keyboard::D:
                showDebugInfo = !showDebugInfo;
                break;
            case sf::Keyboard::Space:
                gameView = window.getDefaultView();
                break;
            case sf::Keyboard::Num1:
                settings.minRooms = std::max(5, settings.minRooms - 5);
                settings.maxRooms = std::max(10, settings.maxRooms - 5);
                break;
            case sf::Keyboard::Num2:
                settings.minRooms = std::min(50, settings.minRooms + 5);
                settings.maxRooms = std::min(100, settings.maxRooms + 5);
                break;
        }
    }
    
    void regenerate() {
        generationClock.restart();
        generator.generate();
        lastGenerationTime = generationClock.getElapsedTime().asMilliseconds();
        
        // Center view on dungeon
        centerView();
    }
    
    void centerView() {
        if (generator.getRooms().empty()) return;
        
        // Calculate bounds
        float minX = 999999, minY = 999999;
        float maxX = -999999, maxY = -999999;
        
        for (const auto& room : generator.getRooms()) {
            Rect bounds = room->getBounds();
            minX = std::min(minX, bounds.x);
            minY = std::min(minY, bounds.y);
            maxX = std::max(maxX, bounds.x + bounds.width);
            maxY = std::max(maxY, bounds.y + bounds.height);
        }
        
        float centerX = (minX + maxX) / 2;
        float centerY = (minY + maxY) / 2;
        gameView.setCenter(centerX, centerY);
    }
    
    void render() {
        window.clear(sf::Color(30, 30, 40));
        
        window.setView(gameView);
        
        // Draw grid
        if (showGrid) {
            drawGrid();
        }
        
        // Draw corridors
        if (showCorridors) {
            drawCorridors();
        }
        
        // Draw rooms
        drawRooms();
        
        // Draw UI
        window.setView(window.getDefaultView());
        drawUI();
        
        window.display();
    }
    
    void drawGrid() {
        auto occupiedCells = generator.getSpatialGrid().getAllOccupiedCells();
        
        for (const auto& cell : occupiedCells) {
            sf::RectangleShape cellShape(sf::Vector2f(100, 100));
            cellShape.setPosition(cell.x * 100, cell.y * 100);
            cellShape.setFillColor(sf::Color(50, 50, 70, 50));
            cellShape.setOutlineColor(sf::Color(70, 70, 100, 100));
            cellShape.setOutlineThickness(1);
            window.draw(cellShape);
        }
    }
    
    void drawCorridors() {
        for (const auto& corridor : generator.getCorridors()) {
            sf::Color corridorColor = corridor->isMainPath ? 
                sf::Color(100, 100, 150, 200) : sf::Color(80, 80, 80, 150);
            
            for (const auto& point : corridor->path) {
                sf::RectangleShape cell(sf::Vector2f(
                    settings.corridorWidth, settings.corridorWidth));
                cell.setPosition(point.x - settings.corridorWidth/2, 
                               point.y - settings.corridorWidth/2);
                cell.setFillColor(corridorColor);
                window.draw(cell);
            }
            
            // Draw corridor as lines
            if (corridor->path.size() > 1) {
                sf::VertexArray line(sf::LineStrip, corridor->path.size());
                for (size_t i = 0; i < corridor->path.size(); i++) {
                    line[i].position = sf::Vector2f(corridor->path[i].x, 
                                                   corridor->path[i].y);
                    line[i].color = sf::Color(255, 255, 255, 50);
                }
                window.draw(line);
            }
        }
    }
    
    void drawRooms() {
        const auto& mainPath = generator.getMainPath();
        
        for (const auto& room : generator.getRooms()) {
            // Determine room color
            sf::Color fillColor, outlineColor;
            
            switch (room->type) {
                case RoomType::Start:
                    fillColor = sf::Color(50, 150, 50, 200);
                    outlineColor = sf::Color::Green;
                    break;
                case RoomType::Boss:
                    fillColor = sf::Color(150, 50, 50, 200);
                    outlineColor = sf::Color::Red;
                    break;
                case RoomType::Treasure:
                    fillColor = sf::Color(150, 150, 50, 200);
                    outlineColor = sf::Color::Yellow;
                    break;
                case RoomType::Shop:
                    fillColor = sf::Color(50, 100, 150, 200);
                    outlineColor = sf::Color::Cyan;
                    break;
                case RoomType::Large:
                    fillColor = sf::Color(100, 50, 150, 200);
                    outlineColor = sf::Color::Magenta;
                    break;
                default:
                    if (room->isMainPath) {
                        fillColor = sf::Color(70, 70, 90, 200);
                        outlineColor = sf::Color::White;
                    } else if (room->hasBeenRelocated) {
                        fillColor = sf::Color(150, 100, 50, 200);
                        outlineColor = sf::Color(255, 200, 100);
                    } else {
                        fillColor = sf::Color(60, 60, 60, 200);
                        outlineColor = sf::Color(150, 150, 150);
                    }
                    break;
            }
            
            // Draw room
            sf::RectangleShape roomShape(sf::Vector2f(room->size.x, room->size.y));
            roomShape.setPosition(room->position.x - room->size.x/2,
                                 room->position.y - room->size.y/2);
            roomShape.setFillColor(fillColor);
            roomShape.setOutlineColor(outlineColor);
            roomShape.setOutlineThickness(3);
            window.draw(roomShape);
            
            // Draw doors
            for (const auto& door : room->doors) {
                Vector2 doorPos = room->getDoorPosition(door->direction);
                sf::CircleShape doorShape(10);
                doorShape.setPosition(doorPos.x - 10, doorPos.y - 10);
                
                sf::Color doorColor;
                switch (door->direction) {
                    case DoorDirection::Right: doorColor = sf::Color(255, 100, 100); break;
                    case DoorDirection::Left: doorColor = sf::Color(100, 100, 255); break;
                    case DoorDirection::Up: doorColor = sf::Color(100, 255, 100); break;
                    case DoorDirection::Down: doorColor = sf::Color(255, 255, 100); break;
                    default: doorColor = sf::Color::White; break;
                }
                
                doorShape.setFillColor(doorColor);
                doorShape.setOutlineColor(door->isConnected ? 
                    sf::Color::White : sf::Color(100, 100, 100));
                doorShape.setOutlineThickness(2);
                window.draw(doorShape);
            }
            
            // Draw room ID
            if (showRoomIds) {
                sf::Text idText;
                idText.setFont(font);
                idText.setString(std::to_string(room->id));
                idText.setCharacterSize(14);
                idText.setFillColor(sf::Color::White);
                
                // Center text
                sf::FloatRect textBounds = idText.getLocalBounds();
                idText.setOrigin(textBounds.width / 2, textBounds.height / 2);
                idText.setPosition(room->position.x, room->position.y);
                
                window.draw(idText);
            }
        }
    }
    
    void drawUI() {
        if (!showDebugInfo) return;
        
        // Background panel
        sf::RectangleShape panel(sf::Vector2f(400, 250));
        panel.setPosition(10, 10);
        panel.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(panel);
        
        // Debug text
        std::stringstream ss;
        ss << "=== OPTIMIZED DUNGEON GENERATOR ===\n";
        ss << "Generation Time: " << std::fixed << std::setprecision(1) 
           << lastGenerationTime << " ms\n";
        ss << "Rooms: " << generator.getRooms().size() << " / " 
           << settings.maxRooms << "\n";
        ss << "Corridors: " << generator.getCorridors().size() << "\n";
        ss << "Main Path Length: " << generator.getMainPath().size() << "\n";
        
        int relocatedCount = 0;
        for (const auto& room : generator.getRooms()) {
            if (room->hasBeenRelocated) relocatedCount++;
        }
        ss << "Relocated Rooms: " << relocatedCount << "\n\n";
        
        ss << "=== CONTROLS ===\n";
        ss << "[R] Regenerate\n";
        ss << "[G] Toggle Grid\n";
        ss << "[C] Toggle Corridors\n";
        ss << "[I] Toggle Room IDs\n";
        ss << "[D] Toggle Debug Info\n";
        ss << "[Space] Reset View\n";
        ss << "[1/2] Decrease/Increase Room Count\n";
        ss << "[Middle Mouse] Pan | [Scroll] Zoom\n\n";
        
        ss << "=== ROOM COLORS ===\n";
        ss << "Green: Start | Red: Boss\n";
        ss << "Yellow: Treasure | Cyan: Shop\n";
        ss << "Orange: Relocated | White: Main Path";
        
        sf::Text debugText;
        debugText.setFont(font);
        debugText.setString(ss.str());
        debugText.setCharacterSize(12);
        debugText.setFillColor(sf::Color::White);
        debugText.setPosition(15, 15);
        
        window.draw(debugText);
    }
};

int main() {
    try {
        DungeonVisualizer visualizer;
        visualizer.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>

// Function for the first window
void runWindow1() {
    sf::RenderWindow window1(sf::VideoMode(800, 600), "Window 1");
    window1.setFramerateLimit(60);
    
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(300.f, 200.f);
    
    while (window1.isOpen()) {
        sf::Event event;
        while (window1.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window1.close();
        }
        
        window1.clear();
        window1.draw(shape);
        window1.display();
    }
    
    std::cout << "Window 1 closed" << std::endl;
}

// Function for the second window
void runWindow2() {
    sf::RenderWindow window2(sf::VideoMode(600, 400), "Window 2");
    window2.setFramerateLimit(60);
    
    sf::RectangleShape shape(sf::Vector2f(120.f, 120.f));
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(240.f, 140.f);
    
    while (window2.isOpen()) {
        sf::Event event;
        while (window2.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window2.close();
        }
        
        window2.clear(sf::Color(50, 50, 50));
        window2.draw(shape);
        window2.display();
    }
    
    std::cout << "Window 2 closed" << std::endl;
}

int main() {
    // Create two threads and detach them
    std::thread t1(runWindow1);
    std::thread t2(runWindow2);
    
    // Detach the threads so they run independently
    t1.detach();
    t2.detach();
    
    // Keep the main thread alive
    std::cout << "Press Enter to exit the program..." << std::endl;
    std::cin.get();
    
    return 0;
}
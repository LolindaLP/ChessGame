#include "Board.h"

int main() {
    Board board;
    board.run();
    return 0;
}

//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Drag and Drop Example");
//    window.setFramerateLimit(30);
//
//    // Create a simple square shape
//    sf::Texture texture;
//    if (!texture.loadFromFile("E:\\SFML\\SFMLtest\\Sprites\\black-king.png")) {
//        std::cerr << "Error loading texture from file: white-king.png" << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    // Create the sprite
//    sf::Sprite sprite;
//    sprite.setTexture(texture);
//    sprite.setPosition(sf::Vector2f(10.f, 50.f)); // Set the sprite position
//
//    bool isDragging = false; // Flag to track dragging
//    sf::Vector2f offset;     // Offset between mouse and square position
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            // Check for mouse button press
//            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
//                if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
//                    isDragging = true;
//                    offset = sprite.getPosition() - window.mapPixelToCoords(sf::Mouse::getPosition(window));
//                }
//            }
//
//            // Check for mouse button release
//            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
//                isDragging = false;
//            }
//        }
//
//        // Update square position while dragging
//        if (isDragging) {
//            sprite.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) + offset);
//        }
//
//        window.clear(sf::Color::White);
//        window.draw(sprite);
//        window.display();
//    }
//
//    return 0;
//}
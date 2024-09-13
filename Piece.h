#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>

// Enumeration for piece colors
enum class Color { WHITE, BLACK };

// Base class for all pieces
class Piece {
protected:
    Color color;                 // Color of the piece (white or black)
    sf::Vector2i position;       // Position of the piece on the board (row, column)
    sf::Sprite sprite;           // SFML sprite for drawing the piece
    sf::Texture texture;         // Texture for the piece image
    const float tileSize = 100.f; // Size of each square on the board

public:
    // Constructor initializes the piece with color and position
    Piece(Color color, sf::Vector2i position)
        : color(color), position(position) {}

    // Virtual destructor
    virtual ~Piece() {}

    // Method to draw the piece on the window
    virtual void draw(sf::RenderWindow& window) {
        if (!texture.getSize().x || !texture.getSize().y) {
            return; // If texture is not loaded, don't attempt to draw
        }

        // Scale sprite to fit within a tile
        float scaleX = tileSize / static_cast<float>(texture.getSize().x);
        float scaleY = tileSize / static_cast<float>(texture.getSize().y);
        sprite.setScale(scaleX, scaleY);

        // Center the sprite within the tile
        sf::Vector2f offset(
            (tileSize - texture.getSize().x * scaleX) / 2.f,
            (tileSize - texture.getSize().y * scaleY) / 2.f
        );
        sprite.setPosition(position.x * tileSize + offset.x, position.y * tileSize + offset.y);
        window.draw(sprite);
    }

    // Getters and setters
    sf::Vector2i getPosition() const { return position; }
    void setPosition(sf::Vector2i newPosition) { position = newPosition; }

    bool loadTexture(const std::string& filename) {
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Error loading texture: " << filename << std::endl;
            return false;
        }
        sprite.setTexture(texture);
        return true;
    }

    // For use in drag-and-drop, allowing access to the sprite directly
    sf::Sprite& getSprite() { return sprite; }
    Color getColor() const { return color; }

    // Method to check if a point (mouse click) is within the piece's bounds
    bool contains(sf::Vector2f point) const {
        return sprite.getGlobalBounds().contains(point);
    }
};

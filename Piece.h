#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "globals.h"

// Enumeration for piece colors
enum class Color { WHITE, BLACK };

// Base class for all pieces
class Piece {
protected:
    Color color;                 // Color of the piece (white or black)
    sf::Vector2i position;       // Position of the piece on the board (row, column)
    sf::Texture texture;         // Texture for the piece image
    const float tileSize = 100.f;

public:
    sf::Sprite sprite;           // SFML sprite for drawing the piece
    
    // Constructor initializes the piece with color and position
    Piece(Color color, sf::Vector2i position);

    // Virtual destructor
    virtual ~Piece();

    // Virtual method to draw the piece on the window
    virtual void draw(sf::RenderWindow& window);

    void snapToGrid();

    sf::Vector2i getPosition() const;

    Color getColor() const;

    void setPosition(sf::Vector2i newPosition);

    bool loadTexture(const std::string& filename);

    // Pure virtual method to validate moves, to be implemented in derived classes
    virtual bool isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const = 0;

};

#include "Rook.h"

// Rook constructor
Rook::Rook(Color color, sf::Vector2i position)
    : Piece(color, position) {
    if (color == Color::WHITE) {
        loadTexture("Sprites/white-rook.png");
    }
    else {
        loadTexture("Sprites/black-rook.png");
    }
}

bool Rook::isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const {
    // Calculate the difference in x and y positions
    int dx = std::abs(newPosition.x - position.x);
    int dy = std::abs(newPosition.y - position.y);

    // Rook moves any number of squares horizontally or vertically
    return (dx == 0 || dy == 0);
}
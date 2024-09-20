#include "Queen.h"

// Queen constructor
Queen::Queen(Color color, sf::Vector2i position)
    : Piece(color, position) {
    if (color == Color::WHITE) {
        loadTexture("Sprites/white-queen.png");
    }
    else {
        loadTexture("Sprites/black-queen.png");
    }
}

bool Queen::isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const {
    // Calculate the difference in x and y positions
    int dx = std::abs(newPosition.x - position.x);
    int dy = std::abs(newPosition.y - position.y);

    // Queen moves any number of squares horizontally, vertically, or diagonally
    return (dx == 0 || dy == 0 || dx == dy);
}
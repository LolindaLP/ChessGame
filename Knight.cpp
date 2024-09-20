#include "Knight.h"

// Knight constructor
Knight::Knight(Color color, sf::Vector2i position)
    : Piece(color, position) {
    if (color == Color::WHITE) {
        loadTexture("Sprites/white-knight.png");
    }
    else {
        loadTexture("Sprites/black-knight.png");
    }
}

bool Knight::isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const {
    // Calculate the difference in x and y positions
    int dx = std::abs(newPosition.x - position.x);
    int dy = std::abs(newPosition.y - position.y);

    // Knight moves in an L-shape: two squares in one direction and one square perpendicular
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}


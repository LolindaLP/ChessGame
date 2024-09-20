#include "King.h"

// King constructor
King::King(Color color, sf::Vector2i position)
    : Piece(color, position) {
    if (color == Color::WHITE) {
        loadTexture("Sprites/white-king.png");
    }
    else {
        loadTexture("Sprites/black-king.png");
    }

}

bool King::isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const {
    // Calculate the difference in x and y positions
    int dx = std::abs(newPosition.x - position.x);
    int dy = std::abs(newPosition.y - position.y);

    // King moves one square in any direction
    if (dx <= 1 && dy <= 1) {
        return true;
    }

    bool isWhiteKing = (getColor() == Color::WHITE);
    bool isBlackKing = (getColor() == Color::BLACK);
    if (isWhiteKing && newPosition == sf::Vector2i(6, 7)) { // White King castling
        return true;
    } else if (isBlackKing && newPosition == sf::Vector2i(6, 0)) { // Black King castling
        return true;
    } else if (isBlackKing && newPosition == sf::Vector2i(2, 0)) { // Black King castling
        return true;
    } else if (isWhiteKing && newPosition == sf::Vector2i(2, 7)) {
        return true;
    }
    return false; // Invalid move if no conditions are met
}
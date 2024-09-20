#include "Bishop.h"

// Bishop constructor
Bishop::Bishop(Color color, sf::Vector2i position)
    : Piece(color, position) {
    if (color == Color::WHITE) {
        loadTexture("Sprites/white-bishop.png");
    }
    else {
        loadTexture("Sprites/black-bishop.png");
    }
}

bool Bishop::isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const {
    // Calculate the difference in x and y positions
    int dx = std::abs(newPosition.x - position.x);
    int dy = std::abs(newPosition.y - position.y);

    // Bishop moves diagonally, so dx must be equal to dy
    return dx == dy;
}


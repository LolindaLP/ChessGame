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

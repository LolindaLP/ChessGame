#pragma once
#include "Piece.h"

// Bishop piece class
class Bishop : public Piece {
public:
    Bishop(Color color, sf::Vector2i position);

    bool isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const override;
};

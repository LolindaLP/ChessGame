#pragma once
#include "Piece.h"

// Rook piece class
class Rook : public Piece {
public:
    Rook(Color color, sf::Vector2i position);

    bool isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const override;
};


#pragma once
#include "Piece.h"

// Knight piece class
class Knight : public Piece {
public:
    Knight(Color color, sf::Vector2i position);

    bool isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const override;
};
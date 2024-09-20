#pragma once
#include "Piece.h"


// Pawn piece class
class Pawn : public Piece {
public:
    Pawn(Color color, sf::Vector2i position);

    bool isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const override;
};

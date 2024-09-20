#pragma once
#include "Piece.h"

// Queen piece class
class Queen : public Piece {
public:
    Queen(Color color, sf::Vector2i position);

    bool isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const override;;
};




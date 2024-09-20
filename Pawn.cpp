#include "Pawn.h"

// Pawn constructor
Pawn::Pawn(Color color, sf::Vector2i position)
    : Piece(color, position) {
    if (color == Color::WHITE) {
        loadTexture("Sprites/white-pawn.png");
    }
    else {
        loadTexture("Sprites/black-pawn.png");
    }
}

bool Pawn::isValidMove(sf::Vector2i newPosition, sf::Vector2i position, Piece* targetPiece) const {
    // Calculate the difference in x and y positions
    int dx = newPosition.x - position.x;
    int dy = newPosition.y - position.y;

    // Determine if the pawn is white or black based on color
    bool isWhite = (this->getColor() == Color::WHITE);

    // White pawns move up (decreasing y) and black pawns move down (increasing y)
    int direction = isWhite ? -1 : 1;
    int startRow = isWhite ? 6 : 1;

    // Check if the pawn is moving forward by 2 squares from its initial position
    if (dx == 0 && dy == 2 * direction && position.y == startRow && targetPiece == nullptr) {
        // Активируем возможность взятия на проходе
        enPassantPossibility = true;
        std::cout << "true\n";
        return true;
    }

    // Check if the pawn is moving forward by 1 square
    if (dx == 0 && dy == direction && targetPiece == nullptr) {
        enPassantPossibility = false;
        std::cout << "false\n";
        return true;
    }

    // Check if the pawn is capturing diagonally
    if (std::abs(dx) == 1 && dy == direction) {
        // If there's a target piece, make sure it's of the opposite color
        if (targetPiece != nullptr && targetPiece->getColor() != this->getColor()) {
            // Обычный захват по диагонали
            enPassantPossibility = false;
            std::cout << "false\n";
            return true;
        }

        // Взятие на проходе
        if (enPassantPossibility) {
            return true;  // Возвращаем true для взятия на проходе
        }
    }

    return false;
}


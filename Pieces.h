#pragma once
#include "Piece.h"

// King piece class
class King : public Piece {
public:
    King(Color color, sf::Vector2i position)
        : Piece(color, position)
    {
        // Load texture based on the color of the king
        if (color == Color::WHITE) {
            loadTexture("Sprites/white-king.png");
        }
        else {
            loadTexture("Sprites/black-king.png");
        }
    }
};

//// Queen piece class
//class Queen : public Piece {
//public:
//    Queen(Color color, sf::Vector2i position, float tileSize)
//        : Piece(color, position, tileSize) {
//        // Load texture based on the color
//        if (color == Color::WHITE) {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\white-queen.png");
//        }
//        else {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\black-queen.png");
//        }
//    }
//};
//
//// Rook piece class
//class Rook : public Piece {
//public:
//    Rook(Color color, sf::Vector2i position, float tileSize)
//        : Piece(color, position, tileSize) {
//        // Load texture based on the color
//        if (color == Color::WHITE) {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\white-rook.png");
//        }
//        else {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\black-rook.png");
//        }
//    }
//};
//
//// Bishop piece class
//class Bishop : public Piece {
//public:
//    Bishop(Color color, sf::Vector2i position, float tileSize)
//        : Piece(color, position, tileSize) {
//        // Load texture based on the color
//        if (color == Color::WHITE) {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\white-bishop.png");
//        }
//        else {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\black-bishop.png");
//        }
//    }
//};
//
//// Knight piece class
//class Knight : public Piece {
//public:
//    Knight(Color color, sf::Vector2i position, float tileSize)
//        : Piece(color, position, tileSize) {
//        // Load texture based on the color
//        if (color == Color::WHITE) {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\white-knight.png");
//        }
//        else {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\black-knight.png");
//        }
//    }
//};
//
//// Pawn piece class
//class Pawn : public Piece {
//public:
//    Pawn(Color color, sf::Vector2i position, float tileSize)
//        : Piece(color, position, tileSize) {
//        // Load texture based on the color
//        if (color == Color::WHITE) {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\white-pawn.png");
//        }
//        else {
//            loadTexture("E:\\SFML\\SFMLtest\\Sprites\\black-pawn.png");
//        }
//    }
//};
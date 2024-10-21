#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "globals.h"

// Board class to handle rendering and interaction
class Board {
private:
    // TODO: Maybe _ as suffix of member variables
    sf::RenderWindow window;                  // Window for displaying the chessboard
    sf::RectangleShape squares[8][8];         // Array to store the board's squares
    const float tileSize = 100.f;             // Size of each square in pixels
    std::vector<std::unique_ptr<Piece>> pieces; // Vector to store all pieces on the board
    // TODO: Docs
    sf::Vector2f offset;
    // TODO: To english
    sf::Vector2i selectedPiecePosition;             
    Piece* draggedPiece = nullptr;       

public:
    bool boardRendered = false; // Flag to check if the board is already rendered
    Color currentTurn;// Constructor initializes the window and the board squares

    Board();

    // Method to initialize the board (create squares and set their colors)
    void initializeBoard();

    // Add initial pieces to the board
    void initializePieces();

    // Main loop to handle events and rendering
    void run();

    void selectPiece(const sf::Vector2i& mousePos);
    
    void placePiece(const sf::Vector2i& mousePos);

    bool isValidMove(Piece* piece, const sf::Vector2i& newPosition);

    bool isInCheck(Color color);

    bool isCheckmate(Color color);

    void handleCastling(King* king, const sf::Vector2i& newPosition);

    void moveRookForCastling(const sf::Vector2i& rookPos, const sf::Vector2i& newPos);

    void handlePawnPromotion(Pawn* pawn, const sf::Vector2i& newPosition);

    void handleCapture(Piece* draggedPiece, const sf::Vector2i& newPosition);

    void handleEnPassant(Pawn* pawn, const sf::Vector2i& newPosition);

    bool isPathClear(sf::Vector2i newPosition, sf::Vector2i position);

    void removePiece(Piece* pieceToRemove);

    Piece* getPieceAt(const sf::Vector2i& pos) const;

    // Helper function to render the board and pieces
    void renderBoard();

    void switchTurn();
};
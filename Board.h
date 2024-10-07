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
    sf::RenderWindow window;                  // Window for displaying the chessboard
    sf::RectangleShape squares[8][8];         // Array to store the board's squares
    const float tileSize = 100.f;             // Size of each square in pixels
    std::vector<std::unique_ptr<Piece>> pieces; // Vector to store all pieces on the board
    sf::Vector2f offset;  
    sf::Vector2i selectedPiecePosition;  // Логическая позиция выбранной фигуры
    bool isDragging = false;             // Флаг, указывает, перетаскиваем ли фигуру
    Piece* draggedPiece = nullptr;       // Указатель на перетаскиваемую фигуру

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

    bool isPathClear(sf::Vector2i newPosition, sf::Vector2i position);

    void removePiece(Piece* pieceToRemove);

    Piece* getPieceAt(const sf::Vector2i& pos) const;

    // Helper function to render the board and pieces
    void renderBoard();

    void switchTurn();

    void checkForCheck(Color currentTurnColor);
};
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Piece.h"
#include "King.h"

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
    // Constructor initializes the window and the board squares
    Board();

    // Method to initialize the board (create squares and set their colors)
    void initializeBoard();

    // Add initial pieces to the board
    void initializePieces();

    // Main loop to handle events and rendering
    void run();

    void selectPiece(const sf::Vector2i& mousePos);
    
    void placePiece(const sf::Vector2i& mousePos);

    // Helper function to render the board and pieces
    void renderBoard();

};

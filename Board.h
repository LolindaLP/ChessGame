#include "Pieces.h"
#include <iostream>
#include <algorithm>
#include <memory>

class Board {
private:
    sf::RenderWindow window;                  // Window for displaying the chessboard
    sf::RectangleShape squares[8][8];         // Array to store the board's squares
    const float tileSize = 100.f;             // Size of each square in pixels
    std::vector<std::unique_ptr<Piece>> pieces; // Vector to store all pieces on the board
    Piece* selectedPiece = nullptr;           // Pointer to the currently selected piece
    bool isDragging = false;                  // Flag to track if dragging is in progress
    sf::Vector2i offset;                      // Offset between mouse and piece position

public:
    // Constructor initializes the window and the board squares
    Board() : window(sf::VideoMode(800, 800), "Chess Board") {
        initializeBoard();
        initializePieces();
    }

    // Method to initialize the board (create squares and set their colors)
    void initializeBoard() {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                // Set the size of the square
                squares[row][col].setSize(sf::Vector2f(tileSize, tileSize));

                // Set the position of the square
                squares[row][col].setPosition(col * tileSize, row * tileSize);

                // Set the color of the square (alternating colors)
                if ((row + col) % 2 == 0) {
                    squares[row][col].setFillColor(sf::Color(222, 184, 135)); // Light color
                }
                else {
                    squares[row][col].setFillColor(sf::Color(139, 69, 19));   // Dark color
                }
            }
        }
    }

    // Add initial pieces to the board
    void initializePieces() {
        // Example pieces, adjust positions and add more pieces as needed
        pieces.push_back(std::make_unique<King>(Color::WHITE, sf::Vector2i(4, 0)));
        pieces.push_back(std::make_unique<King>(Color::BLACK, sf::Vector2i(4, 7)));
    }

    // Main loop to handle events and rendering
    void draw() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.waitEvent(event)) {

            }

            renderBoard(); // Draw the board and pieces
        }
    }


    // Helper function to render the board and pieces
    void renderBoard() {
        window.clear();

        // Draw the board squares
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                window.draw(squares[row][col]);
            }
        }

        // Draw all pieces
        for (const auto& piece : pieces) {
            piece->draw(window);
        }

        window.display();
    }
};

#include "Board.h"

// Constructor
Board::Board() : window(sf::VideoMode(800, 800), "Chess Board") {
    window.setFramerateLimit(60);
    initializeBoard();
    initializePieces();
}

// Initialize the board
void Board::initializeBoard() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            squares[row][col].setSize(sf::Vector2f(tileSize, tileSize));
            squares[row][col].setPosition(col * tileSize, row * tileSize);
            if ((row + col) % 2 == 0) {
                squares[row][col].setFillColor(sf::Color(222, 184, 135)); // Light color
            }
            else {
                squares[row][col].setFillColor(sf::Color(139, 69, 19));   // Dark color
            }
        }
    }
}

// Initialize the pieces
void Board::initializePieces() {
    auto king = std::make_unique<King>(Color::WHITE, sf::Vector2i(4, 0));
    king->snapToGrid();
    pieces.push_back(std::move(king));
}

void Board::run() {

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                selectPiece(mousePos);
                isDragging = true;
            }

            if (isDragging) {
                sf::Vector2f mousePosFloat = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Обновляем позицию фигуры с учетом смещения
                if (draggedPiece) {
                    draggedPiece->sprite.setPosition(mousePosFloat - offset);
                }
                }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (isDragging) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    placePiece(mousePos);
                    isDragging = false;
                }
            }

        }

        renderBoard();
    }
}

void Board::selectPiece(const sf::Vector2i& mousePos) {
    // getting row and col of clicked area
    int row = mousePos.y / tileSize;
    int col = mousePos.x / tileSize;

    // Check if there is a piece
    for (auto& piece : pieces) {
        if (piece->getPosition() == sf::Vector2i(col, row)) {
            draggedPiece = piece.get();
            selectedPiecePosition = piece->getPosition();
            sf::FloatRect bounds = draggedPiece->sprite.getGlobalBounds();
            offset = sf::Vector2f(mousePos.x - bounds.left, mousePos.y - bounds.top);
            break;
        }
    }
}

void Board::placePiece(const sf::Vector2i& mousePos) {
    if (draggedPiece) {
        int row = mousePos.y / tileSize;
        int col = mousePos.x / tileSize;

        draggedPiece->setPosition(sf::Vector2i(col, row));
        draggedPiece->snapToGrid();
        draggedPiece = nullptr;
    }
}


// Render the board and pieces
void Board::renderBoard() {
    window.clear();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            window.draw(squares[row][col]);
        }
    }

    for (const auto& piece : pieces) {
        piece->draw(window);
    }

    window.display();
}

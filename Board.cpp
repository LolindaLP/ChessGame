#include "Board.h"

// Constructor
Board::Board() : window(sf::VideoMode(800, 800), "Chess Board"), currentTurn(Color::WHITE) {
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
    // White pieces
    pieces.push_back(std::make_unique<King>(Color::WHITE, sf::Vector2i(4, 7)));
    pieces.push_back(std::make_unique<Queen>(Color::WHITE, sf::Vector2i(3, 7)));
    pieces.push_back(std::make_unique<Rook>(Color::WHITE, sf::Vector2i(0, 7)));
    pieces.push_back(std::make_unique<Rook>(Color::WHITE, sf::Vector2i(7, 7)));
    pieces.push_back(std::make_unique<Knight>(Color::WHITE, sf::Vector2i(1, 7)));
    pieces.push_back(std::make_unique<Knight>(Color::WHITE, sf::Vector2i(6, 7)));
    pieces.push_back(std::make_unique<Bishop>(Color::WHITE, sf::Vector2i(2, 7)));
    pieces.push_back(std::make_unique<Bishop>(Color::WHITE, sf::Vector2i(5, 7)));

    // White pawns
    for (int i = 0; i < 8; ++i) {
        pieces.push_back(std::make_unique<Pawn>(Color::WHITE, sf::Vector2i(i, 6)));
    }

    // Black pieces
    pieces.push_back(std::make_unique<King>(Color::BLACK, sf::Vector2i(4, 0)));
    pieces.push_back(std::make_unique<Queen>(Color::BLACK, sf::Vector2i(3, 0)));
    pieces.push_back(std::make_unique<Rook>(Color::BLACK, sf::Vector2i(0, 0)));
    pieces.push_back(std::make_unique<Rook>(Color::BLACK, sf::Vector2i(7, 0)));
    pieces.push_back(std::make_unique<Knight>(Color::BLACK, sf::Vector2i(1, 0)));
    pieces.push_back(std::make_unique<Knight>(Color::BLACK, sf::Vector2i(6, 0)));
    pieces.push_back(std::make_unique<Bishop>(Color::BLACK, sf::Vector2i(2, 0)));
    pieces.push_back(std::make_unique<Bishop>(Color::BLACK, sf::Vector2i(5, 0)));

    // Black pawns
    for (int i = 0; i < 8; ++i) {
        pieces.push_back(std::make_unique<Pawn>(Color::BLACK, sf::Vector2i(i, 1)));
    }

    // Snap all pieces to grid
    for (auto& piece : pieces) {
        piece->snapToGrid();
    }
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
        if (piece->getPosition() == sf::Vector2i(col, row) && piece->getColor() == currentTurn) {
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

        sf::Vector2i newPosition(col, row);

        // Check if there is a piece at the target position
        Piece* targetPiece = getPieceAt(newPosition);

        // Check if the move is valid for the dragged piece
        if (draggedPiece->isValidMove(newPosition, draggedPiece->getPosition())) {
            // For non-knight pieces, check if the path is clear
            if (!dynamic_cast<Knight*>(draggedPiece) && !isPathClear(newPosition, draggedPiece->getPosition())) {
                // If the path is blocked, handle it
                draggedPiece->snapToGrid();
                draggedPiece = nullptr;
                return;
            }

            // If a piece of the same color is on the target cell, block the move
            if (targetPiece != nullptr && targetPiece->getColor() == draggedPiece->getColor()) {
                draggedPiece->snapToGrid();  // Return dragged piece to its original position
                draggedPiece = nullptr;
                return;
            }

            draggedPiece->setPosition(newPosition);

            if (targetPiece != nullptr && targetPiece->getColor() != draggedPiece->getColor()) {
                removePiece(targetPiece); // Удаление фигуры противника
            }
            draggedPiece->snapToGrid();
            draggedPiece = nullptr;
            switchTurn(); // Switch turn only if the move is valid
        }
        else {
            // If the move is invalid, you can handle it by resetting the piece to its original position
            draggedPiece->snapToGrid();
            draggedPiece = nullptr; // Reset the dragged piece
        }
    }
}

bool Board::isPathClear(sf::Vector2i newPosition, sf::Vector2i position) {
    int dx = newPosition.x - position.x;
    int dy = newPosition.y - position.y;

    // Определение направления движения
    int stepX = (dx == 0) ? 0 : (dx > 0) ? 1 : -1;
    int stepY = (dy == 0) ? 0 : (dy > 0) ? 1 : -1;

    sf::Vector2i currentPos = position;

    // Проверяем клетки на пути до newPosition
    while (currentPos != newPosition) {
        currentPos.x += stepX;
        currentPos.y += stepY;

        if (currentPos == newPosition) {
            break; // Остановка на целевой клетке, так как там может быть фигура
        }

        if (getPieceAt(currentPos) != nullptr) {
            return false; // Путь заблокирован
        }
    }

    // Путь свободен
    return true;
}


void Board::removePiece(Piece* pieceToRemove) {
    // Ищем фигуру в массиве pieces по её позиции
    auto it = std::find_if(pieces.begin(), pieces.end(),
        [pieceToRemove](const std::unique_ptr<Piece>& piece) {
            return piece.get() == pieceToRemove;
        });
    // Если фигура найдена, удаляем её
    if (it != pieces.end()) {
        pieces.erase(it); // Удаление из вектора автоматически освободит память через unique_ptr
    }
}


Piece* Board::getPieceAt(const sf::Vector2i& pos) const {
    // This function should return the piece at the given position, or nullptr if no piece is present.
    for (const auto& piece : pieces) {
        if (piece->getPosition() == pos) {
            return piece.get(); // Assuming `pieces` is a vector of unique_ptr or shared_ptr to Piece
        }
    }
    return nullptr; // No piece found at the position
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


void Board::switchTurn() {
    currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
}
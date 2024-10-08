#include "Board.h"


// Constructor
Board::Board() : window(sf::VideoMode(800, 800), "Chess Board", sf::Style::Resize | sf::Style::Close), currentTurn(Color::WHITE) {
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
    renderBoard();
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

// Main loop
void Board::run() {

    //renderBoard();

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
            // Handle window resize event
            if (event.type == sf::Event::Resized)
            {
                // Keep a 1:1 aspect ratio by choosing the smaller dimension
                unsigned int newSize = std::min(event.size.width, event.size.height);

                window.setSize(sf::Vector2u(newSize, newSize));
            }
        }
        renderBoard();
    }
}


void Board::selectPiece(const sf::Vector2i& mousePos) {
    // Convert mouse position from pixel coordinates to world coordinates
    sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

    // Get the row and column of the clicked tile based on the world coordinates
    int row = static_cast<int>(worldMousePos.y) / tileSize;
    int col = static_cast<int>(worldMousePos.x) / tileSize;

    // Check if there is a piece at the position and if it's the correct color
    Piece* piece = getPieceAt(sf::Vector2i(col, row));
    if (piece != nullptr && piece->getColor() == currentTurn) {
        draggedPiece = piece;
        selectedPiecePosition = piece->getPosition();

        // Get the resized bounds of the piece in world coordinates
        sf::FloatRect bounds = draggedPiece->sprite.getGlobalBounds();

        // Calculate the offset using world coordinates
        offset = sf::Vector2f(worldMousePos.x - bounds.left, worldMousePos.y - bounds.top);
    }
}


void Board::placePiece(const sf::Vector2i& mousePos) {
    if (draggedPiece) {
        // Convert mouse position from pixel coordinates to world coordinates
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        // Get the row and column of the target tile based on the world coordinates
        int row = static_cast<int>(worldMousePos.y) / tileSize;
        int col = static_cast<int>(worldMousePos.x) / tileSize;

        sf::Vector2i newPosition(col, row);

        // Check if there is a piece at the target position
        Piece* targetPiece = getPieceAt(newPosition);

        // Check if the move is valid for the dragged piece
        if (draggedPiece->isValidMove(newPosition, draggedPiece->getPosition(), targetPiece)) {

            // For non-knight pieces, check if the path is clear
            if (!dynamic_cast<Knight*>(draggedPiece) && !isPathClear(newPosition, draggedPiece->getPosition())) {
                // If the path is blocked, reset the piece
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


            // Castling logic for the King
            if (auto king = dynamic_cast<King*>(draggedPiece)) {
                bool isWhiteKing = (king->getColor() == Color::WHITE);
                bool isBlackKing = (king->getColor() == Color::BLACK);

                // White king-side castling
                if (isWhiteKing && isPathClear(sf::Vector2i(4, 7), sf::Vector2i(7, 7)) && newPosition == sf::Vector2i(6, 7)) {
                    Piece* rook = getPieceAt(sf::Vector2i(7, 7)); // Rook on h1
                    if (rook != nullptr && dynamic_cast<Rook*>(rook) && rook->getColor() == Color::WHITE) {
                        rook->setPosition(sf::Vector2i(5, 7)); // Move rook to f1
                        rook->snapToGrid();
                        king->setPosition(sf::Vector2i(6, 7)); // Move king to g1
                        king->snapToGrid();
                    }
                }

                // Black king-side castling
                if (isBlackKing && isPathClear(sf::Vector2i(4, 0), sf::Vector2i(7, 0)) && newPosition == sf::Vector2i(6, 0)) {
                    Piece* rook = getPieceAt(sf::Vector2i(7, 0)); // Rook on h8
                    if (rook != nullptr && dynamic_cast<Rook*>(rook) && rook->getColor() == Color::BLACK) {
                        rook->setPosition(sf::Vector2i(5, 0)); // Move rook to f8
                        rook->snapToGrid();
                        king->setPosition(sf::Vector2i(6, 0)); // Move king to g8
                        king->snapToGrid();
                    }
                }

                // White queen-side castling
                if (isWhiteKing && isPathClear(sf::Vector2i(4, 7), sf::Vector2i(0, 7)) && newPosition == sf::Vector2i(2, 7)) {
                    Piece* rook = getPieceAt(sf::Vector2i(0, 7)); // Rook on a1
                    if (rook != nullptr && dynamic_cast<Rook*>(rook) && rook->getColor() == Color::WHITE) {
                        rook->setPosition(sf::Vector2i(3, 7)); // Move rook to d1
                        rook->snapToGrid();
                        king->setPosition(sf::Vector2i(2, 7)); // Move king to c1
                        king->snapToGrid();
                    }
                }

                // Black queen-side castling
                if (isBlackKing && isPathClear(sf::Vector2i(4, 0), sf::Vector2i(0, 0)) && newPosition == sf::Vector2i(2, 0)) {
                    Piece* rook = getPieceAt(sf::Vector2i(0, 0)); // Rook on a8
                    if (rook != nullptr && dynamic_cast<Rook*>(rook) && rook->getColor() == Color::BLACK) {
                        rook->setPosition(sf::Vector2i(3, 0)); // Move rook to d8
                        rook->snapToGrid();
                        king->setPosition(sf::Vector2i(2, 0)); // Move king to c8
                        king->snapToGrid();
                    }
                }
            }

            // Pawn promotion and en passant logic
            if (auto pawn = dynamic_cast<Pawn*>(draggedPiece)) {
                bool isWhitePawn = (pawn->getColor() == Color::WHITE);
                bool isBlackPawn = (pawn->getColor() == Color::BLACK);

                // Pawn promotion
                if (isWhitePawn && row == 0) {
                    removePiece(pawn);
                    auto newQueen = std::make_unique<Queen>(Color::WHITE, newPosition);
                    newQueen->snapToGrid();
                    pieces.push_back(std::move(newQueen));
                }

                if (isBlackPawn && row == 7) {
                    removePiece(pawn);
                    auto newQueen = std::make_unique<Queen>(Color::BLACK, newPosition);
                    newQueen->snapToGrid();
                    pieces.push_back(std::move(newQueen));
                }

                // En passant capture logic
                if (enPassantPossibility) {
                    if (isWhitePawn) {
                        Piece* capturedPawn = getPieceAt(sf::Vector2i(newPosition.x, newPosition.y + 1));
                        if (capturedPawn && capturedPawn->getColor() == Color::BLACK) {
                            removePiece(capturedPawn);
                            enPassantPossibility = false;
                        }
                    }
                    else {
                        Piece* capturedPawn = getPieceAt(sf::Vector2i(newPosition.x, newPosition.y - 1));
                        if (capturedPawn && capturedPawn->getColor() == Color::WHITE) {
                            removePiece(capturedPawn);
                            enPassantPossibility = false;
                        }
                    }
                }
            }        

            // Handle capture and end of game logic
            if (targetPiece != nullptr && targetPiece->getColor() != draggedPiece->getColor()) {
                if (dynamic_cast<King*>(targetPiece)) {
                    // End game logic
                    std::cout << "Game over!";
                    if (targetPiece->getColor() == Color::WHITE) {
                        std::cout << "Black wins!" << std::endl;
                    }
                    else {
                        std::cout << "White wins!" << std::endl;
                    }
                    gameOver = true;
                }
                removePiece(targetPiece);
                enPassantPossibility = false;
            }

            // Set the new position and snap to the grid
            draggedPiece->setPosition(newPosition);
            draggedPiece->snapToGrid();
            //Handle Check check
            checkForCheck(currentTurn);
            if (checkCheck) {
                if (!dynamic_cast<King*>(draggedPiece)) {
                    // If the dragged piece is not the King, cancel the move
                    draggedPiece->snapToGrid();  // Return to original position
                    draggedPiece = nullptr;
                    return;
                }
                else {
                    // If the King is moving, ensure the new position is not under attack
                    for (const auto& piece : pieces) {
                        if (piece->getColor() != currentTurn) {  // Only check opponent's pieces

                            // Check if the opponent's piece can move to the new king's position
                            if (piece->isValidMove(newPosition, piece->getPosition(), getPieceAt(newPosition))) {

                                // Additional check for non-knight pieces: Make sure the path is clear
                                if (!dynamic_cast<Knight*>(piece.get()) && !isPathClear(newPosition, piece->getPosition())) {
                                    continue;  // Skip if the path to the king is not clear
                                }

                                // If an opponent piece can move to the new king's position, cancel the move
                                draggedPiece->snapToGrid();  // Return King to original position
                                draggedPiece = nullptr;
                                return;
                            }
                        }
                    }
                }
            }
            draggedPiece = nullptr;
            switchTurn(); // Switch turn only if the move is valid
        }
        else {
            // Invalid move, reset piece to original position
            draggedPiece->snapToGrid();
            draggedPiece = nullptr;
        }
    }
}


bool Board::isPathClear(sf::Vector2i newPosition, sf::Vector2i position) {
    int dx = newPosition.x - position.x;
    int dy = newPosition.y - position.y;

    // Vector of the movement
    int stepX = (dx == 0) ? 0 : (dx > 0) ? 1 : -1;
    int stepY = (dy == 0) ? 0 : (dy > 0) ? 1 : -1;

    sf::Vector2i currentPos = position;

    // Check squares on the way for newPosition
    while (currentPos != newPosition) {
        currentPos.x += stepX;
        currentPos.y += stepY;

        if (currentPos == newPosition) {
            break;
        }

        if (getPieceAt(currentPos) != nullptr) {
            return false;
        }
    }

    return true;
}


void Board::removePiece(Piece* pieceToRemove) {
    // Search for piece by position
    auto it = std::find_if(pieces.begin(), pieces.end(),
        [pieceToRemove](const std::unique_ptr<Piece>& piece) {
            return piece.get() == pieceToRemove;
        });
    // Delete piece if found
    if (it != pieces.end()) {
        pieces.erase(it);
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

int counter = 0;

// Render the board and pieces
void Board::renderBoard() {
    //window.clear();
    // TODO: Try to draw board once and re-render only chess pieces.
    //std::cout << "Board rendered " << counter << " times." << std::endl; // Display the render count
    //counter++;
    //if (!boardRendered) {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                window.draw(squares[row][col]); // Draw the squares
            }
        }
        boardRendered = true; // Set the flag to true after rendering the board
    //}

    for (const auto& piece : pieces) {
        piece->draw(window);
    }

    window.display();
}


void Board::switchTurn() {
    currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
}


void Board::checkForCheck(Color currentTurnColor) {
    // Get the opponent's king position
    Piece* opponentKing = nullptr;

    for (const auto& piece : pieces) {
        if (King* king = dynamic_cast<King*>(piece.get())) {  // Assuming pieces is a vector of unique_ptr or shared_ptr
            if (king->getColor() != currentTurnColor) {  // Opponent's king
                opponentKing = king;
                break;
            }
        }
    }

    sf::Vector2i kingPosition = opponentKing->getPosition();  // Get the opponent king's position

    // Check if any of the current player's pieces can move to the opponent king's position
    for (const auto& piece : pieces) {
        if (piece->getColor() == currentTurnColor) {  // Only check current player's pieces

            // Check if the piece can move to the opponent king's position
            if (piece->isValidMove(kingPosition, piece->getPosition(), getPieceAt(kingPosition))) {

                // Additional check for non-knight pieces: Make sure the path is clear
                if (!dynamic_cast<Knight*>(piece.get()) && !isPathClear(kingPosition, piece->getPosition())) {
                    continue;  // Skip if the path to the king is not clear
                }

                // The king is in check
                std::cerr << "CHECK!" << std::endl;
                checkCheck = true;
            }
        }
    }

    // No pieces threaten the opponent's king
    checkCheck = false;
}
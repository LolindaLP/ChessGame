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

    // TODO try to render board once
    //renderBoard();

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                // Case for mouse button press event (Left Mouse Button)
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        selectPiece(mousePos);
                    }
                    break;

                // Case for mouse movement (only handle if dragging)
                case sf::Event::MouseMoved:
                    if (draggedPiece) {
                        sf::Vector2f mousePosFloat = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        draggedPiece->sprite.setPosition(mousePosFloat - offset);  // Move dragged piece
                    }
                    break;

                // Case for mouse button release event
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        placePiece(mousePos);  // Place the piece in its final position
                    }
                    break;

                // Case for window resizing
                case sf::Event::Resized:
                {
                    unsigned int newSize = std::min(event.size.width, event.size.height);  // Maintain 1:1 aspect ratio
                    window.setSize(sf::Vector2u(newSize, newSize));  // Resize window to maintain square ratio
                    break;
                }

                default:
                    break;
                }
            }
    renderBoard();
    }
}
//TODO: Switch for different events.
//Closed
//isLMBPressed
//isLMBReleased
//isMouseMoved


void Board::selectPiece(const sf::Vector2i& mousePos) {
    // Convert mouse position from pixel coordinates to world coordinates
    sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

    // Get the row and column of the clicked tile based on the world coordinates
    // TODO: Exercise.
    // Instead of calculating world coordinates - try to use .contains() method.
    int row = static_cast<int>(worldMousePos.y) / tileSize;
    int col = static_cast<int>(worldMousePos.x) / tileSize;

    // Check if there is a piece at the position and if it's the correct color
    Piece* piece = getPieceAt(sf::Vector2i(col, row));
    sf::Vector2f tmp(mousePos.x, mousePos.y);
    //if (piece->sprite.getGlobalBounds().contains(tmp)) {
    //    std::cout << "CHUJ " << std::endl;
    //}
    if (piece != nullptr && piece->getColor() == currentTurn) {
        draggedPiece = piece;
        // TODO: Maybe don't need this second variable if we already have draggedPiece?
        selectedPiecePosition = piece->getPosition();

        // Get the resized bounds of the piece in world coordinates
        sf::FloatRect bounds = draggedPiece->sprite.getGlobalBounds();

        // Calculate the offset using world coordinates
        offset = sf::Vector2f(worldMousePos.x - bounds.left, worldMousePos.y - bounds.top);
    }
}


void Board::placePiece(const sf::Vector2i& mousePos) {
    if (draggedPiece) {
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
        int row = static_cast<int>(worldMousePos.y) / tileSize;
        int col = static_cast<int>(worldMousePos.x) / tileSize;
        sf::Vector2i newPosition(col, row);

        // Validate the move
        if (isValidMove(draggedPiece, newPosition)) {
            // Handle capture before special moves
            handleCapture(draggedPiece, newPosition);

            // Handle special moves like castling and en passant
            if (King* king = dynamic_cast<King*>(draggedPiece)) {
                handleCastling(king, newPosition);
            }
            else if (Pawn* pawn = dynamic_cast<Pawn*>(draggedPiece)) {
                handlePawnPromotion(pawn, newPosition);
                handleEnPassant(pawn, newPosition);
            }

            // Update the piece's position and snap it to the grid
            draggedPiece->setPosition(newPosition);
            draggedPiece->snapToGrid();

            // Now switch the turn before checking for check/checkmate
            switchTurn();

            // Check if the opponent is in check
            if (isInCheck(currentTurn)) {
                // If the opponent is in check, check for checkmate
                if (isCheckmate(currentTurn)) {
                    std::cout << "Checkmate! "
                        << (currentTurn == Color::WHITE ? "Black wins!" : "White wins!")
                        << std::endl;
                    gameOver = true;
                }
                else {
                    // Only in check, not checkmate
                    std::cout << "Check! "
                        << (currentTurn == Color::WHITE ? "White" : "Black")
                        << " must save their king!" << std::endl;
                }
            }
        }
        else {
            // Invalid move, reset piece to original position
            draggedPiece->snapToGrid();
        }

        // Reset dragged piece after move or reset
        draggedPiece = nullptr;
    }
}


bool Board::isValidMove(Piece* piece, const sf::Vector2i& newPosition) {
    Piece* targetPiece = getPieceAt(newPosition);

    // Check if the move is valid for the piece
    if (!piece->isValidMove(newPosition, piece->getPosition(), targetPiece)) {
        return false;
    }

    // For non-knight pieces, check if the path is clear
    if (!dynamic_cast<Knight*>(piece) && !isPathClear(newPosition, piece->getPosition())) {
        return false;
    }

    // Ensure the target isn't occupied by a piece of the same color
    if (targetPiece && targetPiece->getColor() == piece->getColor()) {
        return false;
    }

    // Simulate the move by temporarily removing the target piece (if any) from the board
    auto originalPosition = piece->getPosition();
    Piece* capturedPiece = targetPiece;  // Capture the target piece if it exists

    // Temporarily move the piece
    piece->setPosition(newPosition);

    // Temporarily remove the captured piece from the board by skipping it in validation
    if (capturedPiece) {
        capturedPiece->setPosition(sf::Vector2i(-1, -1));  // Set to an invalid position, or simply skip it
    }

    // Check if this move leaves the player's king in check
    bool validMove = !isInCheck(piece->getColor());

    // Restore the piece's original position
    piece->setPosition(originalPosition);

    // Restore the captured piece to its original position if it was removed
    if (capturedPiece) {
        capturedPiece->setPosition(newPosition);
    }

    return validMove;
}


bool Board::isInCheck(Color currentTurnColor) {
    // Get the current player's king
    Piece* currentKing = nullptr;

    // Find the current player's king
    for (const auto& piece : pieces) {
        if (King* king = dynamic_cast<King*>(piece.get())) {
            if (king->getColor() == currentTurnColor) {  // Current player's king
                currentKing = king;
                break;  // No need to check further, we found the king
            }
        }
    }

    // Ensure the current player's king is found
    if (currentKing == nullptr) return false;

    sf::Vector2i kingPosition = currentKing->getPosition();

    // Check if any of the opponent's pieces can move to the current king's position
    for (const auto& piece : pieces) {
        if (piece->getColor() != currentTurnColor) {  // Only check opponent's pieces

            // Check if the piece can move to the current king's position
            if (piece->isValidMove(kingPosition, piece->getPosition(), getPieceAt(kingPosition))) {

                // For non-knight pieces, ensure the path is clear
                if (!dynamic_cast<Knight*>(piece.get()) && !isPathClear(kingPosition, piece->getPosition())) {
                    continue;
                }

                // The king is in check
                return true;
            }
        }
    }

    return false;  // No opponent piece can attack the king
}


bool Board::isCheckmate(Color color) {
    for (const auto& piece : pieces) {
        if (piece->getColor() == color) {
            auto originalPosition = piece->getPosition();
            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    sf::Vector2i newPosition(col, row);
                    Piece* targetPiece = getPieceAt(newPosition);
                    if (piece->isValidMove(newPosition, originalPosition, targetPiece)) {
                        // Temporarily move the piece and check for check
                        piece->setPosition(newPosition);
                        if (!isInCheck(color)) {
                            piece->setPosition(originalPosition); // Restore position
                            return false; // Legal move found
                        }
                        piece->setPosition(originalPosition); // Restore position
                    }
                }
            }
        }
    }
    return true;
}


void Board::handleCastling(King* king, const sf::Vector2i& newPosition) {
    bool isWhiteKing = (king->getColor() == Color::WHITE);
    bool isBlackKing = (king->getColor() == Color::BLACK);

    // White king-side castling
    if (isWhiteKing && isPathClear(sf::Vector2i(4, 7), sf::Vector2i(7, 7)) && newPosition == sf::Vector2i(6, 7)) {
        moveRookForCastling(sf::Vector2i(7, 7), sf::Vector2i(5, 7)); // Move white rook from h1 to f1
    }
    // Handle other castling scenarios similarly
}


void Board::moveRookForCastling(const sf::Vector2i& rookPos, const sf::Vector2i& newPos) {
    Piece* rook = getPieceAt(rookPos);
    if (rook && dynamic_cast<Rook*>(rook)) {
        rook->setPosition(newPos);
        rook->snapToGrid();
    }
}


void Board::handlePawnPromotion(Pawn* pawn, const sf::Vector2i& newPosition) {
    bool isWhitePawn = (pawn->getColor() == Color::WHITE);
    bool isBlackPawn = (pawn->getColor() == Color::BLACK);

    // Check if pawn reaches the last row for promotion
    if ((isWhitePawn && newPosition.y == 0) || (isBlackPawn && newPosition.y == 7)) {
        // Remove the pawn
        removePiece(pawn);

        // Create and add a new Queen at the pawn's position
        auto newQueen = std::make_unique<Queen>(pawn->getColor(), newPosition);
        newQueen->snapToGrid();
        pieces.push_back(std::move(newQueen));
    }
}


void Board::handleEnPassant(Pawn* pawn, const sf::Vector2i& newPosition) {
    // En passant capture logic
    if (enPassantPossibility) {
        Piece* capturedPawn = nullptr;

        if (pawn->getColor() == Color::WHITE) {
            // White captures black pawn
            capturedPawn = getPieceAt(sf::Vector2i(newPosition.x, newPosition.y + 1));
        }
        else if (pawn->getColor() == Color::BLACK) {
            // Black captures white pawn
            capturedPawn = getPieceAt(sf::Vector2i(newPosition.x, newPosition.y - 1));
        }

        if (capturedPawn && capturedPawn->getColor() != pawn->getColor() && dynamic_cast<Pawn*>(capturedPawn)) {
            removePiece(capturedPawn);
            enPassantPossibility = false;
        }
    }
}


void Board::handleCapture(Piece* draggedPiece, const sf::Vector2i& newPosition) {
    // Check if there's a piece at the target position
    Piece* targetPiece = getPieceAt(newPosition);

    // If there is a piece, and it's of the opposite color, capture it
    if (targetPiece != nullptr && targetPiece->getColor() != draggedPiece->getColor()) {
        removePiece(targetPiece);  // Capture the piece
        enPassantPossibility = false;  // Reset en passant flag after any capture
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
        // TODO: Implement this on piece type
        //  void draw(const Drawable& drawable, const RenderStates& states = RenderStates::Default);
        // window.draw(piece.get())
        piece->draw(window);
    }

    window.display();
}


void Board::switchTurn() {
    currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

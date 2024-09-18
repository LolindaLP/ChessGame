#include "Piece.h"

// Constructor
Piece::Piece(Color color, sf::Vector2i position)
    : color(color), position(position) {}

// Destructor
Piece::~Piece() {}

// Draw method
void Piece::draw(sf::RenderWindow& window) {
    if (!texture.getSize().x || !texture.getSize().y) {
        return;
    }
    window.draw(sprite);
}

void Piece::snapToGrid()
{
    sprite.setPosition(position.x * tileSize, position.y * tileSize);
}

// Get position
sf::Vector2i Piece::getPosition() const { return position; }

// Set position
void Piece::setPosition(sf::Vector2i newPosition) { position = newPosition; }

// Load texture
bool Piece::loadTexture(const std::string& filename) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Error loading texture: " << filename << std::endl;
        return false;
    }
    sprite.setTexture(texture);
    return true;
}



#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window) : window(window) {
	// TODO: update defaults if needed
	blockWidth = 30;
}

void Renderer::renderGame(Board& board, Piece& piece) {
	// TODO: impl
	window.clear(sf::Color(22, 25, 40));

	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			// Render a rectangle, if desired
			// TODO: render different colors for different blocks
			// TODO: render differently?
			// Determine whether to render the piece or the existing board cell
			Block pieceCellHere = piece.getCell(x - piece.getLocx(), y - piece.getLocy());
			if (pieceCellHere != Block::None) {
				// Render the piece cell
				sf::RectangleShape toRender;
				toRender.setSize(sf::Vector2f(blockWidth, blockWidth));
				toRender.setPosition(sf::Vector2f(x * blockWidth, y * blockWidth));
				toRender.setFillColor(sf::Color(255, 255, 0));
				window.draw(toRender);
			} else {
				// Render the existing board cell
				if (board.getCell(x, y) != Block::None) {
					sf::RectangleShape toRender;
					toRender.setSize(sf::Vector2f(blockWidth, blockWidth));
					toRender.setPosition(sf::Vector2f(x * blockWidth, y * blockWidth));
					toRender.setFillColor(sf::Color(255, 255, 0));
					window.draw(toRender);
				}
			}
		}
	}

	window.display();
}

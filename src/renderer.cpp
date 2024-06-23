#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window) : window(window) {
	// TODO: update defaults if needed
	blockWidth = 30;
}

void Renderer::renderGame(Board& board, Piece& piece) {
	// Clear
	window.clear(sf::Color(22, 25, 40));
	// Render the board
	// TODO: special graphics effects (static 'noise', some cells are slightly darker than others, darker cell "gradients"/borders rendered via colors rather than images, non-intrusive line clearing fx, etc.)
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			// Render a rectangle, if desired
			// TODO: render different colors for different blocks
			// TODO: render differently?
			// Determine whether to render the piece or the existing board cell
			Block pieceCellHere = piece.getCell(x - piece.getLocx(), y - piece.getLocy());
			Block boardCellHere = board.getCell(x, y);
			if (pieceCellHere != Block::None) {
				// Render the piece cell
				// TODO: refactor
				sf::RectangleShape toRender;
				toRender.setSize(sf::Vector2f(blockWidth, blockWidth));
				toRender.setPosition(sf::Vector2f(x * blockWidth, y * blockWidth));
				toRender.setFillColor(sf::Color(blockToColor(pieceCellHere).r, blockToColor(pieceCellHere).g, blockToColor(pieceCellHere).b));
				window.draw(toRender);
			} else {
				// Render the existing board cell
				// TODO: refactor
				if (boardCellHere != Block::None) {
					sf::RectangleShape toRender;
					toRender.setSize(sf::Vector2f(blockWidth, blockWidth));
					toRender.setPosition(sf::Vector2f(x * blockWidth, y * blockWidth));
					toRender.setFillColor(sf::Color(blockToColor(boardCellHere).r, blockToColor(boardCellHere).g, blockToColor(boardCellHere).b));
					window.draw(toRender);
				}
			}
		}
	}
	// TODO: shadow piece

	window.display();
}

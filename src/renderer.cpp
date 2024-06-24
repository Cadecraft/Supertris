#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window) : window(window) {
	// TODO: update defaults if needed
	blockWidth = 30;
}

void Renderer::renderRect(int x, int y, int width, int height, Color color) {
	sf::RectangleShape toRender;
	toRender.setSize(sf::Vector2f(width, height));
	toRender.setPosition(sf::Vector2f(x, y));
	toRender.setFillColor(sf::Color(color.r, color.g, color.b));
	window.draw(toRender);
}

void Renderer::renderGame(Board& board, Piece& piece) {
	// Clear
	window.clear(sf::Color(22, 25, 40));
	// Render the board
	// TODO: special graphics effects (static 'noise', some cells are slightly darker than others, darker cell "gradients"/borders rendered via colors rather than images, non-intrusive line clearing fx, etc.)
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			// Render a rectangle, if desired
			// TODO: effects
			// Determine whether to render the piece or the existing board cell
			Block pieceCellHere = piece.getCell(x - piece.getLocx(), y - piece.getLocy());
			Block boardCellHere = board.getCell(x, y);
			if (pieceCellHere != Block::None) {
				// Render the piece cell
				renderRect(x * blockWidth, y * blockWidth, blockWidth, blockWidth, multColor(blockToColor(pieceCellHere), 0.9));
				renderRect(x * blockWidth + blockWidth / 10, y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth * 8 / 10, blockToColor(pieceCellHere));
			} else if (boardCellHere != Block::None) {
				// Render the existing board cell
				renderRect(x * blockWidth, y * blockWidth, blockWidth, blockWidth, multColor(blockToColor(boardCellHere), 0.9));
				renderRect(x * blockWidth + blockWidth / 10, y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth * 8 / 10, blockToColor(boardCellHere));
			} else {
				// Empty: render the edges of pieces, if needed
				// TODO: setting to enable/disable these effects
				if (RENDER_EDGES) {
					if (board.getCell(x - 1, y + 1) != Block::None) {
						// Bottom left corner (render first to get overlapped if needed)
						renderRect(x * blockWidth, y * blockWidth + 9 * blockWidth / 10, blockWidth / 10, blockWidth / 10, multColor(blockToColor(board.getCell(x - 1, y + 1)), 0.7));
					}
					if (board.getCell(x - 1, y) != Block::None) {
						// Left edge
						renderRect(x * blockWidth, y * blockWidth, blockWidth / 10, blockWidth, multColor(blockToColor(board.getCell(x - 1, y)), 0.7));
					}
					if (board.getCell(x, y + 1) != Block::None) {
						// Left edge
						renderRect(x * blockWidth, y * blockWidth + 9 * blockWidth / 10, blockWidth, blockWidth / 10, multColor(blockToColor(board.getCell(x, y + 1)), 0.7));
					}
				}
				// TODO: shadow piece
			}
		}
	}
	// TODO: shadow piece

	window.display();
}

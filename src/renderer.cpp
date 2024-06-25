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

void Renderer::renderGame(Board& board, Piece& piece, Bag& bag) {
	// Calculate the shadow piece
	Piece shadow(piece);
	for (int i = 0; i < 20; i++) {
		shadow.move(0, 1, board);
	}
	// Clear
	window.clear(sf::Color(19, 22, 36));
	// Render the board
	// TODO: special graphics effects (static 'noise', some cells are slightly darker than others, darker cell "gradients"/borders rendered via colors rather than images, non-intrusive line clearing fx, etc.)
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			// Render a rectangle, if desired
			// TODO: effects
			// Determine whether to render the piece or the existing board cell
			Block pieceCellHere = piece.getCell(x - piece.getLocx(), y - piece.getLocy());
			Block boardCellHere = board.getCell(x, y);
			Block shadowCellHere = shadow.getCell(x - shadow.getLocx(), y - shadow.getLocy());
			if (pieceCellHere != Block::None) {
				// Render the piece cell
				// TODO: refactor this piece of code
				renderRect(x * blockWidth, y * blockWidth, blockWidth, blockWidth, multColor(blockToColor(pieceCellHere), 0.9));
				renderRect(x * blockWidth + blockWidth / 10, y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth * 8 / 10, blockToColor(pieceCellHere));
				renderRect(x * blockWidth + blockWidth / 10, y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth / 10, lerpWhiteColor(blockToColor(pieceCellHere), 0.8));
			} else if (boardCellHere != Block::None) {
				// Render the existing board cell
				renderRect(x * blockWidth, y * blockWidth, blockWidth, blockWidth, multColor(blockToColor(boardCellHere), 0.9));
				renderRect(x * blockWidth + blockWidth / 10, y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth * 8 / 10, blockToColor(boardCellHere));
				renderRect(x * blockWidth + blockWidth / 10, y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth / 10, lerpWhiteColor(blockToColor(boardCellHere), 0.8));
			} else if (shadowCellHere != Block::None) {
				// Render the shadow cell
				renderRect(x * blockWidth, y * blockWidth, blockWidth, blockWidth, multColor(blockToColor(Block::Shadow), 0.9));
				renderRect(x * blockWidth + blockWidth / 10, y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth * 8 / 10, blockToColor(Block::Shadow));
			} else {
				// Empty: render the grid
				renderRect(x * blockWidth + blockWidth / 10, y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth * 8 / 10, { 22, 25, 40 });
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
			}
		}
	}
	// Show the queue
	// TODO: queue size config
	std::vector<Block> futurequeue = bag.viewFuture(5);
	int miniWidth = blockWidth / 2;
	int blockDisplayStartx = board.getWidth() * blockWidth + miniWidth;
	int blockDisplayStarty = miniWidth;
	for (int i = 0; i < futurequeue.size(); i++) {
		Block blockHere = futurequeue[i];
		// Display the block (smaller than usual, on the right side)
		for (int y = 0; y < PIECE_SIZE; y++) {
			for (int x = 0; x < PIECE_SIZE; x++) {
				Block cellHere = numberToBlock(PIECE_DATA[blockToIndex(blockHere)][0][y][x]);
				if (cellHere != Block::None) {
					renderRect(blockDisplayStartx + x * miniWidth, blockDisplayStarty + i * 4 * miniWidth + y * miniWidth, miniWidth, miniWidth, multColor(blockToColor(blockHere), 0.9));
					renderRect(blockDisplayStartx + x * miniWidth + miniWidth / 10, blockDisplayStarty + i * 4 * miniWidth + y * miniWidth + miniWidth / 10, miniWidth * 8 / 10, miniWidth * 8 / 10, blockToColor(blockHere));
					renderRect(blockDisplayStartx + x * miniWidth + miniWidth / 10, blockDisplayStarty + i * 4 * miniWidth + y * miniWidth + miniWidth / 10, miniWidth * 8 / 10, miniWidth / 10, lerpWhiteColor(blockToColor(blockHere), 0.8));
				}
			}
		}
	}

	window.display();
}

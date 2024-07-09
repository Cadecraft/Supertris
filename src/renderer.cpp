#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window, AssetHandler& assetHandler) : window(window), assetHandler(assetHandler) {
	// TODO: update defaults if needed
	blockWidth = 30;
}

void Renderer::renderRect(int x, int y, int width, int height, Color color, int opacity = 255) {
	sf::RectangleShape toRender;
	toRender.setSize(sf::Vector2f(width, height));
	toRender.setPosition(sf::Vector2f(x, y));
	toRender.setFillColor(sf::Color(color.r, color.g, color.b, opacity));
	window.draw(toRender);
}

void Renderer::renderText(int x, int y, int size, Color color, sf::Font& font, std::string str) {
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(size);
	text.setFillColor(sf::Color(color.r, color.g, color.b));
	text.setPosition(sf::Vector2f(x, y));
	window.draw(text);
}

void Renderer::renderBlock(int x, int y, int boardOffsetx, int boardOffsety, int widthToUse, Block cellHere, int sparkleValue = 0) {
	Color mainColor = blockToColor(cellHere);
	Color postEffectsColor = lerpWhiteColor(mainColor, (100 - sparkleValue) / 100.0);
	// Main block outside
	renderRect(boardOffsetx + x * widthToUse, boardOffsety + y * widthToUse, widthToUse, widthToUse, multColor(postEffectsColor, 0.9));
	// Main block inside
	renderRect(boardOffsetx + x * widthToUse + widthToUse / 10, boardOffsety + y * widthToUse + widthToUse / 10, widthToUse * 8 / 10, widthToUse * 8 / 10, postEffectsColor);
	// Top highlight (if needed)
	if (cellHere != Block::Shadow) {
		renderRect(boardOffsetx + x * widthToUse + widthToUse / 10, boardOffsety + y * widthToUse + widthToUse / 10, widthToUse * 8 / 10, widthToUse / 10, lerpWhiteColor(postEffectsColor, 0.8));
	}
}

void Renderer::renderGame(Board& board, Piece& piece, Bag& bag, Block holdBlock, Score& score, Effects& effects) {
	// Calculate the shadow piece
	Piece shadow(piece);
	for (int i = 0; i < 20; i++) {
		shadow.move(0, 1, board);
	}
	// Clear
	window.clear(sf::Color(19, 22, 36));
	// Render the board
	// TODO: special graphics effects (static 'noise', some cells are slightly darker than others, darker cell "gradients"/borders rendered via colors rather than images, non-intrusive line clearing fx, glitter/shine, etc.)
	int boardOffsety = blockWidth * 2;
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			// Render a rectangle, if desired
			// TODO: more effects
			// Determine whether to render the piece or the existing board cell
			Block pieceCellHere = piece.getCell(x - piece.getLocx(), y - piece.getLocy());
			Block boardCellHere = board.getCell(x, y);
			Block shadowCellHere = shadow.getCell(x - shadow.getLocx(), y - shadow.getLocy());
			if (pieceCellHere != Block::None) {
				// Render the piece cell
				renderBlock(x, y, 0, boardOffsety, blockWidth, pieceCellHere, effects.getSparkleValue(x, y));
			} else if (boardCellHere != Block::None) {
				// Render the existing board cell
				renderBlock(x, y, 0, boardOffsety, blockWidth, boardCellHere, effects.getSparkleValue(x, y));
			} else if (shadowCellHere != Block::None) {
				// Render the shadow cell
				renderBlock(x, y, 0, boardOffsety, blockWidth, Block::Shadow);
			} else {
				// Empty: render the grid
				renderRect(x * blockWidth + blockWidth / 10, boardOffsety + y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth * 8 / 10, { 22, 25, 40 });
				// Empty: render the edges of pieces, if needed
				// TODO: setting to enable/disable these effects
				if (RENDER_EDGES) {
					if (board.getCell(x - 1, y + 1) != Block::None) {
						// Bottom left corner (render first to get overlapped if needed)
						renderRect(x * blockWidth, boardOffsety + y * blockWidth + 9 * blockWidth / 10, blockWidth / 10, blockWidth / 10, multColor(blockToColor(board.getCell(x - 1, y + 1)), 0.7));
					}
					if (board.getCell(x - 1, y) != Block::None) {
						// Left edge
						renderRect(x * blockWidth, boardOffsety + y * blockWidth, blockWidth / 10, blockWidth, multColor(blockToColor(board.getCell(x - 1, y)), 0.7));
					}
					if (board.getCell(x, y + 1) != Block::None) {
						// Left edge
						renderRect(x * blockWidth, boardOffsety + y * blockWidth + 9 * blockWidth / 10, blockWidth, blockWidth / 10, multColor(blockToColor(board.getCell(x, y + 1)), 0.7));
					}
				}
			}
			// Effects
			// Beam
			if (y == effects.getBeamLocy()) {
				renderRect(0, boardOffsety + y * blockWidth + 9 * blockWidth / 10, blockWidth * board.getWidth(), blockWidth / 10, { 255, 255, 255 }, effects.getBeamStrength() * 255 / 100);
			}
		}
	}
	// Flash the screen if needed
	if (effects.getFlashStrength() > 0) {
		// Flash
		renderRect(0, boardOffsety, blockWidth * board.getWidth(), blockWidth * board.getHeight(), effects.getFlashColor(), effects.getFlashStrength() * 255 / 100);
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
					renderBlock(x, y, blockDisplayStartx, blockDisplayStarty + i * 4 * miniWidth, miniWidth, cellHere);
				}
			}
		}
	}
	// Show the hold block
	for (int y = 0; y < PIECE_SIZE; y++) {
		for (int x = 0; x < PIECE_SIZE; x++) {
			Block cellHere = numberToBlock(PIECE_DATA[blockToIndex(holdBlock)][0][y][x]);
			if (cellHere != Block::None) {
				renderBlock(x, y, miniWidth, boardOffsety + miniWidth, miniWidth, cellHere);
			}
		}
	}
	// TODO: Animations for clearing lines, placing pieces, etc.
	// TODO: UI: Show controls info
	sf::Font& font = assetHandler.getFont();
	renderText(
		board.getWidth() * blockWidth, board.getHeight() * blockWidth - 6 * blockWidth, 18,
		{ 77, 84, 112 }, font, "R = restart\nESC = exit"
	);
	// Other color: { 146, 152, 176 }
	// UI: Show current score data
	// TODO: improve appearance, show towards top of screen
	std::string scoreString1 = "";
	scoreString1 += "Score: ";
	scoreString1 += std::to_string(score.points);
	scoreString1 += "\nLines: ";
	scoreString1 += std::to_string(score.lines);
	std::string scoreString2 = "";
	scoreString2 += "Time: ";
	scoreString2 += std::to_string((int) (score.timeElapsed / 1000));
	if (score.currStreak > 0) {
		scoreString2 += "\nB2B: ";
		scoreString2 += std::to_string(score.currStreak);
	}
	//renderText(board.getWidth() * blockWidth, board.getHeight() * blockWidth - 4 * blockWidth, 18, { 77, 84, 112 }, font, scoreString);
	renderText(blockWidth, 10, 18, { 145, 151, 179 }, font, scoreString1);
	renderText(blockWidth * 5, 10, 18, { 145, 151, 179 }, font, scoreString2);
	window.display();
}

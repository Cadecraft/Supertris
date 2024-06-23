#pragma once

// Stores a color value
struct Color {
	int r;
	int g;
	int b;
};

// Stores a block
enum class Block {
	None,
	Z,
	T,
	S,
	O,
	L,
	J,
	I
};

// Convert a block to its number for indexing into `constants`; return -1 if invalid
inline int blockToIndex(Block block) {
	switch (block) {
	case Block::Z:
		return 0;
	case Block::T:
		return 1;
	case Block::S:
		return 2;
	case Block::O:
		return 3;
	case Block::L:
		return 4;
	case Block::J:
		return 5;
	case Block::I:
		return 6;
	default:
		return -1;
	}
}

// Convert a numerical representation in `constants` cells to its block
inline Block numberToBlock(int number) {
	switch (number) {
	case 0:
		return Block::None;
	case 1:
		return Block::Z;
	case 2:
		return Block::T;
	case 3:
		return Block::S;
	case 4:
		return Block::O;
	case 5:
		return Block::L;
	case 6:
		return Block::J;
	case 7:
		return Block::I;
	default:
		return Block::None;
	}
}

// Convert a block to a color
inline Color blockToColor(Block block) {
	switch (block) {
	case Block::Z:
		// Red
		return { 201, 12, 34 };
	case Block::T:
		// Purple
		return { 113, 14, 171 };
	case Block::S:
		// Green
		return { 68, 199, 42 };
	case Block::O:
		// Yellow
		return { 227, 192, 34 };
	case Block::L:
		// Orange
		return { 214, 98, 15 };
	case Block::J:
		// Blue
		return { 16, 52, 196 };
	case Block::I:
		// Cyan
		return { 37, 184, 217 };
	default:
		// None (should not happen)
		return { 0, 0, 0 };
	}
}

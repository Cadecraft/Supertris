#pragma once

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

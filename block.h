#pragma once

#include <array>
#include <random>
#include "coord.h"

constexpr uint64_t NUM_OF_BLOCK_TYPE = 7;
constexpr uint64_t NUM_OF_BLOCK_SHAPE = 4;
using Block = std::array<Coord, NUM_OF_BLOCK_SHAPE>;

constexpr std::array<Block, NUM_OF_BLOCK_TYPE> blocks{
	Block{ Coord{-1,0}, Coord{0,0}, Coord{1,0}, Coord{1,1} },     // L1
	Block{ Coord{-1,0}, Coord{0,0}, Coord{1,0}, Coord{2,0} },     // I
	Block{ Coord{-1,0}, Coord{-1,1},Coord{0,0}, Coord{1,0} },     // L2
	Block{ Coord{-1,0}, Coord{0,0}, Coord{0,1}, Coord{1,1} },     // S
	Block{ Coord{0,0},  Coord{0,1}, Coord{1,0}, Coord{1,1} },     // O
	Block{ Coord{-1,1}, Coord{0,0}, Coord{0,1}, Coord{1,0} },     // Z
	Block{ Coord{-1,0}, Coord{0,0}, Coord{0,1}, Coord{1,0} }      // T
}; 

void clockwiseRotateBlock(Block& block) {
	for (auto& coord : block) {
		clockwiseRotateCoord90(coord);
	}
}

void counterClockwiseRotateBlock(Block& block) {
	for (auto& coord : block) {
		counterClockwiseRotateCoord90(coord);
	}
}

class RandomBlockGen {
	std::random_device rd;  
	std::mt19937 gen{ rd() };
	std::uniform_int_distribution<int> typeGen{ 0, NUM_OF_BLOCK_TYPE - 1 };
	std::uniform_int_distribution<int> shapeGen{ 0, NUM_OF_BLOCK_SHAPE - 1 };
public:
	RandomBlockGen(){}

	Block operator()() {
		Block temp = blocks[typeGen(gen)];

		auto end = shapeGen(gen);
		for (auto i = 0; i <= end;++i) {
			clockwiseRotateBlock(temp);
		}

		return temp;
	}
};

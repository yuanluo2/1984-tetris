#pragma once

#include "coord.h"
#include "block.h"

template<uint64_t WIDTH, uint64_t HEIGHT, uint64_t startX, uint64_t startY>
using Board = std::array<std::array<uint64_t, WIDTH>, HEIGHT + startY>;

template<uint64_t WIDTH, uint64_t HEIGHT, uint64_t startX, uint64_t startY>
class TetrisMap {
	Coord coord;
	Board<WIDTH, HEIGHT, startX, startY> board;
	Block currentBlock;
	Block nextBlock;
	RandomBlockGen gen;
	bool gameOver;
public:
	TetrisMap() :
		coord{ startX, startY },
		gen{},
		board{},
		gameOver{ false }
	{
		currentBlock = gen();
		nextBlock = gen();
	}
private:
	bool checkCollision() {
		for (const auto& temp : currentBlock) {
			auto x = coord.x + temp.x;
			auto y = coord.y + temp.y;

			if (board[y][x] == 1 || x < 0 || x >= WIDTH || y >= HEIGHT + startY) {
				return true;
			}
		}

		return false;
	}

	void eraseLines() {
		auto pred = [](uint64_t i) {
			return i == 1;
		};

		for (auto i = startY; i < HEIGHT + startY; ++i) {
			if (std::ranges::all_of(board[i], pred)) {
				for (auto r = i; r > startY; --r) {
					board[r] = std::move(board[r - 1]);
				}
			}
		}
	}
public:
	const Block& getCurrentBlock() const { return currentBlock; }
	const Coord& getCurrentBlockCoord() const { return coord; }
	const Block& getNextBlock() const { return nextBlock; }
	bool isGameOver() const { return gameOver; }
	const Board<WIDTH, HEIGHT, startX, startY>& getBoard() const { return board; }

	void goLeft() {
		--coord.x;

		if (checkCollision()) {
			++coord.x;
		}
	}

	void goRight() {
		++coord.x;

		if (checkCollision()) {
			--coord.x;
		}
	}

	void rotate() {
		clockwiseRotateBlock(currentBlock);

		if (checkCollision()) {
			counterClockwiseRotateBlock(currentBlock);
		}
	}

	void goDown() {
		++coord.y;

		if (checkCollision()) {
			// 0. back to previous coord.y.
			--coord.y;

			if (coord.y == startY) {
				gameOver = true;
			}

			// 1. merge current block into board.
			for (const auto& temp : currentBlock) {
				auto x = coord.x + temp.x;
				auto y = coord.y + temp.y;

				board[y][x] = 1;
			}

			// 2. erase lines.
			eraseLines();

			// 3. next block assigns to current, and remake next block.
			currentBlock = nextBlock;
			nextBlock = gen();

			// 4. reset coord.x and coord.y.
			coord.x = startX;
			coord.y = startY;
		}
	}
};

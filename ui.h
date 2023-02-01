#pragma once

#include <iostream>
#include <string>
#include "tetrisMap.h"

template<uint64_t WIDTH, uint64_t HEIGHT, uint64_t startX, uint64_t startY>
void drawTetrisMap(TetrisMap<WIDTH, HEIGHT, startX, startY>& tm) {
	static Board<WIDTH, HEIGHT, startX, startY> buffer;

	const auto& board = tm.getBoard();
	const auto& block = tm.getCurrentBlock();
	const auto& blockCoord = tm.getCurrentBlockCoord();

	for (auto r = 0; r < HEIGHT + startY;++r) {
		for (auto c = 0; c < WIDTH;++c) {
			buffer[r][c] = board[r][c];
		}
	}

	for (const auto& temp : block) {
		auto x = blockCoord.x + temp.x;
		auto y = blockCoord.y + temp.y;

		buffer[y][x] = 1;
	}

	std::string str;

	for (auto r = startY; r < HEIGHT + startY;++r) {
		str += "<!";
		for (const auto& i : buffer[r]) {
			if (i == 0) {
				str += " .";
			}
			else {
				str += "[]";
			}
		}

		str += "!>\n";
	}

	str += "<!";
	for (auto i = 0; i < WIDTH;++i) {
		str += "==";
	}
	str += "!>\n";

	str += "  ";
	for (auto i = 0; i < WIDTH;++i) {
		str += R"(\/)";
	}
	str += "  ";

	std::cout << str;
}

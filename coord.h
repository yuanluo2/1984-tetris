#pragma once
#include <cstdint>

struct Coord {
	int64_t x, y;
};

inline void counterClockwiseRotateCoord90(Coord& coord) {
	auto [x, y] = coord;
	coord.x = y;
	coord.y = -x;
}

inline void clockwiseRotateCoord90(Coord& coord) {
	auto [x, y] = coord;
	coord.x = -y;
	coord.y = x;
}

#pragma once
#include <stdint.h>

typedef int8_t Tile;
enum TileType : Tile {
	Empty=-1,
	T,
	O,
	L,
	J,
	Z,
	S,
	I,
	Garbage
};

typedef uint8_t Connection;
enum Side : Connection {
    Up    = 0b1000,
    Right = 0b0000,
    Down  = 0b0010,
    Left  = 0b0001,
};
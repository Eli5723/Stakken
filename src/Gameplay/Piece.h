#pragma once

#include "./PieceDefinitions.h"
#include "./Tile.h"

class Piece {
    public:
    int rotation_index;
    Tile type;

    Tile* tiles;
    Connection* connections;

    int x,y;
    Piece(int x,int y, Tile type);
    
    inline Tile tileAt(int x, int y) { return tiles[x + 4 * y]; };
	inline Connection connectionAt(int x, int y) { return connections[x + y * 4]; }

    inline void moveLeft() {x--;}
	inline void moveRight() {x++;}
	inline void moveDown() {y++;}
	inline void moveUp() {y--;}

    void rotateCCW();
	void rotateCW();
	void flip();

};
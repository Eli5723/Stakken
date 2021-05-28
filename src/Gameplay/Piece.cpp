#include "./Piece.h"

void Piece::rotateCCW(){
	if (type == TileType::O)
		return;

	rotation_index = (rotation_index + 3) % 4;

	tiles = PieceDefintions::getPieceTiles(type, rotation_index);
	connections = PieceDefintions::getPieceConnections(type, rotation_index);
}

void Piece::rotateCW(){
	if (type == TileType::O)
		return;

	rotation_index = (rotation_index + 1) % 4;

	tiles = PieceDefintions::getPieceTiles(type, rotation_index);
	connections = PieceDefintions::getPieceConnections(type, rotation_index);
}

void Piece::flip(){
	if (type == TileType::O)
		return;

	rotation_index = (rotation_index + 2) % 4;

	tiles = PieceDefintions::getPieceTiles(type, rotation_index);
	connections = PieceDefintions::getPieceConnections(type, rotation_index);
}
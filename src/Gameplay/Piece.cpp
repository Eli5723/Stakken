#include "./Piece.h"

void Piece::rotateCCW(){
	if (type == PIECETYPE::O)
		return;

	rotation_index = (rotation_index + 3) % 4;

	tiles = GetPieceDef(type, rotation_index);
	connections = GetPieceConn(type, rotation_index);
}

void Piece::rotateCW(){
	if (type == PIECETYPE::O)
		return;

	rotation_index = (rotation_index + 1) % 4;

	tiles = GetPieceDef(type, rotation_index);
	connections = GetPieceConn(type, rotation_index);
}

void Piece::flip(){
	if (type == PIECETYPE::O)
		return;

	rotation_index = (rotation_index + 2) % 4;

	tiles = GetPieceDef(type, rotation_index);
	connections = GetPieceConn(type, rotation_index);
}
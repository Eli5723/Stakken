#include "./Board.h"
#include "./Piece.h"
#include "./Tile.h"
#include "../Systems/Rendering/Renderer.h"
#include <SDL2/SDL.h>

// Constants
const int kBoardWidth = 10;
const int kBoardHeight = 18;
const int kOverflowRows = 4;
const int kTota = kBoardHeight + kOverflowRows;

// Helper Row Class
Board::Row::Row() {
	contents = new Tile[kBoardWidth];
	connections = new Connection[kBoardWidth];
	empty();
}

Board::Row::~Row() {
	delete[] contents;
	delete[] connections;
}

inline bool Board::Row::isFull() {
	return fill == kBoardWidth;
}

inline void Board::Row::maskConnections(int mask) {
	for (int i = 0; i < kBoardWidth; i++)
		connections[i] &= mask;
}

// Empties a provided row
void Board::Row::makeFull() {
	const uint8_t CONN_MID = Side::Left | Side::Right | Side::Down;

	memset(contents, TileType::Empty, kBoardWidth);
	memset(connections, CONN_MID, kBoardWidth);
	fill = kBoardWidth;
}

void Board::Row::empty() {
	memset(contents, TileType::Empty, sizeof(Tile) * kBoardWidth);
	memset(connections, 0, kBoardWidth);
	fill = 0;
}

// The Board 
Board::Board() {
	rows = new Row* [kTotalRows];
	for (int i = 0; i < kTotalRows; i++) {
		rows[i] = new Row();
	}
}

Board::~Board() {
	for (int i = 0; i < kTotalRows; i++) {
		delete rows[i];
	}

	delete[] rows;
}

void Board::ApplyPiece(Piece* piece) {
	int linesCleared = 0;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (piece->tileAt(i, j) == TileType::Empty){
				continue;
			}

			rows[piece->y + j]->contents[i + piece->x] = piece->tileAt(i, j);
			rows[piece->y + j]->connections[i+ piece->x] = piece->connectionAt(i, j);
			rows[piece->y + j]->fill++;
		}

		if (piece->y + j < kTotalRows &&rows[piece->y + j]->isFull()) {
			removeLine(piece->y + j);
			linesCleared++;
		}
	}
}

void Board::addLine(int hole) {

	// Save a reference to the top row, it will become the garbage
	Row* garbageRow = rows[0];

	// Shift other rows up
	for (int i = 0; i < kTota - 1; i++) {
		rows[i] = rows[i + 1];
	}

	// Fill the row and att it to the bottom
	garbageRow->makeFull();
	rows[kTota - 1] = garbageRow;

	// Create a hole and remove relavent connections
	garbageRow->contents[hole] = -1;

	if (rows[kTota - 2]->contents[hole] == TileType::Empty)
		rows[kTota - 2]->connections[hole] &= ~Side::Down;

	// Remove connections to the hole
	if (hole > 0)
		garbageRow->connections[hole - 1] &= ~Side::Right;
	else if (hole + 1 > kBoardWidth)
		garbageRow->connections[hole + 1] &= ~Side::Left;
}

bool Board::checkFit(Piece* piece) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (piece->tileAt(i, j) == TileType::Empty)
				continue;
			if (piece->x + i< 0)
				return false;

			if (piece->y + j >= kTotalRows){
				return false;
			}
			if (i + piece->x >= kBoardWidth)
				return false;

			//Check if overlapping with another piece
			if (tileAt(piece->x + i, piece->y + j) != TileType::Empty){
				return false;
			}
		}
	}

	return true;
}
 
bool Board::offsetTest(Piece* piece){
	if (checkFit(piece)) //-1,0
		return true;
	piece->x--;
	if (checkFit(piece)) //-1,0
		return true;
	piece->x+=2; //1,0
	if (checkFit(piece))
		return true;
	piece->x--; //0,-1
	piece->y++;
	if (checkFit(piece))
		return true;
	piece->x--; //-1,-1
	if (checkFit(piece))
		return true;
	piece->x+=2; //1,-1
	if (checkFit(piece))
		return true;
	piece->y--;
	piece->x-=3; //-2,0
	if (checkFit(piece))
		return true;
	piece->x+=4; //2,0
	if (checkFit(piece))
		return true;
	piece->x-=2;

	return false;
}

void Board::clear() {
	for (int i = 0; i < kTota; i++) {
		rows[i]->empty();
	}
}

void Board::removeLine(int row) {

	// Remove Connections above and below by applying a mask
	if (row < kTota - 1)
		rows[row + 1]->maskConnections(~Side::Up);
	else if (row > 0)
		rows[row - 1]->maskConnections(~Side::Down);

	// Save a reference to the row being removed
	Row* removed = rows[row];

	// Shift every above row downwards
	for (int i = row; i > 0; i--)
		rows[i] = rows[i - 1];

	// Empty the row and place it at the top
	removed->empty();

	rows[0] = removed;
}
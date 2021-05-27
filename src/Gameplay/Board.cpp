#include "./Board.h"
#include "./Piece.h"
#include "./Tile.h"
#include "../Systems/Rendering/Renderer.h"

// Constants
const int BoardWidth = 10;
const int BoardHeight = 18;
const int AdditionalRows = 4;
const int TotalRows = BoardHeight + AdditionalRows;

// Helper Row Class
inline Row::Row() {
	contents = new Tile[BoardWidth];
	connections = new Connection[BoardWidth];
	fill = 0;
}

inline Row::~Row() {
	delete[] contents;
	delete[] connections;
}

inline bool Row::isFull() {
	return fill == BoardWidth;
}

inline void Row::maskConnections(int mask) {
	for (int i = 0; i < BoardWidth; i++)
		connections[i] &= mask;
}

// Empties a provided row
void emptyRow(Row* row) {
	memset(row->contents, TileType::Empty, sizeof(Tile) * BoardWidth);
	memset(row->connections, 0, BoardWidth);

	row->fill = 0;
}

// Fills the swapRow
void fillRow(Row* row) {
	const uint8_t CONN_MID = Side::Left | Side::Right | Side::Down;

	memset(row->contents, TileType::Empty, BoardWidth);
	memset(row->connections, CONN_MID, BoardWidth);
	row->fill = BoardWidth;
}

// The Board 
Board::Board() {
	rows = new Row * [BoardHeight];
	for (int i = 0; i < BoardHeight; i++) {
		rows[i] = new Row();
	}
}

Board::~Board() {
	for (int i = 0; i < BoardHeight; i++) {
		delete rows[i];
	}

	delete[] rows;
}

inline Tile Board::tileAt(int x, int y) { return rows[y]->contents[x]; }

void Board::ApplyPiece(Piece* piece) {
	int linesCleared = 0;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (piece->tileAt(i, j) < 0)
				continue;

			rows[piece->y + j]->connections[i] = piece->connectionAt(i, j);
		}

		if (rows[piece->y + j]->isFull()) {

			linesCleared++;
		}
	}
}

void Board::addLine(int hole) {

	// Save a reference to the top row, it will become the garbage
	Row* garbageRow = rows[0];

	// Shift other rows up
	for (int i = 0; i < TotalRows - 1; i++) {
		rows[i] = rows[i + 1];
	}

	// Fill the row and att it to the bottom
	fillRow(garbageRow);
	rows[TotalRows - 1] = garbageRow;

	// Create a hole and remove relavent connections
	garbageRow->contents[hole] = -1;

	if (rows[TotalRows - 2]->contents[hole] == TileType::Empty)
		rows[TotalRows - 2]->connections[hole] &= ~Side::Down;

	// Remove connections to the hole
	if (hole > 0)
		garbageRow->connections[hole - 1] &= ~Side::Right;
	else if (hole + 1 > BoardWidth)
		garbageRow->connections[hole + 1] &= ~Side::Left;
}

bool Board::pieceFits(Piece* piece) {
	for (int i = 0, j = 0; i < 4; j++, i++) {
		if (piece->tileAt(i, j) == -1)
			continue;

		if (piece->x < 0)
			return false;

		if (i + piece->x > BoardWidth)
			return false;

		if (j + piece->y >= BoardHeight)
			return false;

		//Check if overlapping with another piece
		if (tileAt(piece->x + i, piece->y + j != -1))
			return false;
	}

	return true;
}
 
void Board::clear() {
	for (int i = 0; i < TotalRows; i++) {
		emptyRow(rows[i]);
	}
}

void Board::removeLine(int row) {

	// Remove Connections above and below by applying a mask
	if (row < TotalRows - 1)
		rows[row + 1]->maskConnections(~Side::Up);
	else if (row > 0)
		rows[row - 1]->maskConnections(~Side::Down);

	// Save a reference to the row being removed
	Row* removed = rows[row];

	// Shift every above row downwards
	for (int i = row; i > 0; i--)
		rows[i] = rows[i - 1];

	// Empty the row and place it at the top
	emptyRow(removed);

	rows[0] = removed;
}
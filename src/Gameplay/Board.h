#include "./Piece.h"

struct Row {
	Tile* contents;
	uint8_t* connections;

	int fill;

	Row();

	~Row();

	bool isFull();

	inline void maskConnections(int mask);
};

struct Board {
	Row** rows;

	Board();
	~Board();

	inline Tile tileAt(int x, int y);

	void ApplyPiece(Piece* piece);

	void removeLine(int row);

	void clear();

	void addLine(int hole);

	bool pieceFits(Piece* piece);
};


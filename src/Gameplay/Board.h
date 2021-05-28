#pragma once
#include "./Piece.h"

struct Board {
	static const int kWidth = 10;
	static const int kHeight = 18;
	static const int kOverflowRows = 4;
	static const int kTotalRows = kHeight + kOverflowRows;

	struct Row {
		Tile* contents;
		uint8_t* connections;

		int fill;

		Row();
		~Row();

		bool isFull();

		void makeFull();
		void empty();

		inline void maskConnections(int mask);
	};

	Row** rows;

	Board();
	~Board();

	inline Tile tileAt(int x, int y){ return rows[y]->contents[x]; };

	void ApplyPiece(Piece* piece);

	void removeLine(int row);

	void clear();

	void addLine(int hole);

	bool checkFit(Piece* piece);
	bool offsetTest(Piece* piece);
};


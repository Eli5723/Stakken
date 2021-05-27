#pragma once
#include "./Board.h"
#include "../Piece.h"
#include "../Systems/Profile.h"
#include "../Renderer.h"

class Game {
	public:

	Board* board;
	Piece* heldPiece;

	Game() {
		board = new Board();
		heldPiece = nullptr;



	}
};


void RenderBoard(Board* board, glm::vec2 position, float scale, Profile* profile) {
	const float PieceWidth = 16;
	const float BorderWidth = 2;

	// Draw background

	// Draw Pieces

	// Renderer::DrawQuad(position, );
}
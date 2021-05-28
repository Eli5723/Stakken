#pragma once

#include "./Board.h"
#include "./Piece.h"
#include "./Tile.h"
#include "../Systems/Input/InputBuffer.h"

#include <SDL2/SDL.h>

#include <cmath>

const int kInitialPieceX = 4;
const int kInitialPieceY = 0;

struct Game {
	public:

	Board* board;
	
	Piece* heldPiece;
	Piece* nextPiece;

	int time;

	Game() {
		board = new Board();
		
		heldPiece = new Piece( kInitialPieceX, kInitialPieceY, (TileType)(rand()%7), 0);
		nextPiece = new Piece( kInitialPieceX, kInitialPieceY, (TileType)(rand()%7), 0);
	}

	void Update(int dt){

	}

	void ApplyInput(InputBuffer& input){
		if (input.actionCount > 0)
		for (int i=0; i < input.actionCount; i++){
			switch(input.actions[i]){
				case TetrisAction::Left:
					heldPiece->moveLeft();
					if (!board->checkFit( heldPiece )) {
						heldPiece->moveRight();
					}
				break;

				case TetrisAction::Right:
					heldPiece->moveRight();
					if (!board->checkFit( heldPiece ))
						heldPiece->moveLeft();
				break;

				case TetrisAction::SoftDrop:
					heldPiece->moveDown();
					if (!board->checkFit(heldPiece)){
						heldPiece->moveUp();
						ApplyHeldPiece();
					}
				break;

				case TetrisAction::SonicDrop:
					while (board->checkFit(heldPiece)){
						heldPiece->moveDown();
					}
					heldPiece->moveUp();
				break;

				case TetrisAction::HardDrop:
					while (board->checkFit(heldPiece))
						heldPiece->moveDown();
					heldPiece->moveUp();
					ApplyHeldPiece();
				break;

				case TetrisAction::Flip:
					heldPiece->flip();
					if (!board->offsetTest(heldPiece))
						heldPiece->flip();
				break;

				case TetrisAction::RCW:
					heldPiece->rotateCW();
					if (!board->offsetTest(heldPiece))
						heldPiece->rotateCCW();
				break;

				case TetrisAction::RCCW:
					heldPiece->rotateCCW();
					if (!board->offsetTest(heldPiece))
						heldPiece->rotateCW();
				break;

				case TetrisAction::None:
				default:
				break;
			}
		}
	}

	void ApplyHeldPiece(){
		board->ApplyPiece(heldPiece);
		heldPiece = nextPiece;
		nextPiece = new Piece( kInitialPieceX, kInitialPieceY, (TileType)(rand()%7), 0);
	}
};
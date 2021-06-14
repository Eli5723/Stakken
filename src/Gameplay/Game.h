#pragma once

#include "./Board.h"
#include "./Piece.h"
#include "./Tile.h"
#include "../Systems/Input/InputBuffer.h"
#include "../Systems/Input/InputProfile.h"
#include "./Randomizers/xoroshiroRandomizer.h"
#include "../Systems/Assets/Assets.h"

#include <SDL2/SDL.h>

#include <cmath>

const int kInitialPieceX = 4;
const int kInitialPieceY = 0;

struct Game {
	public:
	enum GameState {
		Disabled,
		Playing
	};
	int state;

	// Ugly association
	InputProfile* profile;

	Board* board;
	
	Piece* heldPiece;
	Piece* nextPiece;

	xoroshiroRandomizer pieceRandomizer;
	xoroshiroRandomizer holeRandomizer;

	// Stats
	int time;
	int clears;
	int pieces;
	int lastPlacement;
	int speed;

	Game(InputProfile* profile) {
		this->profile = profile;

		// TODO: Start Inactive
		state = GameState::Playing;

		board = new Board();

		TileType firstPieceType = (TileType)(pieceRandomizer.next()%7);
		TileType nextPieceType = (TileType)(pieceRandomizer.next()%7);

		heldPiece = new Piece( kInitialPieceX, kInitialPieceY, firstPieceType, profile->rotation[firstPieceType]);
		nextPiece = new Piece( kInitialPieceX, kInitialPieceY, nextPieceType, profile->rotation[nextPieceType]);

		// Stats
		time = 0;
		clears = 0;
		pieces = 0;
		lastPlacement = 0;
		speed = 0;
	}

	void Reset(){
		board->clear();
		TileType firstPieceType = (TileType)(pieceRandomizer.next()%7);
		TileType nextPieceType = (TileType)(pieceRandomizer.next()%7);

		heldPiece->Convert(kInitialPieceX, kInitialPieceY, firstPieceType, profile->rotation[firstPieceType]);
		nextPiece->Convert(kInitialPieceX, kInitialPieceY, nextPieceType, profile->rotation[nextPieceType]);

		// Stats
		time = 0;
		clears = 0;
		pieces = 0;
	}

	void Win(){
		state = GameState::Disabled;
	}

	void Lose(){
		state = GameState::Disabled;
	}

	~Game(){
		delete heldPiece;
		delete nextPiece;
	}

	void Update(int dt){
		time += dt;
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
		// If the piece cannot possibly be applied, lose
		if (!board->checkFit(heldPiece)) {
			state = Disabled;
			return;
		}
		// Apply Piece to board
		int cleared = board->ApplyPiece(heldPiece);

		// Play Sounds
		activeAssets.sound_lock->play();
		if (cleared > 0) {
			activeAssets.sound_lineclear->play();
		}

		// Get next piece
		Piece* temp = heldPiece;
		heldPiece = nextPiece;
		TileType nextPieceType = (TileType)(pieceRandomizer.next()%7);

		nextPiece = temp->Convert(kInitialPieceX, kInitialPieceY, nextPieceType, profile->rotation[nextPieceType]);

		// Increment stats
		pieces += 1;
		clears+= cleared;
		if (lastPlacement != 0)
			speed = time - lastPlacement;
		lastPlacement = time;

		// Check Rules: TODO: make dynamic
		if (this->clears >= 40) {
			Win();
		}
	}
};
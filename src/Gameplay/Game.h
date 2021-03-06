#pragma once

#include "./Board.h"
#include "./Piece.h"
#include "./Tile.h"
#include "../Systems/Input/InputBuffer.h"
#include "../Systems/Input/InputProfile.h"
#include "./Randomizers/xoroshiroRandomizer.h"
#include "../Systems/Assets/Assets.h"

#include "./Replay.h"

#include <SDL2/SDL.h>

#include <SDL2/SDL_log.h>
#include <cmath>

const int kInitialPieceX = 3;
const int kInitialPieceY = 0;

struct Game {
	public:
	enum GameState {
		Disabled,
		Playing
	};
	int state;

	// Ugly association
	int* rotationTable;

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

	// Replay Recorder
	Recorder* recorder = 0;

	// Gameplay variables
	int gravityTime = 300;
	int gravityTimer = gravityTime;

	int creepTime = 2000;
	int creepTimer = creepTime;


	Game(InputProfile* profile, int seed) {
		this->rotationTable = profile->rotation;

		// TODO: Start Inactive, unseeded?
		state = GameState::Playing;

		board = new Board();

		// Apply seed
		pieceRandomizer.seed(seed);
		holeRandomizer.seed(seed);
		
		if (recorder)
			recorder->Reset(seed);

		TileType firstPieceType = (TileType)(pieceRandomizer.next()%7);
		TileType nextPieceType = (TileType)(pieceRandomizer.next()%7);

		heldPiece = new Piece( 0,0,0,0);
		nextPiece = new Piece( 0,0,0,0);

		// Configuration
		gravityTime = 300;
		gravityTimer = gravityTime;

		// Stats
		time = 0;
		clears = 0;
		pieces = 0;
		lastPlacement = 0;
		speed = 0;
	}

	void Reset(int seed){
		pieceRandomizer.seed(seed);
		holeRandomizer.seed(seed);
		
		if (recorder)
			recorder->Reset(seed);

		time = 0;
		gravityTimer = gravityTime;
		creepTimer = creepTime;
		clears = 0;
		pieces = 0;
		lastPlacement = 0;
		speed = 0;
		board->clear();
		TileType firstPieceType = (TileType)(pieceRandomizer.next()%7);
		TileType nextPieceType = (TileType)(pieceRandomizer.next()%7);

		heldPiece->Convert(kInitialPieceX, kInitialPieceY, firstPieceType, rotationTable[firstPieceType]);
		nextPiece->Convert(kInitialPieceX, kInitialPieceY, nextPieceType, rotationTable[nextPieceType]);

		// Stats
		time = 0;
		clears = 0;
		pieces = 0;
		lastPlacement = 0;
		speed = 0;
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
		if (recorder)
			recorder->advance(dt);

		time += dt;

		// Gravity
		gravityTimer -= dt;
		while (gravityTimer <= 0){
			gravity();
			gravityTimer += gravityTime;
			recorder->record(TetrisAction::Gravity);
		}

		// Survivor Garbage Creep
		creepTimer -= dt;
		while (creepTimer <= 0){
			creep();
			creepTimer += creepTime;
			recorder->record(TetrisAction::AddRow);
		} 

		if (recorder)
			recorder->saveFrame();
	}

	inline void gravity(){
		heldPiece->moveDown();
		if (!board->checkFit(heldPiece)){
			heldPiece->moveUp();
			ApplyHeldPiece();
		}
	}

	inline void creep(){
		if (heldPiece->y > 0)
				heldPiece->moveUp();
			board->addLine(holeRandomizer.next()%10);
	}

	void UpdateReplay(int dt){
		time += dt;
	}

	void ApplyInput(InputBuffer& input){
		for (int i=0; i < input.actionCount; i++){
			if (recorder)
						recorder->record(input.actions[i]);

			switch(input.actions[i]){
				// Input Events

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
					} else {
						gravityTimer = gravityTime;
					}
				break;

				case TetrisAction::SonicDrop: {
					int distance = 0;

					while (board->checkFit(heldPiece)){
						heldPiece->moveDown();
						distance++;
					}
					heldPiece->moveUp();

					// Prevent stalling
					if (distance > 1){
						gravityTimer = gravityTime;
					}
				} break;

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
				break;heldPiece->moveDown();
					if (!board->checkFit(heldPiece)){
						heldPiece->moveUp();
						ApplyHeldPiece();
					}
				case TetrisAction::RCCW:
					heldPiece->rotateCCW();
					if (!board->offsetTest(heldPiece))
						heldPiece->rotateCW();
				break;

				// Game Events
				case TetrisAction::Gravity:
					gravity();
				break;

				case TetrisAction::AddRow:
					creep();
				break;
				
				case TetrisAction::None:
				default:
				SDL_LogWarn(0, "Game recieved malformed input!");
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

		nextPiece = temp->Convert(kInitialPieceX, kInitialPieceY, nextPieceType, rotationTable[nextPieceType]);

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
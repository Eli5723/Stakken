#include "./RenderGame.h"

#include <glm/glm.hpp>

#include "../Systems/Rendering/Renderer.h"

const float kTileSize = 16.0f;

const glm::vec2 kTileDimensions{kTileSize, kTileSize}; // 16 x 16
const glm::vec2 kPieceDimensions{kTileSize * 4, kTileSize * 4}; // 64 x 64
const glm::vec2 kBoardDimensions{kTileSize * Board::kWidth, kTileSize * Board::kHeight}; //160 x 288

float kMargin = 1.0f;
const glm::vec2 kMarginDimensions{kMargin, kMargin};

void DrawPiece(const glm::vec2& position, Piece& piece){
    for (int y=0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            if (piece.tileAt(x,y) != TileType::Empty)
                Renderer::DrawQuad(position + glm::vec2{kTileSize * x, kTileSize * y}, kTileDimensions, {1,1,1,1});
        }
    }
}

void DrawBoard(const glm::vec2& position, Board& board){
    // Draw "Board" background + Border
    Renderer::QuadBox(position, kBoardDimensions, kMargin, {1,1,1,1});
    Renderer::DrawQuad(position, kBoardDimensions, {0,0,0,1});

    //Draw Tiles
    for (int y = 0; y < Board::kHeight;y++){
        for (int x=0; x < Board::kWidth; x++){
            if (board.tileAt(x,y + Board::kOverflowRows) != TileType::Empty)
                Renderer::DrawQuad(position + glm::vec2{kTileSize * x, kTileSize * y}, kTileDimensions,{1,1,1,1});
        }
    }
}

void DrawGame(const glm::vec2& position, Game& game){
    DrawBoard(position, *game.board);
    DrawPiece(position + glm::vec2(game.heldPiece->x * kTileSize,(game.heldPiece->y - Board::kOverflowRows) * kTileSize), *game.heldPiece);
    
    // Draw Ghost Piece
    int oldY = game.heldPiece->y;
    while (game.board->checkFit(game.heldPiece))
        game.heldPiece->moveDown();
    game.heldPiece->moveUp();

    DrawPiece(position + glm::vec2(game.heldPiece->x * kTileSize,(game.heldPiece->y - Board::kOverflowRows) * kTileSize), *game.heldPiece);
    game.heldPiece->y = oldY;
}
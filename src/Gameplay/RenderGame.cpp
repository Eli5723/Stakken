#include "./RenderGame.h"

#include <glm/glm.hpp>

#include "../Systems/Rendering/Renderer.h"
#include "./Board.h"
#include "./Piece.h"

const int kTileSize = 16;

const glm::vec2 kTileDimensions{kTileSize, kTileSize}; // 16 x 16
const glm::vec2 kPieceDimensions{kTileSize * 4, kTileSize * 4}; // 64 x 64
const glm::vec2 kBoardDimensions{kTileSize * Board::kWidth, kTileSize * Board::kHeight}; //160 x 288

void DrawPiece(const glm::vec2& position, Piece& piece){
    for (int y=0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            if (piece.tileAt(x,y) != TileType::Empty)
                Renderer::DrawQuad(position + glm::vec2{kTileSize * x, kTileSize * y}, kTileDimensions, {1,1,1,1});
        }
    }
}

void DrawBoard(const glm::vec2& position, Board& board){
    //Draw Background
    Renderer::DrawQuad(position, kBoardDimensions, {0,0,0,1});

    //Draw Tiles
    for (int y = 0; y < Board::kHeight;y++){
        for (int x=0; x < Board::kWidth; x++){
            if (board.tileAt(x,y + Board::kOverflowRows) != TileType::Empty)
                Renderer::DrawQuad(position + glm::vec2{kTileSize * x, kTileSize * y}, kTileDimensions,{1,1,1,1});
        }
    }
}
#include "./RenderGame.h"

#include <glm/glm.hpp>

#include "../Systems/Rendering/Renderer.h"
#include "../Systems/Assets/Texture.h"

const float kTileSize = 16.0f;

const glm::vec2 kTileDimensions{kTileSize, kTileSize}; // 16 x 16
const glm::vec2 kPieceDimensions{kTileSize * 4, kTileSize * 4}; // 64 x 64

const glm::vec2 kBoardDimensions{kTileSize * Board::kWidth, kTileSize * Board::kHeight}; //160 x 288
const glm::vec2 kStatsDimensions = glm::vec2{kPieceDimensions.x, kBoardDimensions.y - (kGaps + kPieceDimensions.y)};

const glm::vec2 kGameDimensions = kBoardDimensions + kStatsDimensions + glm::vec2{kGaps*2,0};

float kMargin = 1.0f;
float kGaps = 8.0f;
const glm::vec2 kMarginDimensions{kMargin, kMargin};

void DrawPiece(const glm::vec2& position, Piece& piece, Identity& identity, Texture& texture){
    for (int y=0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            if (piece.tileAt(x,y) != TileType::Empty){
                
                glm::vec2 tilePosition = position + glm::vec2{kTileSize * x, kTileSize * y};
                Renderer::DrawQuad(tilePosition, kTileDimensions, texture.id, identity.color_table.entries[piece.type]);

                const Connection connections = piece.connectionAt(x,y);
                if ((connections & Side::Up) ^ Side::Up)
                    Renderer::DrawQuad(tilePosition + glm::vec2{0,-1},glm::vec2{kTileSize,1},{1,1,1,1});
                
                if ((connections & Side::Down) ^ Side::Down)
                    Renderer::DrawQuad(tilePosition + glm::vec2{0,kTileSize-1},glm::vec2{kTileSize,1},{1,1,1,1});

                if ((connections & Side::Left) ^ Side::Left)
                    Renderer::DrawQuad(tilePosition,glm::vec2{1,kTileSize},{1,1,1,1});

                if ((connections & Side::Right) ^ Side::Right)
                    Renderer::DrawQuad(tilePosition + glm::vec2{kTileSize-1,0},glm::vec2{1,kTileSize},{1,1,1,1});

            }
        }
    }
}

void DrawGhostPiece(const glm::vec2& position, Piece& piece, Identity& identity, Texture& texture){
    for (int y=0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            if (piece.tileAt(x,y) != TileType::Empty)
                Renderer::DrawQuad(position + glm::vec2{kTileSize * x, kTileSize * y}, kTileDimensions, texture.id, identity.color_table.entries[piece.type] * glm::vec4{1.0f, 1.0f, 1.0f, 0.5f});
        }
    }
}

void DrawBoard(const glm::vec2& position, Board& board, Identity& identity, Texture& texture){
    // Draw "Board" background + Border
    Renderer::QuadBox(position, kBoardDimensions, kMargin, {1,1,1,1});
    Renderer::DrawQuad(position, kBoardDimensions, {0,0,0,1});

    //Draw Tiles
    for (int y = Board::kOverflowRows; y < Board::kTotalRows; y++){
        for (int x=0; x < Board::kWidth; x++){
            int type = board.tileAt(x,y);
            if (type != TileType::Empty) {
                
                const glm::vec2 tilePosition = position + glm::vec2{kTileSize * x, kTileSize * (y - Board::kOverflowRows)};
                Renderer::DrawQuad(tilePosition, kTileDimensions, texture.id, identity.color_table.entries[(int)type]);
                
                const Connection connections = board.connectionAt(x, y);

                // // Cultris Style Outlines
                // if ((connections & Side::Up) ^ Side::Up)
                //     Renderer::DrawQuad(tilePosition + glm::vec2{0,-1},glm::vec2{kTileSize+1,1},{1,1,1,1});
                
                // if ((connections & Side::Down) ^ Side::Down)
                //     Renderer::DrawQuad(tilePosition + glm::vec2{0,kTileSize},glm::vec2{kTileSize,1},{1,1,1,1});

                // if ((connections & Side::Left) ^ Side::Left)
                //     Renderer::DrawQuad(tilePosition,glm::vec2{1,kTileSize},{1,1,1,1});

                // if ((connections & Side::Right) ^ Side::Right)
                //     Renderer::DrawQuad(tilePosition + glm::vec2{kTileSize,0},glm::vec2{1,kTileSize+1},{1,1,1,1}); 

                // TGM Style Outlines
                if (board.tileAt(x,y-1) == TileType::Empty)
                    Renderer::DrawQuad(tilePosition + glm::vec2{0,-1},glm::vec2{kTileSize+1,1},{1,1,1,1});
                
                if (y < Board::kTotalRows-1&& board.tileAt(x,y+1) == TileType::Empty)
                    Renderer::DrawQuad(tilePosition + glm::vec2{0,kTileSize},glm::vec2{kTileSize,1},{1,1,1,1});

                if (x > 0 && board.tileAt(x-1,y) == TileType::Empty)
                    Renderer::DrawQuad(tilePosition,glm::vec2{1,kTileSize},{1,1,1,1});

                if (x < Board::kWidth && board.tileAt(x+1,y) == TileType::Empty)
                    Renderer::DrawQuad(tilePosition + glm::vec2{kTileSize,0},glm::vec2{1,kTileSize+1},{1,1,1,1}); 
            }
        }
    }
}

void DrawGame(const glm::vec2& position, Game& game, Identity& identity, Texture& texture){
    DrawBoard(position, *game.board, identity, texture);
    DrawPiece(position + glm::vec2(game.heldPiece->x * kTileSize,(game.heldPiece->y - Board::kOverflowRows) * kTileSize), *game.heldPiece, identity, texture);
    
    // Draw Ghost Piece
    int oldY = game.heldPiece->y;
    while (game.board->checkFit(game.heldPiece))
        game.heldPiece->moveDown();
    game.heldPiece->moveUp();

    DrawGhostPiece(position + glm::vec2(game.heldPiece->x * kTileSize,(game.heldPiece->y - Board::kOverflowRows) * kTileSize), *game.heldPiece, identity, texture);
    game.heldPiece->y = oldY;

    // Draw Next Piece Preview
    const glm::vec2 previewPosition = position  + glm::vec2{kBoardDimensions.x + kGaps,0};
    Renderer::QuadBox(previewPosition, kPieceDimensions, kMargin, {1,1,1,1});
    Renderer::DrawQuad(previewPosition, kPieceDimensions, {0,0,0,1});
    DrawPiece(previewPosition, *game.nextPiece, identity, texture);

    // Draw Game Stats
    const glm::vec2 statsPosition = previewPosition + glm::vec2{0,kGaps + kPieceDimensions.y};

    Renderer::QuadBox(statsPosition, kStatsDimensions, kMargin, {1,1,1,1});
    Renderer::DrawQuad(statsPosition, kStatsDimensions, {0,0,0,1});
}
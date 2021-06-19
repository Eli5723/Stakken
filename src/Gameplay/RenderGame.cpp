#include "./RenderGame.h"

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

#include "../Systems/Rendering/Renderer.h"
#include "../Systems/Assets/Texture.h"
#include "PieceDefinitions.h"

inline int min(int x){
    return (x > 0 ? x : 0);
}

namespace RenderGame {

    Options options{
        .outlineStyle = OutlineStyle::TGM,
        .backgroundOpacity = .9f
    };

    void cycleOutlineStyles(){
         options.outlineStyle = (RenderGame::options.outlineStyle + 1) % RenderGame::kOutlineStyles;
    }
    
    float kMargin = 2.0f;
    float kGaps = 16.0f;

    const float kTileSize = 30.0f;

    const glm::vec2 kTileDimensions{kTileSize, kTileSize}; // 16 x 16
    const glm::vec2 kPieceDimensions{kTileSize * 4, kTileSize * 4}; // 64 x 64

    const glm::vec2 kBoardDimensions{kTileSize * Board::kWidth, kTileSize * Board::kHeight}; //160 x 288
    const glm::vec2 kPreviewOffset = glm::vec2{kBoardDimensions.x + kGaps,0};
    const glm::vec2 kStatsDimensions = glm::vec2{kPieceDimensions.x, kBoardDimensions.y - (kGaps + kPieceDimensions.y)};

    const float kProfilePictureSize = 64.0f;
    const glm::vec2 kPFPDimensions{kProfilePictureSize,kProfilePictureSize};
    const glm::vec2 kIdentityDimensions = glm::vec2{kProfilePictureSize,kProfilePictureSize + kGaps * 2};
    const glm::vec2 kGameDimensions = kBoardDimensions + glm::vec2{kStatsDimensions.x + kGaps*2, kIdentityDimensions.y};

    static float pixelThickness = 1.0f;

    void SetPixelTickness(float thickness){
        pixelThickness = thickness;
    }

    void DrawPiece(const glm::vec2& position, Piece& piece, Identity& identity, Texture* texture){
        for (int y=0; y < 4; y++){
            for (int x = 0; x < 4; x++){
                if (piece.tileAt(x,y) != TileType::Empty){
                    
                    glm::vec2 tilePosition = position + glm::vec2{kTileSize * x, kTileSize * y};

                    if (texture)
                        Renderer::DrawQuad(tilePosition, kTileDimensions, texture->id, identity.color_table.entries[piece.type]);
                    else
                        Renderer::DrawQuad(tilePosition, kTileDimensions, identity.color_table.entries[piece.type]);


                    const Connection connections = piece.connectionAt(x,y);
                    if ((connections & Side::Up) ^ Side::Up)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0,-1},glm::vec2{kTileSize,pixelThickness},{1,1,1,1});
                    
                    if ((connections & Side::Down) ^ Side::Down)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0,kTileSize-1},glm::vec2{kTileSize,pixelThickness},{1,1,1,1});

                    if ((connections & Side::Left) ^ Side::Left)
                        Renderer::DrawQuad(tilePosition,glm::vec2{pixelThickness,kTileSize},{1,1,1,1});

                    if ((connections & Side::Right) ^ Side::Right)
                        Renderer::DrawQuad(tilePosition + glm::vec2{kTileSize-1,0},glm::vec2{pixelThickness,kTileSize},{1,1,1,1});

                }
            }
        }
    }

    // Draw a piece without having a piece object; used to draw pieces in the options menu
    void DrawPiece(const glm::vec2& position, ColorTable* colorTable, Texture* texture, int type, int rotation){
        Tile* tiles = PieceDefintions::getPieceTiles(type, rotation);
        Connection* connections = PieceDefintions::getPieceConnections(type, rotation);

        for (int y = 0; y < 4; y++){
            for (int x = 0; x < 4; x++){
                if (tiles[x + y * 4] != TileType::Empty){
                    glm::vec2 tilePosition = position + glm::vec2{kTileSize * x, kTileSize * y};
                    
                    if (texture)
                        Renderer::DrawQuad(tilePosition, kTileDimensions, texture->id, colorTable->entries[type]);
                    else
                        Renderer::DrawQuad(tilePosition, kTileDimensions, colorTable->entries[type]);

                    const Connection connection = connections[x + y * 4];
                    if ((connection & Side::Up) ^ Side::Up)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0,-1},glm::vec2{kTileSize,pixelThickness},{1,1,1,1});
                    
                    if ((connection & Side::Down) ^ Side::Down)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0,kTileSize-1},glm::vec2{kTileSize,pixelThickness},{1,1,1,1});

                    if ((connection & Side::Left) ^ Side::Left)
                        Renderer::DrawQuad(tilePosition,glm::vec2{pixelThickness,kTileSize},{1,1,1,1});

                    if ((connection & Side::Right) ^ Side::Right)
                        Renderer::DrawQuad(tilePosition + glm::vec2{kTileSize-1,0},glm::vec2{pixelThickness,kTileSize},{1,1,1,1});
                }
            }
        }
    }

    void DrawPiece(const glm::vec2& position, Piece& piece, Identity& identity, Texture* texture, int cutoff){
        for (int y=cutoff; y < 4; y++){
            for (int x = 0; x < 4; x++){
                if (piece.tileAt(x,y) != TileType::Empty){
                    
                    glm::vec2 tilePosition = position + glm::vec2{kTileSize * x, kTileSize * y};
                    
                    if (texture)
                        Renderer::DrawQuad(tilePosition, kTileDimensions, texture->id, identity.color_table.entries[piece.type]);
                    else
                        Renderer::DrawQuad(tilePosition, kTileDimensions, identity.color_table.entries[piece.type]);

                    const Connection connections = piece.connectionAt(x,y);
                    if ((connections & Side::Up) ^ Side::Up)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0,-1},glm::vec2{kTileSize,pixelThickness},{1,1,1,1});
                    
                    if ((connections & Side::Down) ^ Side::Down)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0,kTileSize-1},glm::vec2{kTileSize,pixelThickness},{1,1,1,1});

                    if ((connections & Side::Left) ^ Side::Left)
                        Renderer::DrawQuad(tilePosition,glm::vec2{pixelThickness,kTileSize},{1,1,1,1});

                    if ((connections & Side::Right) ^ Side::Right)
                        Renderer::DrawQuad(tilePosition + glm::vec2{kTileSize-1,0},glm::vec2{pixelThickness,kTileSize},{1,1,1,1});

                }
            }
        }
    }


    void DrawGhostPiece(const glm::vec2& position, Piece& piece, Identity& identity, Texture* texture){
        for (int y=0; y < 4; y++){
            for (int x = 0; x < 4; x++){
                if (piece.tileAt(x,y) != TileType::Empty) {
                    // Because the background of the board is black, we can get away with tinting the ghost piece a darker shade to "cheat" the transparency. This feels better when the background of the board is not opaque
                    if (texture)
                        Renderer::DrawQuad(position + glm::vec2{kTileSize * x, kTileSize * y}, kTileDimensions, texture->id, identity.color_table.entries[piece.type] * glm::vec4{.5f, .5f, .5f, 1.0f});
                    else
                        Renderer::DrawQuad(position + glm::vec2{kTileSize * x, kTileSize * y}, kTileDimensions, identity.color_table.entries[piece.type] * glm::vec4{.5f, .5f, .5f, 1.0f});;

                }
            }
        }
    }

    void DrawBoard(const glm::vec2& position, Board& board, Identity& identity, Texture* texture){
        // Background
        Renderer::DrawQuad(position, kBoardDimensions, {0,0,0,options.backgroundOpacity});

        //Draw Tiles
        for (int y = Board::kOverflowRows; y < Board::kTotalRows; y++){
            for (int x=0; x < Board::kWidth; x++){
                int type = board.tileAt(x,y);
                if (type != TileType::Empty) {
                    
                    const glm::vec2 tilePosition = position + glm::vec2{kTileSize * x, kTileSize * (y - Board::kOverflowRows)};

                    if (texture)
                        Renderer::DrawQuad(tilePosition, kTileDimensions, texture->id, identity.color_table.entries[(int)type]);
                    else 
                        Renderer::DrawQuad(tilePosition, kTileDimensions, identity.color_table.entries[(int)type]);

                    if (options.outlineStyle == 1) {
                    // Cultris Style Outlines
                        const Connection connections = board.connectionAt(x, y);
                        glm::vec4 outlineColor = identity.color_table.entries[(int)type] * glm::vec4{1.5f,1.5f,1.5,1}; 
                        if ((connections & Side::Up) ^ Side::Up)
                            Renderer::DrawQuad(tilePosition + glm::vec2{0,-1},glm::vec2{kTileSize+1,1},outlineColor);
                        
                        if ((connections & Side::Down) ^ Side::Down)
                            Renderer::DrawQuad(tilePosition + glm::vec2{0,kTileSize},glm::vec2{kTileSize,1},outlineColor);

                        if ((connections & Side::Left) ^ Side::Left)
                            Renderer::DrawQuad(tilePosition,glm::vec2{1,kTileSize},outlineColor);

                        if ((connections & Side::Right) ^ Side::Right)
                            Renderer::DrawQuad(tilePosition + glm::vec2{kTileSize,0},glm::vec2{1,kTileSize+1},outlineColor); 
                    } else if (options.outlineStyle == 2) {
                    // TGM Style Outlines
                        if (board.tileAt(x,y-1) == TileType::Empty)
                            Renderer::DrawQuad(tilePosition + glm::vec2{0,-1},glm::vec2{kTileSize+1,pixelThickness},{1,1,1,1});
                        
                        if (y < Board::kTotalRows-1&& board.tileAt(x,y+1) == TileType::Empty)
                            Renderer::DrawQuad(tilePosition + glm::vec2{0,kTileSize},glm::vec2{kTileSize,pixelThickness},{1,1,1,1});

                        if (x > 0 && board.tileAt(x-1,y) == TileType::Empty)
                            Renderer::DrawQuad(tilePosition,glm::vec2{pixelThickness,kTileSize},{1,1,1,1});

                        if (x < Board::kWidth && board.tileAt(x+1,y) == TileType::Empty)
                            Renderer::DrawQuad(tilePosition + glm::vec2{kTileSize,0},glm::vec2{pixelThickness,kTileSize+1},{1,1,1,1});
                    }
                }
            }
        }

        // Border
        Renderer::QuadBox(position, kBoardDimensions, pixelThickness, {1,1,1,1});
    }

    void DrawGame(const glm::vec2& position, Game& game, Identity& identity, Texture* texture){
        
        // Draw Profile Picture
        if (identity.pfp)
            Renderer::DrawQuad(position, kPFPDimensions, identity.pfp->id);
        else
            Renderer::QuadBox(position, kPFPDimensions, pixelThickness * -1.0f, {1.0f,1,1,1});

        // Draw Name
        Renderer::DrawQuad(position + glm::vec2{kPFPDimensions.x + kGaps*.5,0},{200.0f,64.0f},{0.0f,0.0f,0.0f,0.5f});
        Renderer::DrawStr(position + glm::vec2{kPFPDimensions.x + kGaps,0}, 1.5f, identity.name, activeAssets.font);

        const glm::vec2 boardPosition = position + glm::vec2{0, kProfilePictureSize + kGaps};


        // GAMEPLAY 
        DrawBoard(boardPosition, *game.board, identity, texture);

        // Draw Ghost Piece
        int oldY = game.heldPiece->y;
        while (game.board->checkFit(game.heldPiece))
            game.heldPiece->moveDown();
        game.heldPiece->moveUp();

        DrawGhostPiece(boardPosition + glm::vec2(game.heldPiece->x * kTileSize,(game.heldPiece->y - Board::kOverflowRows) * kTileSize), *game.heldPiece, identity, texture);
        game.heldPiece->y = oldY;

        // Draw Actual Piece after ghost piece to prevent blending 
        DrawPiece(boardPosition + glm::vec2(game.heldPiece->x * kTileSize,(game.heldPiece->y - Board::kOverflowRows) * kTileSize), *game.heldPiece, identity, texture, ::min(Board::kOverflowRows - game.heldPiece->y));

        // Draw Next Piece Preview
        const glm::vec2 previewPosition = boardPosition  + kPreviewOffset;
        Renderer::QuadBox(previewPosition, kPieceDimensions, pixelThickness, {1,1,1,1});
        Renderer::DrawQuad(previewPosition, kPieceDimensions, {0,0,0,options.backgroundOpacity});
        DrawPiece(previewPosition, *game.nextPiece, identity, texture);

        // END OF GAMEPLAY
 
        if (game.state == Game::GameState::Disabled){
            Renderer::DrawQuad(boardPosition, kBoardDimensions, {0,0,0,.5f});
            Renderer::DrawStrC(boardPosition + glm::vec2{kBoardDimensions.x/2.0f, kBoardDimensions.y/3.0f},2.0f,"Game Over", activeAssets.font);
        }

        // Draw Game Stats
        const glm::vec2 statsPosition = previewPosition + glm::vec2{0,kGaps + kPieceDimensions.y};
        Renderer::QuadBox(statsPosition, kStatsDimensions, pixelThickness, {1,1,1,1});
        Renderer::DrawQuad(statsPosition, kStatsDimensions, {0,0,0,options.backgroundOpacity});
        
        char stats[50];
        float bpm = (float)game.pieces/((float)game.time/60000.0f);
        sprintf(stats,"Time:\n%.2f\nClears:\n%i\nPieces:\n%i",(float)game.time/1000.0f,game.clears,game.pieces);
        Renderer::DrawStr(statsPosition, 1.0f, stats, activeAssets.font);
        
        // Speedometer
        char bpmStr[5];
        sprintf(bpmStr,"%.0f",bpm);
        Renderer::DrawNeedle(statsPosition+ glm::vec2{kStatsDimensions.x/2,kStatsDimensions.y - 100}, 55.0f, glm::radians(180.0f + (bpm)));
        Renderer::DrawStrC(statsPosition+ glm::vec2{kStatsDimensions.x/2 -10, kStatsDimensions.y - 100}, 2.0f, bpmStr, activeAssets.font);
    }

}
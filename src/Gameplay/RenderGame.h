#pragma once

#include <glm/glm.hpp>

#include "./Board.h"
#include "./Piece.h"
#include "./Game.h"

#include "../Systems/Identity.h"
#include "../Systems/Assets/Assets.h"

namespace RenderGame {
    enum OutlineStyle {
        NONE,
        CULTRIS,
        TGM
    };
    
    const int kOutlineStyles = 3;

    struct Options {
        int outlineStyle = OutlineStyle::TGM;
        float backgroundOpacity = .8f;
    };

    void cycleOutlineStyles();

    extern Options options;

    void SetPixelTickness(float thickness);

    extern const float kTileSize;

    extern const glm::vec2 kTileDimensions; // 16 x 16
    extern const glm::vec2 kPieceDimensions; // 64 x 64
    extern const glm::vec2 kBoardDimensions; //160 x 288
    extern const glm::vec2 kGameDimensions; 

    extern float kMargin;
    extern float kGaps;
    extern const glm::vec2 kMarginDimensions;

    void SetPixelTickness(float thickness);

    void DrawPiece(const glm::vec2& position, Piece& piece, Identity& identity, Texture* texture);

    // Draw a piece without having a piece object; used to draw pieces in the options menu
    void DrawPiece(const glm::vec2& position, ColorTable* colorTable, Texture* texture, int type, int rotation);

    void DrawBoard(const glm::vec2& position, Board& board, Identity& identity, Texture* texture);
    void DrawGame(const glm::vec2& position, Game& game, Identity& identity, Texture* texture);

    extern const float kTileSize;

    extern const glm::vec2 kTileDimensions; // 16 x 16
    extern const glm::vec2 kPieceDimensions; // 64 x 64
    extern const glm::vec2 kBoardDimensions; //160 x 288
    extern const glm::vec2 kGameDimensions; 

    extern float kMargin;
    extern float kGaps;
    extern const glm::vec2 kMarginDimensions;
}
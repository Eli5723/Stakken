#pragma once

#include <glm/glm.hpp>

#include "./Board.h"
#include "./Piece.h"

void DrawPiece(const glm::vec2& position, Piece& piece);
void DrawBoard(const glm::vec2& position, Board& board);
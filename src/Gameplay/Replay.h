#include "../Systems/TetrisAction.h"
#include "Board.h"

#include <vector>

using std::vector;

// The Place a piece was when it was locked
struct PieceRecord {
    int x, y, rotation, type;
};

// A frame of a replay
struct ReplayFrame {
    int actionCount;
    int time;
};

// The Record of a removed line for playing replays backwards
struct RowRecord {
    Tile tiles[Board::kWidth];
    Connection connections[Board::kWidth];
    int y;
};


struct Replay {

    vector<TetrisAction> actions;
    vector<ReplayFrame> frames;
    vector<RowRecord> rows;



    int pos;
};
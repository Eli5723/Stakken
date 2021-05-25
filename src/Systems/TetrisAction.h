#pragma once

// For debugging purposes
const char* TetrisActionNames[9] = {
    "None",
    "Left",
    "Right",
    "Soft Drop",
    "Hard Drop",
    "Sonic Drop",
    "RCW",
    "RCCW",
    "Flip",
};

enum class TetrisAction {
    None,
    Left,
    Right,
    SoftDrop,
    HardDrop,
    SonicDrop,
    RCW,
    RCCW,
    Flip
};
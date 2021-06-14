#pragma once

enum class TetrisAction {
    None,
    Left,
    Right,
    SoftDrop,
    HardDrop,
    SonicDrop,
    RCW,
    RCCW,
    Flip,
    RemoveRow,
};
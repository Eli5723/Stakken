#pragma once
#include <cstdint>
enum class TetrisAction : uint8_t {
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
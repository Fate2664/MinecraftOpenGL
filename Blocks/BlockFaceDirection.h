#pragma once
#include <cstdint>

enum class BlockFaceDirection : std::uint8_t
{
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom,
    
    Count
};

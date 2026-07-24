#pragma once
#include <cstdint>

enum class BlockType : std::uint8_t //using std::uint8_t reduces each block type to just one byte
{
    Empty,
    Dirt,
    Grass,
    OakWood,
    Leaf,
    Stone,
    Coal,
    Iron,
    Diamond,
    
    Count
};

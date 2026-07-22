#pragma once
#include <array>
#include <cstdint>
#include "BlockFaceDirection.h"

enum class BlockRenderLayer : std::uint8_t
{
    Invisible,
    Opaque,
    Cutout,
    Translucent
};

enum class BlockModelType : std::uint8_t
{
    None,
    Cube, 
    CrossedBillboard
};

struct AtlasTile
{
    std::uint8_t x;
    std::uint8_t y;
};

struct BlockDefinition
{
    bool solid;
    bool occludeNeighbourFaces; //Check this works
    bool replaceable;   
    
    BlockRenderLayer renderLayer;
    BlockModelType modelType;
    std::array<AtlasTile, static_cast<std::size_t>(BlockFaceDirection::Count)> textureFaces {};
};

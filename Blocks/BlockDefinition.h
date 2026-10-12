#pragma once
#include <array>
#include <cstdint>
#include "BlockFaceDirection.h"

enum class BlockTintType : std::uint8_t
{
    None,
    Grass,
    Foliage
};

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
    bool occludeNeighbourFaces; //Hide adjacent cube faces
    bool replaceable; //Replaceable by trees/ player-placed

    BlockRenderLayer renderLayer;
    BlockModelType modelType;
    std::array<AtlasTile, static_cast<std::size_t>(BlockFaceDirection::Count)> textureFaces{};
    
    BlockTintType tintType = BlockTintType::None;
    std::array<bool, static_cast<std::size_t>(BlockFaceDirection::Count)> tintedFaces{};
};

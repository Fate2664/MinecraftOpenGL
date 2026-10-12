#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "BlockDefinition.h"
#include "BlockType.h"

class BlockRegistry
{
public:
    static const BlockDefinition& Get(BlockType type);
    static bool IsAir(BlockType type);
    static bool ShouldRenderFace(BlockType current, BlockType neighbour);
    static glm::vec3 GetFaceNormals(BlockFaceDirection face);
    static glm::vec3 GetTint(BlockType type, BlockFaceDirection face);
    
    //Helper method for blocks with the same texture all around:
    constexpr static std::array<AtlasTile, static_cast<std::size_t>(BlockFaceDirection::Count)> AllFaceTextures(AtlasTile tile)
    {
        return {
            tile,
            tile,
            tile,
            tile,
            tile,
            tile
        };
    }

    inline static const std::array<BlockDefinition, static_cast<std::size_t>(BlockType::Count)>
    blockDefinitions = {
        //Empty
        BlockDefinition{
            false,
            false,
            true,
            BlockRenderLayer::Invisible,
            BlockModelType::None,
            {}
        },

        //Dirt
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{2, 15})
        },

        //Grass - Block
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            {
                AtlasTile{0, 15}, // Front
                AtlasTile{0, 15}, // Back
                AtlasTile{0, 15}, // Left
                AtlasTile{0, 15}, // Right
                AtlasTile{1, 15}, // Top
                AtlasTile{2, 15} // Bottom
            },
            BlockTintType::Grass,
            {false, false, false, false, true, false}
        },

        //OakWood
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            {
                AtlasTile{4, 15}, // Front
                AtlasTile{4, 15}, // Back
                AtlasTile{4, 15}, // Left
                AtlasTile{4, 15}, // Right
                AtlasTile{5, 15}, // Top
                AtlasTile{5, 15} // Bottom
            }
        },

        //Leaf
        BlockDefinition{
            true,
            false,
            false,
            BlockRenderLayer::Cutout,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{1, 14}),
            BlockTintType::Foliage,
            {true, true, true, true, true, true}
        },
        
        //Stone
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{3, 15})
        },
        
        //Coal
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{6, 15})
        },
        
        //Iron
        BlockDefinition{
        true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{7, 15})
        },
        
        //Diamond
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{9, 15})
        },
        
        //Grass - Billboard
        BlockDefinition{
        false,
            false,
            true,
            BlockRenderLayer::Cutout,
            BlockModelType::CrossedBillboard,
            AllFaceTextures(AtlasTile{0, 14}),
            BlockTintType::Grass,
            {true, true, true, true, true, true}
        }

    };
};

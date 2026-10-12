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
                AtlasTile{3, 15}, // Front
                AtlasTile{3, 15}, // Back
                AtlasTile{3, 15}, // Left
                AtlasTile{3, 15}, // Right
                AtlasTile{0, 15}, // Top
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
                AtlasTile{4, 14}, // Front
                AtlasTile{4, 14}, // Back
                AtlasTile{4, 14}, // Left
                AtlasTile{4, 14}, // Right
                AtlasTile{5, 14}, // Top
                AtlasTile{5, 14} // Bottom
            }
        },

        //Leaf
        BlockDefinition{
            true,
            false,
            false,
            BlockRenderLayer::Cutout,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{4, 12})
        },
        
        //Stone
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{1, 15})
        },
        
        //Coal
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{2, 13})
        },
        
        //Iron
        BlockDefinition{
        true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{1, 13})
        },
        
        //Diamond
        BlockDefinition{
            true,
            true,
            false,
            BlockRenderLayer::Opaque,
            BlockModelType::Cube,
            AllFaceTextures(AtlasTile{2, 12})
        },
        
        //Grass - Billboard
        BlockDefinition{
        false,
            false,
            true,
            BlockRenderLayer::Cutout,
            BlockModelType::CrossedBillboard,
            AllFaceTextures(AtlasTile{11, 10}),
            BlockTintType::Grass,
            {true, true, true, true, true, true}
        }

    };
};

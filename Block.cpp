#include "Block.h"

#include "TextureData.h"


Block::Block(glm::vec3 position, BlockType blockType)
{
    this->position = position;
    this->type = blockType;
    
    blockFaces =
    {
        {
            BlockFaceDirection::Front, BlockFaceData{
                AddTransformedVertices(BlockVertexDataRaw::blockVertexData[BlockFaceDirection::Front]),
                blockUV = TextureData::GetFaceUVs(blockType, BlockFaceDirection::Front)
            }
        },
        {
            BlockFaceDirection::Back, BlockFaceData{
                AddTransformedVertices(BlockVertexDataRaw::blockVertexData[BlockFaceDirection::Back]),
                blockUV = TextureData::GetFaceUVs(blockType, BlockFaceDirection::Back)
            }
        },
        {
            BlockFaceDirection::Left, BlockFaceData{
                AddTransformedVertices(BlockVertexDataRaw::blockVertexData[BlockFaceDirection::Left]),
                blockUV = TextureData::GetFaceUVs(blockType, BlockFaceDirection::Left)
            }
        },
        {
            BlockFaceDirection::Right, BlockFaceData{
                AddTransformedVertices(BlockVertexDataRaw::blockVertexData[BlockFaceDirection::Right]),
                blockUV = TextureData::GetFaceUVs(blockType, BlockFaceDirection::Right)
            }
        },
        {
            BlockFaceDirection::Top, BlockFaceData{
                AddTransformedVertices(BlockVertexDataRaw::blockVertexData[BlockFaceDirection::Top]),
                blockUV = TextureData::GetFaceUVs(blockType, BlockFaceDirection::Top)
            }
        },
        {
            BlockFaceDirection::Bottom, BlockFaceData{
                AddTransformedVertices(BlockVertexDataRaw::blockVertexData[BlockFaceDirection::Bottom]),
                blockUV = TextureData::GetFaceUVs(blockType, BlockFaceDirection::Bottom)
            }
        },
    };
}


std::array<glm::vec3, 4> Block::AddTransformedVertices(const std::array<glm::vec3, 4>& vertices) const
{
    std::array<glm::vec3, 4> transformedVertices;
    for (int i = 0; i < vertices.size(); i++)
    {
        transformedVertices[i] = vertices[i] + position;
    }
    return transformedVertices;
}

BlockFaceData Block::GetFace(BlockFaceDirection face)
{
    return blockFaces.at(face);
}


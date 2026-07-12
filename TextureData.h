#pragma once
#include <map>
#include <vector>

#include "BlockVertexDataRaw.h"

static class TextureData
{
public:
    inline static std::map<BlockType, std::map<BlockFaceDirection, glm::vec2>> blockTypeUVCoord = {
        {
            BlockType::Dirt, std::map<BlockFaceDirection, glm::vec2>
            {
                {BlockFaceDirection::Front, glm::vec2(2.0f, 15.0f)},
                {BlockFaceDirection::Back, glm::vec2(2.0f, 15.0f)},
                {BlockFaceDirection::Left, glm::vec2(2.0f, 15.0f)},
                {BlockFaceDirection::Right, glm::vec2(2.0f, 15.0f)},
                {BlockFaceDirection::Top, glm::vec2(2.0f, 15.0f)},
                {BlockFaceDirection::Bottom, glm::vec2(2.0f, 15.0f)}
            }
        },
        {
            BlockType::Grass, std::map<BlockFaceDirection, glm::vec2>
            {
                {BlockFaceDirection::Front, glm::vec2(3.0f, 15.0f)},
                {BlockFaceDirection::Back, glm::vec2(3.0f, 15.0f)},
                {BlockFaceDirection::Left, glm::vec2(3.0f, 15.0f)},
                {BlockFaceDirection::Right, glm::vec2(3.0f, 15.0f)},
                {BlockFaceDirection::Top, glm::vec2(0.0f, 15.0f)},
                {BlockFaceDirection::Bottom, glm::vec2(2.0f, 15.0f)}
            }
        }
    };
    
    static std::array<glm::vec2, 4> GetFaceUVs(BlockType blockType, BlockFaceDirection face)
    {
        constexpr float atlasSize = 16.0f;
    
        glm::vec2 tile = TextureData::blockTypeUVCoord.at(blockType).at(face);
    
        float x0 = tile.x / atlasSize;
        float y0 = tile.y / atlasSize;
        float x1 = (tile.x + 1.0f) / atlasSize;
        float y1 = (tile.y + 1.0f) / atlasSize;
    
        return {
            glm::vec2(x1, y1),
            glm::vec2(x0, y1),
            glm::vec2(x0, y0),
            glm::vec2(x1, y0)
        };
    }

};

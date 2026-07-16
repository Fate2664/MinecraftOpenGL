#pragma once
#include <map>
#include <vector>

#include "BlockDefinition.h"
#include "BlockRegistry.h"
#include "BlockType.h"


class TextureData
{
public:

    static std::array<glm::vec2, 4> GetFaceUVs(BlockType blockType, BlockFaceDirection face)
    {
        constexpr float atlasSize = 16.0f;
        const BlockDefinition& definition = BlockRegistry::Get(blockType);
        const std::size_t faceIndex = static_cast<std::size_t>(face);
        const AtlasTile tile = definition.textureFaces.at(faceIndex);

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

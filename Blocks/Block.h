#pragma once

#include <vector>
#include "BlockVertexDataRaw.h"

class Block
{
public:
    glm::vec3 position;
    BlockType type;
    
    Block() = default;
    Block(glm::vec3 position, BlockType blockType = BlockType::Empty);
    BlockFaceData GetFace(BlockFaceDirection face);
    
private:
    std::map<BlockFaceDirection, BlockFaceData> blockFaces;
    std::array<glm::vec2, 4> blockUV;
    //This function will read the original array without copying it. The final const means that the function will use the
    //block data but not edit it
    std::array<glm::vec3, 4> AddTransformedVertices(const std::array<glm::vec3, 4>& vertices) const;  
};

#include "BlockRegistry.h"

const BlockDefinition& BlockRegistry::Get(BlockType type)
{
    return blockDefinitions.at(static_cast<std::size_t>(type));
}

bool BlockRegistry::IsAir(BlockType type)
{
    return Get(type).renderLayer == BlockRenderLayer::Invisible;
}

bool BlockRegistry::ShouldRenderFace(BlockType current, BlockType neighbour)
{
    if (neighbour == BlockType::Empty) return true;

    const BlockDefinition& neighbourDefinition = BlockRegistry::Get(neighbour);

    if (neighbourDefinition.occludeNeighbourFaces) return false;

    if (current == neighbour) return false;

    return true;
}

glm::vec3 BlockRegistry::GetFaceNormals(BlockFaceDirection face)
{
    //lookup table
   static const std::array<glm::vec3, static_cast<std::size_t>(BlockFaceDirection::Count)> faceNormals =
   {
       glm::vec3{ 0.0f, 0.0f, 1.0f },   //Front
       glm::vec3{ 0.0f, 0.0f, -1.0f },  //Back
       glm::vec3{ -1.0f, 0.0f, 0.0f },  //Left
       glm::vec3{ 1.0f, 0.0f, 0.0f },   //Right
       glm::vec3{ 0.0f, 1.0f, 0.0f },   //Top
       glm::vec3{ 0.0f, -1.0f, 0.0f }  //Bottom
   };
    
    return faceNormals[static_cast<std::size_t>(face)];
}

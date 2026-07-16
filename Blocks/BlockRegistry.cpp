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

#include "Chunk.h"
#include "../Player/Camera.h"
#include <filesystem>
#include <random>
#include <glm/gtx/transform.hpp>


Chunk::Chunk(glm::vec3 position, GeneratedChunkData&& data)
    : position(position),
      chunkBlocks(std::move(data.blocks)),
      heightMap(std::move(data.heightMap))
{
    GenerateTrees();
}

GeneratedChunkData Chunk::GenerateData(glm::vec3 position)
{
    GeneratedChunkData data;

    const int minTerrainHeight = 35;
    const int maxTerrainHeight = Constants::chunkheight - 12;

    for (int x = 0; x < Constants::chunkSize; x++)
    {
        for (int z = 0; z < Constants::chunkSize; z++)
        {
            float worldX = position.x + x;
            float worldZ = position.z + z;

            float noiseValue = 0.0f;
            float frequency = 0.02f;
            float amplitude = 1.0f;
            float maxValue = 0.0f;

            for (int octave = 0; octave < 4; octave++)
            {
                float n = glm::perlin(glm::vec2(worldX * frequency, worldZ * frequency));
                n = (n + 1.0f) * 0.5f;

                noiseValue += n * amplitude;
                maxValue += amplitude;

                amplitude *= 0.5f;
                frequency *= 2.0f;
            }

            noiseValue /= maxValue;

            int terrainHeight = minTerrainHeight + static_cast<int>(noiseValue * (maxTerrainHeight - minTerrainHeight));

            data.heightMap[x][z] = static_cast<float>(terrainHeight);

            for (int y = 0; y < Constants::chunkheight; y++)
            {
                if (y > terrainHeight)
                {
                    data.blocks[x][y][z] = BlockType::Empty;
                }
                else if (y == terrainHeight)
                {
                    data.blocks[x][y][z] = BlockType::Grass;
                }
                else
                {
                    data.blocks[x][y][z] = BlockType::Dirt;
                }
            }
        }
    }
    return data;
}

void Chunk::RebuildMesh(const ChunkNeigbors& neigbors)
{
    chunkVertices.clear();
    chunkIndices.clear();

    GenerateFaces(neigbors);
    BuildChunk();

    hasMesh = true;
}

void Chunk::GenerateFaces(const ChunkNeigbors& neigbors)
{
    for (int x = 0; x < Constants::chunkSize; x++)
    {
        for (int z = 0; z < Constants::chunkSize; z++)
        {
            for (int y = 0; y < Constants::chunkheight; y++)
            {
                if (!IsBlockEmpty(x, y, z))
                {
                    const glm::vec3 blockPosition(x, static_cast<float>(y) - Constants::chunkheight + 0.5f, z);
                    const BlockType blockType = chunkBlocks[x][y][z];

                    //Left faces
                    //Requirements: block to left is empty & is furthest left in chunk
                    if (IsBlockEmpty(x - 1, y, z, neigbors))
                        AddFace(BlockFaceDirection::Left, blockPosition, blockType);

                    //Right faces
                    //Requirements: block to right is empty & is furthest right in chunk
                    if (IsBlockEmpty(x + 1, y, z, neigbors))
                        AddFace(BlockFaceDirection::Right, blockPosition, blockType);

                    //Top faces
                    //Requirements: block above is empty & is furthest up in chunk
                    if (y == Constants::chunkheight - 1 || IsBlockEmpty(x, y + 1, z))
                        AddFace(BlockFaceDirection::Top, blockPosition, blockType);

                    //Bottom faces
                    //Requirements: block below is empty & is furthest down in chunk
                    if (IsBlockEmpty(x, y - 1, z, neigbors))
                        AddFace(BlockFaceDirection::Bottom, blockPosition, blockType);

                    //Front faces
                    //Requirements: block infront is empty & is furthest forward in chunk
                    if (IsBlockEmpty(x, y, z + 1, neigbors))
                        AddFace(BlockFaceDirection::Front, blockPosition, blockType);

                    //Back faces
                    //Requirements: block behind is empty & is furthest back in chunk
                    if (IsBlockEmpty(x, y, z - 1, neigbors))
                        AddFace(BlockFaceDirection::Back, blockPosition, blockType);
                }
            }
        }
    }
}

void Chunk::AddFace(BlockFaceDirection face, const glm::vec3& blockPosition, BlockType blockType)
{
    //static_cast converts chunkVertices.size() into a GLuint
    GLuint startIndex = static_cast<GLuint>(chunkVertices.size());

    const auto& rawVertices = BlockVertexDataRaw::blockVertexData.at(face);
    const auto& uv = TextureData::GetFaceUVs(blockType, face);

    for (int i = 0; i < 4; i++)
    {
        chunkVertices.push_back(Vertex{
            rawVertices[i] + blockPosition,
            uv[i]
        });
    }

    chunkIndices.push_back(startIndex);
    chunkIndices.push_back(startIndex + 1);
    chunkIndices.push_back(startIndex + 2);

    chunkIndices.push_back(startIndex + 2);
    chunkIndices.push_back(startIndex + 3);
    chunkIndices.push_back(startIndex);
}

void Chunk::GenerateTrees()
{
    //One roll for the entire chunk
    //Generate a random number from 1 - 100
    if ((std::rand() % 100) >= 50)
        return;

    const int x = std::rand() % Constants::chunkSize;
    const int z = std::rand() % Constants::chunkSize;

    const int grassY = static_cast<int>(heightMap[x][z]);
    const int treeY = grassY + 1;

    GenerateTreeModel(x, treeY, z);
}

void Chunk::GenerateTreeModel(int treeSpawnX, int treeSpawnY, int treeSpawnZ)
{
    constexpr int minTreeHeight = 6;
    constexpr int maxTreeHeight = 8;

    const int availableHeight = Constants::chunkheight - treeSpawnY;
    const int largestAllowedTree = std::min(maxTreeHeight, availableHeight);
    //trees cannot be heigher than max chunk height 

    //Randomly choose height between min and max while still being inside max chunk height
    std::random_device rd;
    std::mt19937 randomEngine(rd());
    std::uniform_int_distribution<int> distr(minTreeHeight, largestAllowedTree);

    //Build trunk
    int trunkHeight = distr(randomEngine);
    for (int offset = 0; offset < (trunkHeight - 1); offset++) //leave one block above trunk for grass
    {
        const int y = treeSpawnY + offset;
        chunkBlocks[treeSpawnX][y][treeSpawnZ] = BlockType::OakWood;
    }
    chunkBlocks[treeSpawnX][trunkHeight - 1 + treeSpawnY][treeSpawnZ] = BlockType::Leaf;
    //Build leaves
    constexpr int baseLeafRadius = 2;
    const int baseLeafY = treeSpawnY + trunkHeight - 4;
    //Base layer
    for (int xOffset = -baseLeafRadius; xOffset <= baseLeafRadius; xOffset++)
    {
        for (int zOffset = -baseLeafRadius; zOffset <= baseLeafRadius; zOffset++)
        {
            for (int yOffset = 0; yOffset < 2; yOffset++)
            {
                int leafX = treeSpawnX + xOffset;
                int leafZ = treeSpawnZ + zOffset;
                int leafY = baseLeafY + yOffset;

                //Check if x and z offsets are outside of bounds of chunk
                if (leafX < 0 || leafX >= Constants::chunkSize
                    || leafZ < 0 || leafZ >= Constants::chunkSize
                    || leafY < 0 || leafY >= Constants::chunkheight)
                {
                    continue;
                }

                //Avoid replacing trunk
                if (chunkBlocks[leafX][leafY][leafZ] == BlockType::Empty)
                {
                    chunkBlocks[leafX][leafY][leafZ] = BlockType::Leaf; //Place leaf
                }
            }
        }
    }
    //Upper layer
    constexpr int upperLeafRadius = 1;
    const int upperLeafY = treeSpawnY + trunkHeight - 2;
    //Base layer
    for (int xOffset = -upperLeafRadius; xOffset <= upperLeafRadius; xOffset++)
    {
        for (int zOffset = -upperLeafRadius; zOffset <= upperLeafRadius; zOffset++)
        {
            int leafX = treeSpawnX + xOffset;
            int leafZ = treeSpawnZ + zOffset;

            //Check if x and z offsets are outside of bounds of chunk
            if (leafX < 0 || leafX >= Constants::chunkSize
                || leafZ < 0 || leafZ >= Constants::chunkSize
                || upperLeafY < 0 || upperLeafY >= Constants::chunkheight)
            {
                continue;
            }

            //Avoid replacing trunk
            if (chunkBlocks[leafX][upperLeafY][leafZ] == BlockType::Empty)
            {
                chunkBlocks[leafX][upperLeafY][leafZ] = BlockType::Leaf; //Place leaf
            }
        }
    }
}

#pragma region Rendering

void Chunk::BuildChunk()
{
    //Delete already made VBO and EBO for remeshing
    if (chunkVBO)
    {
        chunkVBO->Delete();
        chunkVBO.reset();
    }

    if (chunkEBO)
    {
        chunkEBO->Delete();
        chunkEBO.reset();
    }

    chunkVAO.Bind();

    chunkVBO = std::make_unique<VertexBufferObject>(chunkVertices);

    chunkVAO.LinkAttrib(*chunkVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    chunkVAO.LinkAttrib(*chunkVBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));

    chunkEBO = std::make_unique<ElementBufferObject>(chunkIndices);

    chunkVAO.Unbind();
    chunkVBO->Unbind();
    chunkEBO->Unbind();
}

void Chunk::Render(Shader& shaderProgram, Camera& camera)
{
    if (!hasMesh) return;

    shaderProgram.Activate();

    glm::mat4 chunk = glm::translate(glm::mat4(1.0f), position);
    shaderProgram.SetMat4("chunk", chunk);

    // Take care of the camera Matrix
    camera.Matrix(shaderProgram, "camMatrix");

    chunkVAO.Bind();

    // Draw the actual mesh
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(chunkIndices.size()), GL_UNSIGNED_INT, 0);

    chunkVAO.Unbind();
}

void Chunk::Delete()
{
    if (chunkVBO)
    {
        chunkVBO->Delete();
    }
    if (chunkEBO)
    {
        chunkEBO->Delete();
    }
    chunkVAO.Delete();
}

#pragma endregion

#pragma region State Check Methods

bool Chunk::IsBlockEmpty(int x, int y, int z) const
{
    if (x < 0 || x >= Constants::chunkSize) return true;
    if (y < 0 || y >= Constants::chunkheight) return true;
    if (z < 0 || z >= Constants::chunkSize) return true;

    return chunkBlocks[x][y][z] == BlockType::Empty;
}

bool Chunk::IsBlockEmpty(int x, int y, int z, const ChunkNeigbors& neigbors) const
{
    if (y < 0 || y >= Constants::chunkheight) return true;
    if (x < 0)
    {
        //returns true if left neighbour is null or if left neighbour block is empty at Constants::chunkSize - 1, y, z coords
        return neigbors.left == nullptr || neigbors.left->IsBlockEmpty(Constants::chunkSize - 1, y, z);
    }
    if (x >= Constants::chunkSize)
    {
        return neigbors.right == nullptr || neigbors.right->IsBlockEmpty(0, y, z);
    }
    if (z < 0)
    {
        return neigbors.back == nullptr || neigbors.back->IsBlockEmpty(x, y, Constants::chunkSize - 1);
    }
    if (z >= Constants::chunkSize)
    {
        return neigbors.front == nullptr || neigbors.front->IsBlockEmpty(x, y, 0);
    }

    return IsBlockEmpty(x, y, z);
}
#pragma endregion

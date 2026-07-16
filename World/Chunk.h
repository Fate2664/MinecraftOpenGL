#pragma once
#include <array>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../Blocks/BlockRawGeometry.h"
#include "../Constants.h"
#include "../OpenGL_Objects/ElementBufferObject.h"
#include "../Blocks/Texture.h"
#include "../OpenGL_Objects/VertexArrayObject.h"
#include "../Blocks/TextureData.h"
#include "../Shaders/ShaderClass.h"
#include "../Player/Camera.h"
#include "../Blocks/BlockType.h"
#include <glm/gtc/noise.hpp>

class Camera;
class Chunk;

struct ChunkCoord
{
    int x, z;
    
    bool operator==(const ChunkCoord& other) const
    {
        return x == other.x && z == other.z;
    }
};

struct ChunkNeigbors
{
    const Chunk* left = nullptr;
    const Chunk* right = nullptr;
    const Chunk* front = nullptr;
    const Chunk* back = nullptr;
};

struct ChunkCoordHash
{
    std::size_t operator()(const ChunkCoord& coord) const
    {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.z) << 1);
    }
};

using ChunkBlockData = std::array<std::array<std::array<BlockType, Constants::chunkSize>, Constants::chunkheight>, Constants::chunkSize>;
using ChunkHeightMap = std::array<std::array<float, Constants::chunkSize>, Constants::chunkSize>;

struct GeneratedChunkData
{
    ChunkBlockData blocks{};
    ChunkHeightMap heightMap{};
};

class Chunk
{
public:
    glm::vec3 position;
    ChunkBlockData chunkBlocks;
    ChunkHeightMap heightMap;
    
    Chunk(glm::vec3 position, GeneratedChunkData&& data);
    
    static GeneratedChunkData GenerateData(glm::vec3 position);
    
    void RebuildMesh(const ChunkNeigbors& neigbors);    //Used to rebuild faces when new chunk is loaded
    void Render(Shader& shaderProgram, Camera& camera);     //draw the chunk
    void Delete();

    bool HasMesh() const
    {
        return hasMesh;
    }
private:
    std::vector<Vertex> chunkVertices;
    std::vector<GLuint> chunkIndices;
    
    VertexArrayObject chunkVAO;
    
    std::unique_ptr<VertexBufferObject> chunkVBO;    
    std::unique_ptr<ElementBufferObject> chunkEBO;
    
    void GenerateFaces(const ChunkNeigbors& neigbors);
    void AddFace(BlockFaceDirection face, const glm::vec3& blockPosition, BlockType blockType);    //we use a const reference to give the function access to an 
    //existing BlockFaceData but it does not copy it and it cannot modify it. This is helpful since BlockFaceData uses arrays
    void BuildChunk();  //take data and process it for rendering
    void GenerateTrees();
    void GenerateTreeModel(int treeSpawnX, int treeSpawnY, int treeSpawnZ);

    bool hasMesh = false;
    bool IsBlockEmpty(int x, int y, int z) const;
    BlockType GetBlockType(int x, int y, int z, const ChunkNeigbors& neigbors) const;
};

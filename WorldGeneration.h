#pragma once

#include <deque>
#include <future>
#include <unordered_map>
#include <unordered_set>

#include "Camera.h"
#include "Chunk.h"
#include "ShaderClass.h"

class WorldGeneration
{
public:
    explicit WorldGeneration(int loadDistance);
    void Update(const Camera& camera);
    void Render(Shader& shaderProgram, Camera& camera);
    void Delete();
private:
    struct ChunkGenerationResult
    {
        ChunkCoord coord;
        glm::vec3 worldPosition;
        GeneratedChunkData data;
    };

    struct ChunkGenerationJob
    {
        ChunkCoord coord;
        std::future<ChunkGenerationResult> future;
    };
    
    int loadDistance = 2;
    std::unique_ptr<Texture> blockTexture;
    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> chunks;
    std::deque<ChunkCoord> generationQueue;
    std::unordered_set<ChunkCoord, ChunkCoordHash> generationScheduled;
    std::vector<ChunkGenerationJob> generationJobs;
    std::deque<ChunkCoord> meshQueue;
    std::unordered_set<ChunkCoord, ChunkCoordHash> meshScheduled;
    
    //Job budgets
    std::size_t maxConcurrentGenerationJobs = 2;
    std::size_t maxChunkIntegrationsPerFrame = 2;
    std::size_t maxMeshBuildsPerFrame = 2;
    
    const Chunk* FindChunk(const ChunkCoord& chunkCoord) const;
    ChunkNeigbors GetNeigbors(const ChunkCoord& chunkCoord) const;
    void GenerateTrees();
    
    void QueueMissingChunks(int cameraChunkX, int cameraChunkZ);
    void StartGenerationJobs();
    void CollectFinishedGenerationJobs();
    void MarkChunkDirty(const ChunkCoord& coord);
    void MarkChunkAndNeighboursDirty(const ChunkCoord& coord);
    void ProcessMeshQueue();
    
};

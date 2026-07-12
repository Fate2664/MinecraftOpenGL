#include "WorldGeneration.h"

#include <unordered_set>

WorldGeneration::WorldGeneration(int loadDistance) : loadDistance(loadDistance)
{
    blockTexture = std::make_unique<Texture>("MinecraftBlockTextures.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
}

void WorldGeneration::Update(const Camera& camera)
{
    //Round down the camera coordinates relative to chunk size
    int cameraChunkX = (int)(std::floor(camera.Position.x / Constants::chunkSize));
    int cameraChunkZ = (int)(std::floor(camera.Position.z / Constants::chunkSize));

    QueueMissingChunks(cameraChunkX, cameraChunkZ);
    CollectFinishedGenerationJobs();
    StartGenerationJobs();
    ProcessMeshQueue();
}

void WorldGeneration::Render(Shader& shaderProgram, Camera& camera)
{
    shaderProgram.Activate();
    blockTexture->texUnit(shaderProgram, "diffuse0", 0);
    blockTexture->Bind();


    //Get the chunk object and render it
    for (auto& chunk : chunks)
    {
        chunk.second->Render(shaderProgram, camera);
    }
    blockTexture->Unbind();
}

void WorldGeneration::Delete()
{
    for (ChunkGenerationJob& job : generationJobs)
    {
        if (job.future.valid())
            job.future.wait();
    }

    generationJobs.clear();
    generationQueue.clear();
    generationScheduled.clear();
    
    meshQueue.clear();
    meshScheduled.clear();
    
    for (auto& chunk : chunks)
        chunk.second->Delete();

    chunks.clear();

    if (blockTexture)
        blockTexture->Delete();
}

const Chunk* WorldGeneration::FindChunk(const ChunkCoord& chunkCoord) const
{
    auto chunk = chunks.find(chunkCoord);
    if (chunk == chunks.end()) return nullptr;

    return chunk->second.get();
}

ChunkNeigbors WorldGeneration::GetNeigbors(const ChunkCoord& chunkCoord) const
{
    return ChunkNeigbors
    {
        FindChunk({chunkCoord.x - 1, chunkCoord.z}), //Left neighbour
        FindChunk({chunkCoord.x + 1, chunkCoord.z}), //Right neigbour
        FindChunk({chunkCoord.x, chunkCoord.z + 1}), //Front neighour
        FindChunk({chunkCoord.x, chunkCoord.z - 1}) //Back neighbour
    };
}

void WorldGeneration::QueueMissingChunks(int cameraChunkX, int cameraChunkZ)
{
    //loop until outside of loadDistance
    for (int x = cameraChunkX - loadDistance; x <= cameraChunkX + loadDistance; x++)
    {
        for (int z = cameraChunkZ - loadDistance; z <= cameraChunkZ + loadDistance; z++)
        {
            const ChunkCoord coord{x, z};
            const bool alreadyLoaded = chunks.find(coord) != chunks.end();
            //check if coordinates already hold a chunk or not
            const bool alreadyScheduled = generationScheduled.find(coord) != generationScheduled.end();

            if (!alreadyScheduled && !alreadyLoaded)
            //If it does not already hold a chunk and it already has not been scheduled
            {
                generationQueue.push_back(coord); //Add to the queue
                generationScheduled.insert(coord); //And schedule it
            }
        }
    }
}

void WorldGeneration::StartGenerationJobs()
{
    while (!generationQueue.empty() && generationJobs.size() < maxConcurrentGenerationJobs)
    {
        const ChunkCoord coord = generationQueue.front();
        generationQueue.pop_front();
        const glm::vec3 worldPosition(coord.x * Constants::chunkSize, 0.0f, coord.z * Constants::chunkSize);

        ChunkGenerationJob job;
        job.coord = coord;

        //We must call std::launch::async otherwise std is allowed to defer the work until get() is called which could put
        //the generation back on the main thread
        job.future = std::async(std::launch::async, [coord, worldPosition]()
        {
            ChunkGenerationResult result;
            result.coord = coord;
            result.worldPosition = worldPosition;
            result.data = Chunk::GenerateData(worldPosition);
            return result;
        });
        generationJobs.push_back(std::move(job));
    }
}

void WorldGeneration::CollectFinishedGenerationJobs()
{
    std::size_t integratedThisFrame = 0;

    for (auto job = generationJobs.begin(); job != generationJobs.end() && integratedThisFrame <
         maxChunkIntegrationsPerFrame;)
    {
        const bool ready = job->future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;

        if (!ready)
        {
            ++job;
            continue;
        }

        ChunkGenerationResult result = job->future.get();
        const ChunkCoord coord = result.coord;

        chunks.emplace(coord, std::make_unique<Chunk>(result.worldPosition, std::move(result.data)));
        generationScheduled.erase(coord);
        MarkChunkAndNeighboursDirty(coord);

        job = generationJobs.erase(job);
        integratedThisFrame++;
    }
}

void WorldGeneration::MarkChunkDirty(const ChunkCoord& coord)
{
    if (chunks.find(coord) == chunks.end()) return;

    const bool inserted = meshScheduled.insert(coord).second;
    if (inserted)
        meshQueue.push_back(coord);
}

void WorldGeneration::MarkChunkAndNeighboursDirty(const ChunkCoord& coord)
{
    MarkChunkDirty(coord);

    const ChunkCoord adjacent[] = {
        {coord.x - 1, coord.z},
        {coord.x + 1, coord.z},
        {coord.x, coord.z - 1},
        {coord.x, coord.z + 1}
    };

    for (const ChunkCoord neighbour : adjacent) //Mark neigbour chunks as dirty 
        MarkChunkDirty(neighbour);
}

void WorldGeneration::ProcessMeshQueue()
{
    std::size_t meshesBuiltThisFrame = 0;

    while (!meshQueue.empty() && meshesBuiltThisFrame < maxMeshBuildsPerFrame)
    {
        const ChunkCoord coord = meshQueue.front();
        meshQueue.pop_front();
        meshScheduled.erase(coord);

        auto chunk = chunks.find(coord);

        if (chunk == chunks.end())
            continue;

        chunk->second->RebuildMesh(GetNeigbors(coord)); //Rebuild every dirty chunk
        meshesBuiltThisFrame++;
    }
}

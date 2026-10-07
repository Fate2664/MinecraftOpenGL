#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "World/NoiseData.h"

namespace Constants
{
    //Rendering
    constexpr int windowWidth = 1920;
    constexpr int windowHeight = 1080;

    //Chunks
    constexpr int chunkSize = 16;
    constexpr int chunkheight = 128;
    constexpr int worldMinY = -64;

    /* NOTE 
     * Minecraft average terrain y height is 60 - 70
     * Minecraft sea level is y = 63
     * Mincraft theoretical build limit is y = 320
     */

    //Noise Data
    const NoiseData ContinentalnessNoise(0.002f, 2.0f, 0.5f, 4, 1001);
    const NoiseData ErosionNoise(0.008f, 2.0f, 0.5f, 3, 2002);
    const NoiseData PeaksNoise(0.01f, 2.0f, 0.5f, 4, 3003);
}

#endif

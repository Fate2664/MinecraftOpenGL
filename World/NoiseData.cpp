#include "NoiseData.h"

NoiseData::NoiseData(float frequency, float lacunarity, float persistence, int octaves, int seed)
{
    this->frequency = frequency;
    this->lacunarity = lacunarity;
    this->persistence = persistence;
    this->octaves = octaves;
    this->seed = seed;
}

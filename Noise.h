#pragma once
#include <algorithm>

class Noise
{
public:
    int outputWidth = 256;
    int outputHeight = 256;
    float *noiseSeed = nullptr;
    float *perlinNoise = nullptr;
    
    Noise(int outputWidth, int outputHeight, float *noiseSeed, float *perlinNoise);
    ~Noise();
    
    Noise(const Noise&) = delete;
    Noise& operator=(const Noise&) = delete;
    
    void PerlinNoise(int width, int height, float *seed, int octaves, float bias, float *output);
    void ChangeSeed();
};

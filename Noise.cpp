#include "Noise.h"

Noise::Noise(int outputWidth, int outputHeight, float* noiseSeed, float* perlinNoise)
{
    this->outputWidth = outputWidth;
    this->outputHeight = outputHeight;

    this->noiseSeed = new float[outputWidth * outputHeight];
    this->perlinNoise = new float[outputWidth * outputHeight];
    for (int i = 0; i < outputWidth * outputHeight; i++) this->noiseSeed[i] = (float)rand() / (float)RAND_MAX;
    
}

Noise::~Noise()
{
    delete[] noiseSeed;
    delete[] perlinNoise;
}

void Noise::PerlinNoise(int width, int height, float* seed, int octaves, float bias, float* output)
{
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
        {
            float noise = 0.0f;
            float scale = 1.0f;
            float scaleAcc = 0.0f;
                
            for (int o = 0; o < octaves; o++)
            {
                int pitch = width >> o;
                int sampleX1 = (x / pitch) * pitch;
                int sampleY1 = (y / pitch) * pitch;
                    
                int sampleX2 = (sampleX1 + pitch) % width;
                int sampleY2 = (sampleY1 + pitch) % height;
                                        
                float blendX = (float)(x - sampleX1) / (float)pitch;
                float blendY = (float)(y - sampleY1) / (float)pitch;

                float sampleT = (1.0f - blendX) * seed[sampleY1 * width + sampleX1] + blendX * seed[sampleY1 * width + sampleX2];
                float sampleB = (1.0f - blendX) * seed[sampleY2 * width + sampleX1] + blendX * seed[sampleY2 * width + sampleX2];

                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scaleAcc += scale;
                scale = scale / bias;
            }
                
            output[y * width + x] = noise / scaleAcc;
        }
}

void Noise::ChangeSeed()
{
    for (int i = 0; i < outputWidth * outputHeight; i++)
    {
        noiseSeed[i] = (float)rand() / (float)RAND_MAX;
    }
}



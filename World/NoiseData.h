#pragma once

class NoiseData
{
public:
    float frequency = 0.0f;
    float lacunarity = 1.0f;
    
    //Persistance is how quickly the octave's amplitude scales.
    //e.g persistance of 0.5: 
    //First octave amplitude : 1.0
    //Second octave : 0.5
    //Third octave : 0.25
    float persistence = 1.0f;   
    int octaves = 1;
    int seed = 12345;
    
    NoiseData(float frequency, float lacunarity, float persistence, int octaves, int seed);
};

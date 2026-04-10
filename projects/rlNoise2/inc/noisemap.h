#ifndef NOISEMAP_H
#define NOISEMAP_H

#include "FastNoise/FastNoise.h"

struct NoiseMap
{
    NoiseMap();

    struct : public FastNoise { FastNoise lookup; } noise;

};

#endif // NOISEMAP_H

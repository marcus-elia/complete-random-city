// This is based on code from https://www.youtube.com/watch?v=6-0UaeJBumA,
// a tutorial by javidx9

#ifndef RANDOM_3D_CITY_PERLINNOISEGENERATOR_H
#define RANDOM_3D_CITY_PERLINNOISEGENERATOR_H

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>

class PerlinNoiseGenerator
{
private:
    int width;
    int height;

    std::vector<std::vector<double>> noiseSeed;
    std::vector<std::vector<double>> perlinNoise;

    double bias; // how sharp the noise is

public:
    PerlinNoiseGenerator();
    PerlinNoiseGenerator(int inputWidth, int inputHeight, double inputBias);

    void fillNoiseSeed();

    std::vector<double> calculatePerlinNoise1D(int count, std::vector<double> seed, int numOctaves);

    std::vector<std::vector<double>> calculatePerlinNoise2D(int width, int height,
            std::vector<std::vector<double>> seed, int numOctaves);

    // Linearly scales the perlin noise so that the smallest in the 2d array becomes minvalue
    // and the highest becomes maxvalue
    void scaleNoise(double minValue, double maxValue);

    std::vector<std::vector<double>> getPerlinNoise();
};

#endif //RANDOM_3D_CITY_PERLINNOISEGENERATOR_H

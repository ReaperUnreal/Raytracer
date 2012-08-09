#ifndef NOISE_H
#define NOISE_H

//no need for a class here really as all the functions would be static anyway
float Noise(const Vector &p);
float SmoothNoise(const Vector &p);
float Perlin(const Vector &p);

#endif

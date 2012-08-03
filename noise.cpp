#include "main.h"

//just standard interpolation from the glsl spec
float mix(float x, float y, float a)
{
   return (x * (1.0f - a)) + (y * a);
}

//just a basic noise implementation
float Noise(const Vector &p)
{
   float x = p.xv();
   float y = p.yv();
   float z = p.zv();

   float t = x + (y * 57.0f) + (z * 27.0f);

   return sin(cos(t) * t);
}

//smoother noise, basically some interpolation
float SmoothNoise(const Vector &p)
{
   Vector ints = p.Floor();
   Vector fractional = p - ints;

   //straight from Pouet.net, I'm not going to bother tearing this apart
   // here's the reference: http://pouet.net/topic.php?which=7931&page=2#cite-4548
   return mix(mix(mix(Noise(ints),
                      Noise(ints + Vector(1, 0, 0)),
                      fractional.xv()),
                  mix(Noise(ints + Vector(0, 1, 0)),
                      Noise(ints + Vector(1, 1, 0)),
                      fractional.xv()),
                  fractional.yv()),
              mix(mix(Noise(ints + Vector(0, 0, 1)),
                      Noise(ints + Vector(1, 0, 1)),
                      fractional.xv()),
                  mix(Noise(ints + Vector(0, 1, 1)),
                      Noise(ints + 1.0f),
                      fractional.xv()),
                  fractional.yv()),
              fractional.zv()) * 0.5f + 0.5f;
   return 0.0f;
}

//the ever famous perlin noise
float Perlin(const Vector &p)
{
   Vector p1 = p * 0.6125f; //octave 1
   Vector p2 = p * 0.125f; //octave 2
   Vector p3 = p * 0.25f; //octave 3
   return SmoothNoise(p1) * 0.5f +
          SmoothNoise(p2) * 0.25f +
          SmoothNoise(p3) * 0.125f;
}

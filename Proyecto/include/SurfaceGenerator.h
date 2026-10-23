#ifndef SURFACE_GENERATOR_H
#define SURFACE_GENERATOR_H

#include "OBJLoader.h"

class SurfaceGenerator
{
public:
    static OBJData generate(int resolution = 100);
};

#endif
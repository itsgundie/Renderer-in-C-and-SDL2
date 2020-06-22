#ifndef LIGHTNING_H
#define LIGHTNING_H

#include "vector.h"

typedef struct {
    vec3d_t direction;
} light_t;

extern light_t light;

uint32_t    lumos(uint32_t color, float luminosity);

#endif

#ifndef TEXTURE_H
#define TEXTURE_H

#include "render.h"

typedef struct
{
    float u;
    float v;
}       tex2_t;

extern uint32_t texture_width;
extern uint32_t texture_height;

extern uint32_t* mesh_texture; 

extern const uint8_t REDBRICK_TEXTURE[];

#endif

#include "render.h"

vec3d_t vec3_rotate_x(vec3d_t vector, float angle)
{
    vec3d_t rotated = { .x = vector.x,
                        .y = vector.y * cos(angle) - vector.z * sin(angle),
                        .z = vector.y * sin(angle) + vector.z * cos(angle)
                      };
    return(rotated);
}


vec3d_t vec3_rotate_y(vec3d_t vector, float angle)
{
    vec3d_t rotated = { .x = vector.x * cos(angle) - vector.z * sin(angle),
                        .y = vector.y ,
                        .z = vector.x * sin(angle) + vector.z * cos(angle)
                      };
    return(rotated);
}


vec3d_t vec3_rotate_z(vec3d_t vector, float angle)
{
    vec3d_t rotated = { .x = vector.x * cos(angle) - vector.y * sin(angle),
                        .y = vector.x * sin(angle) + vector.y * cos(angle),
                        .z = vector.z
                      };
    return(rotated);
}
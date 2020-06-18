#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <math.h>

typedef struct          s_matrix4x4
{
    float               m[4][4];
}                       mtx4_t;

mtx4_t      mtx4_identity(void);
vec4d_t     mtx4_mult_vec4d(mtx4_t mat, vec4d_t vec);
mtx4_t      mtx4_mult_mtx4(mtx4_t first, mtx4_t second);
mtx4_t      mtx4_scaling(float sx, float sy, float sz);
mtx4_t      mtx4_translation(float tx, float ty, float tz);
mtx4_t      mtx4_rotation_x(float angle);
mtx4_t      mtx4_rotation_y(float angle);
mtx4_t      mtx4_rotation_z(float angle);
mtx4_t      mtx4_perspective(float fov, float aspect, float z_near, float z_far);
vec4d_t		mtx4_mult_vec4d_projection(mtx4_t proj_matrx, vec4d_t vec);
#endif
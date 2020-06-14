
#include "matrix.h"

mtx4_t      mtx4_identity(void)
{
    mtx4_t  id = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }
    };
    return(id);
}


mtx4_t      mtx4_scaling(float sx, float sy, float sz)
{
    mtx4_t  scale = mtx4_identity();
    scale.m[0][0] = sx;
    scale.m[1][1] = sy;
    scale.m[2][2] = sz;
    return(scale);
}

vec4d_t     mtx4_mult_vec4d(mtx4_t mat, vec4d_t vec)
{
    vec4d_t     result;
    result.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3] * vec.w;
    result.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3] * vec.w;
    result.z = mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3] * vec.w;
    result.w = mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z + mat.m[3][3] * vec.w;

    return(result);
}


mtx4_t      mtx4_translation(float tx, float ty, float tz)
{
    mtx4_t  result = mtx4_identity();

    result.m[0][3] = tx;
    result.m[1][3] = ty;
    result.m[2][3] = tz;
    return(result);
}

mtx4_t      mtx4_rotation_x(float angle)
{
    mtx4_t  result = mtx4_identity();
    result.m[1][1] = cos(angle);
    result.m[1][2] = -sin(angle);
    result.m[2][1] = sin(angle);
    result.m[2][2] = cos(angle);
    return(result);
}

mtx4_t      mtx4_rotation_y(float angle)
{
    mtx4_t  result = mtx4_identity();
    result.m[0][0] = cos(angle);
    result.m[0][2] = sin(angle);
    result.m[2][0] = -sin(angle);
    result.m[2][2] = cos(angle);
    return(result);
}
mtx4_t      mtx4_rotation_z(float angle)
{
    mtx4_t  result = mtx4_identity();
    result.m[0][0] = cos(angle);
    result.m[0][1] = -sin(angle);
    result.m[1][0] = sin(angle);
    result.m[1][1] = cos(angle);
    return(result);
}
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef struct	s_face_mesh
{
		int a;
		int b;
		int c;
}				face_mesh_t;

typedef struct	s_triangle
{
		vec2d_t points[3];
}               triangle_t;

#endif
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "render.h"
#include "vector.h"
#include "texture.h"

typedef struct	s_face_mesh
{
		int 	a;
		int 	b;
		int 	c;
		tex2_t	a_uv;
		tex2_t	b_uv;
		tex2_t	c_uv;
		uint32_t color;
}				face_mesh_t;

typedef struct	s_triangle
{
		vec2d_t points[3];
		tex2_t	tex_coords[3];
		uint32_t color;
		float	avg_depth;
}               triangle_t;

void    draw_triangle_fill_bottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void    draw_triangle_fill_top(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void	draw_triangle_filled(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void	draw_triangle_textured(
			int x0, int y0, float u0, float v0,
			int x1, int y1, float u1, float v1,
			int x2, int y2, float u2, float v2,
			uint32_t *texture);

#endif
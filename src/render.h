
#ifndef RENDER_H
#define RENDER_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "SDL.h"
#include "array.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define NUMBER_OF_POINTS (10 * 10 * 10)
#define NUM_CUBE_VERTS 8
#define NUM_CUBE_FACES (6 * 2)



typedef struct s_vec2d
{
		float x;
		float y;
}               vec2d_t;

typedef struct s_vec3d
{
				float x;
				float y;
				float z;
}               vec3d_t;

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

typedef struct		s_mesh
{
	vec3d_t*		vertices;
	face_mesh_t*	faces;
	vec3d_t			rotation;
}					mesh_t;

extern unsigned int    win_width;
extern unsigned int    win_height;
extern bool            game_is_on;
extern SDL_Window *    game_win;
extern SDL_Renderer*   game_render;
extern uint32_t*       color_buffer;
extern SDL_Texture* color_buffer_texture;
extern vec3d_t cube[NUMBER_OF_POINTS];
extern vec2d_t cube_projected[NUMBER_OF_POINTS];
extern vec3d_t camera_plane;
extern vec3d_t cube_rotation;
extern int prev_frame_time;
extern vec3d_t cube_vertices[NUM_CUBE_VERTS];
extern face_mesh_t cube_faces[NUM_CUBE_FACES];
extern triangle_t* triangles_to_render;
extern mesh_t mesh;

void    set_me_free(void);

void    clear_color_buffer(uint32_t color);

bool    init_window(void);

void    input_catch(void);

void    update(void);

void    render_color_buffer(void);

void    draw_grid(void);

void    draw_rect(int x, int y, int width, int height, uint32_t color);

void    render(void);

void    setup(void);

int     main(void);


vec3d_t vec3_rotate_x(vec3d_t vector, float angle);


vec3d_t vec3_rotate_y(vec3d_t vector, float angle);


vec3d_t vec3_rotate_z(vec3d_t vector, float angle);



#endif
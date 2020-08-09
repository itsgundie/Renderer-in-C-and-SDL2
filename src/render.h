
#ifndef RENDER_H
#define RENDER_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "../SDL/inc/SDL.h"
#include "array.h"
#include "vector.h"
#include "triangle.h"
#include "mesh.h"
#include "texture.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define NUMBER_OF_POINTS (10 * 10 * 10)

#define OBJ_1 "./assets/teapot.obj"
#define OBJ_2 "./assets/Axe.obj"
#define OBJ_3 "./assets/Hammer.obj"
#define OBJ_4 "./assets/PPSH.obj"
#define OBJ_5 "./assets/crowbar.obj"
#define OBJ_6 "./assets/plain_doll.obj"
#define OBJ_7 "./assets/lady maria.obj"
#define OBJ_8 "./assets/gehrman.obj"
#define OBJ_9 "./assets/f22.obj"
#define OBJ_0 "./assets/cube.obj"

#define	MODEL_OBJ OBJ_9
#define	LINE_SIZE 1024

#define PIE 3.14159265358979323846

#define FOV     640.0f

#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0xFFFFFFFF
#define COLOR_GREEN     0x00FF0000
#define COLOR_BLUE      0x0000FF00
#define COLOR_CYAN      0x00FFFF00
#define COLOR_RED       0xFF000000

enum    cull_method
{
        CULL_NONE,
        CULL_BACKFACE
}       cull_method_e;

enum    render_method
{
        RENDER_WIRE,
        RENDER_WIRE_VERTEX,
        RENDER_FILL_TRIANGLE,
        RENDER_FILL_TRIANGLE_WIRE,
        RENDER_TEXTURED,
        RENDER_TEXTURED_WIRED
}       render_method_e;

extern unsigned int    win_width;
extern unsigned int    win_height;
extern bool            game_is_on;
extern SDL_Window *    game_win;
extern SDL_Renderer*   game_render;
extern uint32_t*       color_buffer;
extern float*          z_buf;
extern SDL_Texture* color_buffer_texture;
extern vec3d_t cube[NUMBER_OF_POINTS];
extern vec2d_t cube_projected[NUMBER_OF_POINTS];
extern vec3d_t camera_plane;
extern vec3d_t cube_rotation;
extern int prev_frame_time;
extern triangle_t* triangles_to_render;


void    set_me_free(void);

void    clear_color_buffer(uint32_t color);

void    clear_z_buf();

bool    init_window(void);

void    input_catch(void);

void    update(void);

void    render_color_buffer(void);

void    draw_grid(void);

void    draw_rect(int x, int y, int width, int height, uint32_t color);

void	draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void    render(void);

void    setup(void);



int     main(void);

vec2d_t     project3to2d(vec3d_t source);


#endif

#ifndef RENDER_H
#define RENDER_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "SDL.h"


#define NUMBER_OF_POINTS (10 * 10 * 10)


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

#endif
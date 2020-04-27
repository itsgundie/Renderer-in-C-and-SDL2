
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "SDL.h"

unsigned int    win_width = 320;
unsigned int    win_height = 240;
bool            game_is_on = false;;
SDL_Window *    game_win = NULL;
SDL_Renderer*   game_render = NULL;
uint32_t*       color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

void    set_me_free()
{
    free(color_buffer);
    SDL_DestroyRenderer(game_render);
    SDL_DestroyWindow(game_win);
    SDL_Quit();
}

void    clear_color_buffer(uint32_t color)
{
    for (int q = 0; q < (win_width * win_height); q++)
        color_buffer[q] = color;
}


bool    init_window(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL Initialization Error\n");
        return (false);
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0,&display_mode);
    win_width = display_mode.w;
    win_height = display_mode.h;
    game_win = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             win_width, win_height, SDL_WINDOW_BORDERLESS);
    if(!game_win)
    {
        fprintf(stderr, "Error Creating Window\n");
        return(false);
    }
    game_render = SDL_CreateRenderer(game_win, -1, 0);
    if(!game_render)
    {
        fprintf(stderr, "SDL Render Initialization Error\n");
        return(false);
    }
    return (true);
}

void    input_catch(void)
{
    SDL_Event   eve;
    SDL_PollEvent(&eve);

    switch (eve.type)
    {
        case SDL_QUIT:
            game_is_on = false;
            break;
        case SDL_KEYDOWN:
            if(eve.key.keysym.sym == SDLK_ESCAPE)
                game_is_on = false;
            break;
    }
}

void    update(void)
{
    //
}

void    render_color_buffer(void)
{
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(win_width * sizeof(uint32_t)));
    SDL_RenderCopy(game_render, color_buffer_texture, NULL, NULL);
}

void    draw_grid(void)
{
   for(int y = 0; y <= win_height; y += 10)
    for(int x = 0; x <= win_width; x += 10)
        color_buffer[(win_width * y) + x] = 0x222222FF;
   return ;
}

void    draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for(int row = y; row <= (y + height); row++)
        for(int col = x; col <= (x + width); col++)
            color_buffer[(win_width * row) + col] = color;
    return ;
}

void    render()
{
    SDL_SetRenderDrawColor(game_render, 0, 0, 0, 255);
    SDL_RenderClear(game_render);
    draw_grid();
    draw_rect(66, 77, 128, 256, 0x0099FFFF);
    render_color_buffer();
    clear_color_buffer(0x000000FF);
    SDL_RenderPresent(game_render);
}


void    setup(void)
{
    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * win_width * win_height);
    color_buffer_texture = SDL_CreateTexture(game_render, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, win_width, win_height);
}


int     main(void)
{
    game_is_on = init_window();
    setup();
    while(game_is_on)
    {
        input_catch();
        update();
        render();
    }
    return(0);
}
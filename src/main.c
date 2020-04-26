
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "SDL.h"

# define WIN_WIDTH  800
# define WIN_HEIGHT 600

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
    for (int q = 0; q < (WIN_WIDTH * WIN_HEIGHT); q++)
        color_buffer[q] = color;
}


bool    init_window(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL Initialization Error\n");
        return (false);
    }
    game_win = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_BORDERLESS);
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
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(WIN_WIDTH * sizeof(uint32_t)));
    SDL_RenderCopy(game_render, color_buffer_texture, NULL, NULL);
}

void    render()
{
    SDL_SetRenderDrawColor(game_render, 0, 0, 0, 255);
    SDL_RenderClear(game_render);
    render_color_buffer();
    clear_color_buffer(0xFFFFFF00);
    SDL_RenderPresent(game_render);
}


void    setup(void)
{
    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * WIN_WIDTH * WIN_HEIGHT);
    color_buffer_texture = SDL_CreateTexture(game_render, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, WIN_WIDTH, WIN_HEIGHT);
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
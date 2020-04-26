
#include "render.h"
#include <stdlib.h>
#include <stdbool.h>

#include "SDL/inc/SDL.h"

# define WIN_WIDTH  800
# define WIN_HEIGHT 600

bool    game_is_on;
SDL_Window *    game_win;
SDL_Renderer*   game_render;

void    set_me_free()
{
    free(color_buffer);
    SDL_DestroyRenderer(game_render);
    SDL_DestroyWindow(game_win);
    SDL_Quit();
}

void    clear_color_buffer(uint32_t color)
{
    for (int q = 0; q < (WIN_WIDTH * WIN_HEIGHT; q++))
        color_buffer[q] = color;
}

void    render()
{
    SDL_SetRenderDrawColor(game_render, 0, 0, 0, 255);
    SDL_RenderClear(game_render);

    clear_color_buffer(0xFFFFFF00);

    SDL_RenderPresent(game_render);
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
}


void    setup(void)
{
    uint32_t    color_buffer = {0};

    color_buffer = (uint32_t)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

}


int     main(void)
{

    setup();
    return(0);
}
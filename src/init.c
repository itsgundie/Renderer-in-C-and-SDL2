
#include "render.h"


void    set_me_free(void)
{
    free(color_buffer);
    SDL_DestroyRenderer(game_render);
    SDL_DestroyWindow(game_win);
    SDL_Quit();
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


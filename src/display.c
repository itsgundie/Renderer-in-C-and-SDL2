
#include "render.h"

unsigned int    win_width = 320;
unsigned int    win_height = 240;
SDL_Window *    game_win = NULL;
SDL_Renderer*   game_render = NULL;
uint32_t*       color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;


void    clear_color_buffer(uint32_t color)
{
    for (int q = 0; q < (win_width * win_height); q++)
        color_buffer[q] = color;
}


void    render_color_buffer(void)
{
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(win_width * sizeof(uint32_t)));
    SDL_RenderCopy(game_render, color_buffer_texture, NULL, NULL);
}

void    draw_pixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < win_width && y >= 0 && y < win_height)
        color_buffer[(win_width * y) + x] = color;
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
    for(int row = y; row < (y + height); row++)
        for(int col = x; col < (x + width); col++)
            draw_pixel(col, row, color);
            //color_buffer[(win_width * row) + col] = color;
    return ;
}



void    render(void)
{
    draw_grid();
    for(int q = 0; q < NUMBER_OF_POINTS; q++)
    {
        vec2d_t this_point = cube_projected[q];
        draw_rect(this_point.x + win_width / 2, this_point.y + win_height / 2, 5, 5, 0x55119900);
    }
    
    // draw_rect(66, 77, 128, 256, 0x0099FFFF);
    // for(int q = 0; q < 100; q++)
    // {
    //     draw_pixel(42 + q, 42 + q, 0x336699BB);
    // }
    render_color_buffer();
    clear_color_buffer(0x000000FF);
    SDL_RenderPresent(game_render);
}


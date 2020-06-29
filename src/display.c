
#include "render.h"
#include "triangle.h"
#include "array.h"
#include "vector.h"
#include "mesh.h"
#include "tools.h"


SDL_Window		*game_win = NULL;
SDL_Renderer	*game_render = NULL;
uint32_t		*color_buffer = NULL;
SDL_Texture		*color_buffer_texture = NULL;

unsigned int    win_width = 320;
unsigned int    win_height = 240;


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
    game_win = SDL_CreateWindow(NULL, 
                SDL_WINDOWPOS_CENTERED, 
                SDL_WINDOWPOS_CENTERED,
                win_width, 
                win_height, 
                SDL_WINDOW_BORDERLESS);
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


void    draw_grid(void)
{
   for(int y = 0; y < win_height; y += 10)
	for(int x = 0; x < win_width; x += 10)
		color_buffer[(win_width * y) + x] = 0x222222FF;
}

void    draw_pixel(int x, int y, uint32_t color)
{
	if (x >= 0 && x < win_width && y >= 0 && y < win_height)
		color_buffer[(win_width * y) + x] = color;
}

void	draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float step_x = delta_x / (float)(longest_side_length);
	float step_y = delta_y / (float)(longest_side_length);

	float this_x = x0;
	float this_y = y0;
	for (int q = 0; q <= longest_side_length; q++)
	{
		draw_pixel((int)this_x, (int)this_y, color);
		this_x += step_x;
		this_y += step_y;
	}
}

void	draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{	
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
}

void    draw_triangle_fill_bottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
    float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

    float x_start = x0;
    float x_end = x0;
    for (int y = y0; y <= y2; y++)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }
}

void    draw_triangle_fill_top(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float inv_slope_1 = (float)(x2 - x0) / (y2 - y0);
    float inv_slope_2 = (float)(x2 - x1) / (y2 - y1);

    float x_start = x2;
    float x_end = x2;

    for (int y = y2; y >= y0; y--)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }

}

void	draw_triangle_filled(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    if (y0 > y1)
    {
        swap_int(&y0, &y1);
        swap_int(&x0, &x1);
    }
    if (y1 > y2)
    {
        swap_int(&y1, &y2);
        swap_int(&x1, &x2);
    }
    if (y0 > y1)
    {
        swap_int(&y0, &y1);
        swap_int(&x0, &x1);
    }

    if (y1 == y2)
        draw_triangle_fill_bottom(x0, y0, x1, y1, x2, y2, color);
    else if (y0 == y1)
        draw_triangle_fill_top(x0, y0, x1, y1, x2, y2, color);
    else
    {
        int mid_y = y1;
        int mid_x = ((x2 -  x0) * (y1 - y0) / (y2 - y0) + x0);

        draw_triangle_fill_bottom(x0, y0, x1, y1, mid_x, mid_y, color);
        draw_triangle_fill_top(x1, y1, mid_x, mid_y, x2, y2, color);        
    }
    


}

void	draw_triangle_textured(int x0, int y0, float u0, float v0,
			                    int x1, int y1, float u1, float v1,
			                        int x2, int y2, float u2, float v2,
			                            uint32_t *texture)
{
    if (y0 > y1)
    {
        swap_int(&y0, &y1);
        swap_int(&x0, &x1);
        swap_float(&u0, &u1);
        swap_float(&v0, &v1);
    }
    if (y1 > y2)
    {
        swap_int(&y1, &y2);
        swap_int(&x1, &x2);
        swap_float(&u1, &u2);
        swap_float(&v1, &v2);
    }
    if (y0 > y1)
    {
        swap_int(&y0, &y1);
        swap_int(&x0, &x1);
        swap_float(&u0, &u1);
        swap_float(&v0, &v1);
    }

    float inv_slope_left = 0;
    float inv_slope_right = 0;

    inv_slope_left = (float)(x1 - x0) / ( (y1 - y0) != 0 ? abs(y1 - y0) : 1);
    inv_slope_right = (float)(x2 - x0) / ( (y1 - y0) != 0 ? abs(y2 - y0) : 1);

    if (y1 - y0 != 0)
    {
        for (int y = y0; y <= y1; y++)
        {
            int x_start = x1 + (y - y1) * inv_slope_left;
            int x_end = x0 + (y - y0) * inv_slope_right;
            
            if (x_end < x_start)
                swap_int(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++)
            {
                draw_pixel(x, y , 0xFF7711DD);
            }
        }

    }

    inv_slope_left = (float)(x2 - x1) / ((y2 - y1) != 0 ? abs(y2 - y1) : 1);
    inv_slope_right = (float)(x2 - x0) / ((y2 - y0) != 0 ? abs(y2 - y0) : 1);

    if (y2 - y1)
    {
        for (int y = y1; y <= y2; y++)
        {
            int x_start = x1 + (y - y1) * inv_slope_left;
            int x_end = x0 + (y - y0) * inv_slope_right;

            if (x_end < x_start)
                swap_int(&x_start, &x_end);
            for (int x = x_start; x < x_end; x++)
            {
                draw_pixel(x, y, 0xBB33CC00);
            }
        }
    }
}


void    draw_rect(int x, int y, int width, int height, uint32_t color)
{
	for(int row = y; row < (y + height); row++)
		for(int col = x; col < (x + width); col++)
			draw_pixel(col, row, color);
			//color_buffer[(win_width * row) + col] = color;
	return ;
}

void    render_color_buffer(void)
{
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(win_width * sizeof(uint32_t)));
	SDL_RenderCopy(game_render, color_buffer_texture, NULL, NULL);
}

void    clear_color_buffer(uint32_t color)
{
	for (int q = 0; q < (win_width * win_height); q++)
		color_buffer[q] = color;
}

void    set_me_free(void)
{
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
    SDL_DestroyRenderer(game_render);
    SDL_DestroyWindow(game_win);
    SDL_Quit();
}

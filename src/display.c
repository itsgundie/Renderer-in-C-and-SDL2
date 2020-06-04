
#include "render.h"


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

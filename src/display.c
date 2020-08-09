
#include "render.h"
#include "triangle.h"
#include "array.h"
#include "vector.h"
#include "mesh.h"
#include "tools.h"
#include "texture.h"


SDL_Window		*game_win = NULL;
SDL_Renderer	*game_render = NULL;
uint32_t		*color_buffer = NULL;
float           *z_buf = NULL;
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
                SDL_WINDOW_SHOWN);
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



void draw_texel(
    int x, int y, uint32_t* texture,
    vec4d_t point_a, vec4d_t point_b, vec4d_t point_c,
    tex2_t a_uv, tex2_t b_uv, tex2_t c_uv
) {
    vec2d_t p = { x, y };
    vec2d_t a = vec2d_from_vec4d(point_a);
    vec2d_t b = vec2d_from_vec4d(point_b);
    vec2d_t c = vec2d_from_vec4d(point_c);

    vec3d_t weights = barycentric_weights(a, b, c, p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    // Variables to store the interpolated values of U, V, and also 1/w for the current pixel
    float interpolated_u;
    float interpolated_v;
    float interpolated_reciprocal_w;

    // Perform the interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
    interpolated_u = (a_uv.u / point_a.w) * alpha + (b_uv.u / point_b.w) * beta + (c_uv.u / point_c.w) * gamma;
    interpolated_v = (a_uv.v / point_a.w) * alpha + (b_uv.v / point_b.w) * beta + (c_uv.v / point_c.w) * gamma;

    // Also interpolate the value of 1/w for the current pixel
    interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

    // Now we can divide back both interpolated values by 1/w
    interpolated_u /= interpolated_reciprocal_w;
    interpolated_v /= interpolated_reciprocal_w;

    // Map the UV coordinate to the full texture width and height
    int tex_x = abs((int)(interpolated_u * texture_width));
    int tex_y = abs((int)(interpolated_v * texture_height));

    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

    if(interpolated_reciprocal_w < z_buf[(win_width * y) + x])
    {   
        if ((tex_x >= 0 && tex_x < texture_width) && (tex_y >= 0 && tex_y < texture_height))
        {
            draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);
            z_buf[(win_width * y) + x] = interpolated_reciprocal_w;
        }
    }
}


void draw_triangle_textured(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    uint32_t* texture
) {
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1) {
        swap_int(&y0, &y1);
        swap_int(&x0, &x1);
        swap_float(&z0, &z1);
        swap_float(&w0, &w1);
        swap_float(&u0, &u1);
        swap_float(&v0, &v1);
    }
    if (y1 > y2) {
        swap_int(&y1, &y2);
        swap_int(&x1, &x2);
        swap_float(&z1, &z2);
        swap_float(&w1, &w2);
        swap_float(&u1, &u2);
        swap_float(&v1, &v2);
    }
    if (y0 > y1) {
        swap_int(&y0, &y1);
        swap_int(&x0, &x1);
        swap_float(&z0, &z1);
        swap_float(&w0, &w1);
        swap_float(&u0, &u1);
        swap_float(&v0, &v1);
    }

    // Create vector points and texture coords after we sort the vertices
    vec4d_t point_a = { x0, y0, z0, w0 };
    vec4d_t point_b = { x1, y1, z1, w1 };
    vec4d_t point_c = { x2, y2, z2, w2 };
    tex2_t a_uv = { u0, v0 };
    tex2_t b_uv = { u1, v1 };
    tex2_t c_uv = { u2, v2 };

    ///////////////////////////////////////////////////////
    // Render the upper part of the triangle (flat-bottom)
    ///////////////////////////////////////////////////////
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start) {
                swap_int(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                // Draw our pixel with the color that comes from the texture
                draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
            }
        }
    }

    ///////////////////////////////////////////////////////
    // Render the bottom part of the triangle (flat-top)
    ///////////////////////////////////////////////////////
    inv_slope_1 = 0;
    inv_slope_2 = 0;

    if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start) {
                swap_int(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                // Draw our pixel with the color that comes from the texture
                draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
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
	for(int q = 0; q < (win_width * win_height); q++)
		color_buffer[q] = color;
}

void    clear_z_buf(void)
{
    if(!z_buf)
        return ;
    for(int q = 0; q < (win_width * win_height); q++)
        z_buf[q] = 1.0;
}

void    set_me_free(void)
{
    free(color_buffer);
    free(z_buf);
    array_free(mesh.faces);
    array_free(mesh.vertices);
    SDL_DestroyRenderer(game_render);
    SDL_DestroyWindow(game_win);
    SDL_Quit();
}

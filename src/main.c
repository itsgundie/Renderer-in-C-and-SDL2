
#include "render.h"

float FOV = 1280;
bool            game_is_on = false;
vec3d_t cube[NUMBER_OF_POINTS];
vec2d_t cube_projected[NUMBER_OF_POINTS];
vec3d_t camera_plane = { .x = 0.0f, .y = 0.0f, .z = -5.0f};
vec3d_t cube_rotation = {0};

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

vec2d_t     project3to2d(vec3d_t source)
{
    vec2d_t result = { .x = (FOV * source.x / source.z), .y = (FOV * source.y / source.z)};
    return(result);
}

void    update(void)
{
    cube_rotation.y += 0.01;
    cube_rotation.x += 0.01;
    cube_rotation.z += 0.01;
    for(int q = 0; q < NUMBER_OF_POINTS; q++)
    {
        vec3d_t point = cube[q];
        vec3d_t transformed_point = vec3_rotate_y(point, cube_rotation.y);
        transformed_point = vec3_rotate_x(transformed_point, cube_rotation.x);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);
        transformed_point.z -= camera_plane.z;
        vec2d_t projected_point = project3to2d(transformed_point);
        cube_projected[q] = projected_point;
    }
}

void    setup(void)
{
    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * win_width * win_height);
    color_buffer_texture = SDL_CreateTexture(game_render, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, win_width, win_height);
    int point_index = 0;
    for(float x = -1.0f; x <= 1.0f; x += 0.25)
    {
        for(float y = -1.0f; y <= 1.0f; y += 0.25)
        {
            for(float z = -1.0f; z <= 1.0f; z += 0.25)
            {
                vec3d_t this_point = {.x = x, .y = y, .z = z};
                cube[point_index++] = this_point;
            }
        }

    }
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
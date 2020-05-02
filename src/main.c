
#include "render.h"

float FOV = 1280;
bool            game_is_on = false;
vec3d_t cube[NUMBER_OF_POINTS];
vec2d_t cube_projected[NUMBER_OF_POINTS];
vec3d_t camera_plane = { .x = 0.0f, .y = 0.0f, .z = -5.0f};
vec3d_t cube_rotation = {0};
int prev_frame_time = 0;

triangle_t triangles_to_render[NUM_OF_MESH_FACES];

vec3d_t mesh_vertices[NUM_OF_MESH_VERTS] = {
    { .x = -1,  .y = -1,    .z = -1},
    { .x = -1,  .y =  1,    .z = -1},
    { .x =  1,  .y =  1,    .z = -1},
    { .x =  1,  .y = -1,    .z = -1},
    { .x =  1,  .y =  1,    .z =  1},
    { .x =  1,  .y = -1,    .z =  1},
    { .x = -1,  .y =  1,    .z =  1},
    { .x = -1,  .y = -1,    .z =  1}
};

face_mesh_t mesh_faces[NUM_OF_MESH_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
};

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
    vec2d_t projection = { 
        .x = (FOV * source.x) / source.z,
        .y = (FOV * source.y) / source.z
    };
    return(projection);
}

void    update(void)
{
    int delay_time = FRAME_TARGET_TIME - (SDL_GetTicks() - prev_frame_time);

    if (delay_time > 0 && delay_time <= FRAME_TARGET_TIME)
        SDL_Delay(delay_time);
    
    prev_frame_time = SDL_GetTicks();

    cube_rotation.y += 0.01;
    cube_rotation.x += 0.01;
    cube_rotation.z += 0.01;

    for(int j = 0; j < NUM_OF_MESH_FACES; j++)
    {
        face_mesh_t this_face = mesh_faces[j];
        vec3d_t this_face_vertices[3];
        this_face_vertices[0] = mesh_vertices[this_face.a - 1];
        this_face_vertices[1] = mesh_vertices[this_face.b - 1];
        this_face_vertices[2] = mesh_vertices[this_face.c - 1];

        triangle_t projected_triangle;

        for(int k = 0; k < 3; k++)
        {
            vec3d_t transformed_vertex = this_face_vertices[k];
            transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);
        
            transformed_vertex.z -= camera_plane.z;

            vec2d_t projected_point = project3to2d(transformed_vertex);

            projected_point.x += (win_width / 2);
            projected_point.y += (win_height / 2);

            projected_triangle.points[k] = projected_point;
        }
        triangles_to_render[j] = projected_triangle;
    }
    
}
    // for(int q = 0; q < NUMBER_OF_POINTS; q++)
    // {
    //     vec3d_t point = cube[q];
    //     vec3d_t transformed_point = vec3_rotate_y(point, cube_rotation.y);
    //     transformed_point = vec3_rotate_x(transformed_point, cube_rotation.x);
    //     transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);
    //     transformed_point.z -= camera_plane.z;
    //     vec2d_t projected_point = project3to2d(transformed_point);
    //     cube_projected[q] = projected_point;
    // }


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
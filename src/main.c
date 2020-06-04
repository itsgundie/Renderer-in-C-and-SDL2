#include "array.h"
#include "render.h"
#include "mesh.h"
#include "vector.h"
#include "triangle.h"
#include "array.h"

triangle_t	*triangles_to_render = NULL;
bool	game_is_on = false;
int prev_frame_time = 0;
vec3d_t camera_plane = { .x = 0.0f, .y = 0.0f, .z = -5.0f};

void    setup(void)
{
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * win_width * win_height);
	color_buffer_texture = SDL_CreateTexture(
							game_render, 
							SDL_PIXELFORMAT_RGBA8888,
							SDL_TEXTUREACCESS_STREAMING,
							win_width,
							win_height);
	load_obj_file_data(MODEL_OBJ);
	vec3d_t a = { .x = -4.6f, .y = 1.9f, .z = -9.99f};
	vec3d_t b = { .x = 14.7f, .y = -11.77f, .z = 45.67f};

	float a_length = vec3d_length(a);
	float b_length = vec3d_length(b);

	printf("Length A - %f and B - %f\n", a_length, b_length);

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

	triangles_to_render = NULL;

	mesh.rotation.y += 0.01;
	mesh.rotation.x += 0.0;
	mesh.rotation.z += 0.01;

	int num_faces = array_length(mesh.faces);
	for(int j = 0; j < num_faces; j++)
	{
		face_mesh_t this_face = mesh.faces[j];
		vec3d_t this_face_vertices[3];

		this_face_vertices[0] = mesh.vertices[this_face.a - 1];
		this_face_vertices[1] = mesh.vertices[this_face.b - 1];
		this_face_vertices[2] = mesh.vertices[this_face.c - 1];

		triangle_t projected_triangle;

		for(int k = 0; k < 3; k++)
		{
			vec3d_t transformed_vertex = this_face_vertices[k];
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);
		
			transformed_vertex.z -= camera_plane.z;

			vec2d_t projected_point = project3to2d(transformed_vertex);

			projected_point.x += (win_width / 2);
			projected_point.y += (win_height / 2);

			projected_triangle.points[k] = projected_point;
		}
		array_push(triangles_to_render, projected_triangle);
	}
	
}

void    render(void)
{
	draw_grid();
	int num_triangles = array_length(triangles_to_render);
	for(int q = 0; q < num_triangles; q++)
	{
		triangle_t triangle = triangles_to_render[q];
		draw_rect(triangle.points[0].x , triangle.points[0].y, 4, 4, 0xBBBBBBFF);
		draw_rect(triangle.points[1].x , triangle.points[1].y, 4, 4, 0xBBBBBBFF);
		draw_rect(triangle.points[2].x , triangle.points[2].y, 4, 4, 0xBBBBBBFF);
		draw_triangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			0xFF00FF00);
	}
	array_free(triangles_to_render);
	render_color_buffer();
	clear_color_buffer(0x000000FF);
	SDL_RenderPresent(game_render);
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
	set_me_free();
	return(0);
}
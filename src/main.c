#include "array.h"
#include "render.h"
#include "mesh.h"
#include "vector.h"
#include "triangle.h"
#include "array.h"
#include "matrix.h"
#include "lightning.h"


triangle_t	*triangles_to_render = NULL;
bool	game_is_on = false;
int prev_frame_time = 0;
vec3d_t camera_plane = {0};
mtx4_t	proj_matrx;

void    setup(void)
{
	render_method_e = RENDER_WIRE;
	cull_method_e = CULL_BACKFACE;

	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * win_width * win_height);
	color_buffer_texture = SDL_CreateTexture(
							game_render, 
							SDL_PIXELFORMAT_RGBA8888,
							SDL_TEXTUREACCESS_STREAMING,
							win_width,
							win_height);

	float fov = PIE / 3.0f;
	float aspect = ((float)win_height) / (float)win_width;
	float z_near = 0.1f;
	float z_far = 100.0f;
	proj_matrx = mtx4_perspective(fov, aspect, z_near, z_far);

	// vec3d_t a = { .x = -4.6f, .y = 1.9f, .z = -9.99f};
	// vec3d_t b = { .x = 14.7f, .y = -11.77f, .z = 45.67f};

	// float a_length = vec3d_length(a);
	// float b_length = vec3d_length(b);

	// printf("Length A - %f and B - %f\n", a_length, b_length);

	load_obj_file_data(MODEL_OBJ);

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
			if (eve.key.keysym.sym == SDLK_1)
				render_method_e = RENDER_WIRE;
			if (eve.key.keysym.sym == SDLK_2)
				render_method_e = RENDER_WIRE_VERTEX;
			if (eve.key.keysym.sym == SDLK_3)
				render_method_e = RENDER_FILL_TRIANGLE;
			if (eve.key.keysym.sym == SDLK_4)
				render_method_e = RENDER_FILL_TRIANGLE_WIRE;
			if (eve.key.keysym.sym == SDLK_5)
				cull_method_e = CULL_BACKFACE;
			if (eve.key.keysym.sym == SDLK_6)
				cull_method_e = CULL_NONE;
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

void	swap_triangle(triangle_t *a, triangle_t *b)
{
	triangle_t tmp = *a;
	*a = *b;
	*b = tmp;
}

// void	sort_bubble_faces_array(triangle_t *triangles, int length)
// {
// 	for(int j = 0; j < length; j++)
// 		for(int k = j; k < length; k++)
// 			if (triangles[j].avg_depth < triangles[k].avg_depth)
// 				swap_triangle(&(triangles[j]), &(triangles[k]));
// }

int		sort_quick_faces_array(triangle_t *triangles, int start_index, int last_index)
{
	float pivot = triangles[last_index].avg_depth;

	int j = start_index;

	for (int k = start_index; k < last_index; k++)
	{
		if (triangles[k].avg_depth > pivot)
		{
			if (j != k)
				swap_triangle(&(triangles[j]), &(triangles[k]));
		}
		j++;
	}
	swap_triangle(&(triangles[j]), &(triangles[last_index]));
	return(j);

}

void	sort_quick_triangles(triangle_t *triangles, int start_index, int last_index)
{
	if (start_index < last_index)
	{
		int pivot_index = sort_quick_faces_array(triangles, start_index, last_index);
		sort_quick_triangles(triangles, start_index, pivot_index - 1);
		sort_quick_triangles(triangles, pivot_index + 1, last_index);
	}
}

void    update(void)
{
	int delay_time = FRAME_TARGET_TIME - (SDL_GetTicks() - prev_frame_time);

	if (delay_time > 0 && delay_time <= FRAME_TARGET_TIME)
		SDL_Delay(delay_time);
	
	prev_frame_time = SDL_GetTicks();

	triangles_to_render = NULL;

	mesh.rotation.y += 0.004;
	mesh.rotation.x += 0.007;
	mesh.rotation.z += 0.009;

	// mesh.scaling.x += 0.00002;
	// mesh.scaling.y += 0.00002;
	// mesh.scaling.z += 0.00002;

	// mesh.translation.x += 0.002;
	// mesh.translation.y += 0.00000002;
	mesh.translation.z = 5.0;

	mtx4_t scale_matrix = mtx4_scaling(mesh.scaling.x, mesh.scaling.y, mesh.scaling.z);
	
	mtx4_t	translate_matrix = mtx4_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	mtx4_t	rotation_matrix_x = mtx4_rotation_x(mesh.rotation.x);
	mtx4_t rotation_matrix_y = mtx4_rotation_y(mesh.rotation.y);
	mtx4_t rotation_matrix_z = mtx4_rotation_z(mesh.rotation.z);

	int num_faces = array_length(mesh.faces);
	for(int j = 0; j < num_faces; j++)
	{
		face_mesh_t this_face = mesh.faces[j];
		vec3d_t this_face_vertices[3];
		//this_face.color = 0xABCDEF99;
		// this_face.color = 0x11111111 * (j + 1);

		this_face_vertices[0] = mesh.vertices[this_face.a - 1];
		this_face_vertices[1] = mesh.vertices[this_face.b - 1];
		this_face_vertices[2] = mesh.vertices[this_face.c - 1];

		triangle_t projected_triangle;

		vec3d_t transformed_vertices[3] = {0};

		for(int k = 0; k < 3; k++)
		{
			vec4d_t transformed_vertex = vec4d_from_vec3d(this_face_vertices[k]);

			mtx4_t world_matrix = mtx4_identity();

			world_matrix = mtx4_mult_mtx4(scale_matrix, world_matrix);
			world_matrix = mtx4_mult_mtx4(rotation_matrix_x, world_matrix);
			world_matrix = mtx4_mult_mtx4(rotation_matrix_y, world_matrix);
			world_matrix = mtx4_mult_mtx4(rotation_matrix_z, world_matrix);
			world_matrix = mtx4_mult_mtx4(translate_matrix, world_matrix);
			// world_matrix = mtx4_mult_mtx4(world_matrix, rotation_matrix);
			// world_matrix = mtx4_mult_mtx4(world_matrix, translate_matrix);
			// transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			// transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			// transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex = mtx4_mult_vec4d(world_matrix, transformed_vertex);

			//transformed_vertex = mtx4_mult_vec4d(scale_matrix,transformed_vertex);
			// transformed_vertex = mtx4_mult_vec4d(rotation_matrix_x, transformed_vertex);
			// transformed_vertex = mtx4_mult_vec4d(rotation_matrix_y, transformed_vertex);
			// transformed_vertex = mtx4_mult_vec4d(rotation_matrix_z, transformed_vertex);
			// transformed_vertex = mtx4_mult_vec4d(translate_matrix, transformed_vertex);

			transformed_vertices[k] = vec3d_from_vec4d(transformed_vertex);
		}

		vec3d_t vec_a = transformed_vertices[0];
		vec3d_t vec_b = transformed_vertices[1];
		vec3d_t vec_c = transformed_vertices[2];

		vec3d_t vec_ab = vec3d_sub(vec_b, vec_a);
		vec3d_t vec_ac = vec3d_sub(vec_c, vec_a);

		vec3d_normalize(&vec_ab);
		vec3d_normalize(&vec_ac);

		vec3d_t normal = vec3d_cross(vec_ab, vec_ac);

		vec3d_normalize(&normal);

		vec3d_t camera_ray = vec3d_sub(camera_plane, vec_a);

		if (cull_method_e == CULL_BACKFACE)
		{
			float is_visible = vec3d_dot(normal, camera_ray);

			if (is_visible < 0)
				continue;
		}

		vec4d_t projected_points[3];

		for(int l = 0; l < 3; l++)
		{
			projected_points[l] = mtx4_mult_vec4d_projection(proj_matrx, vec4d_from_vec3d(transformed_vertices[l]));
			//vec2d_t projected_point = project3to2d(transformed_vertices[l]);

			projected_points[l].x *= (win_width / 2.0f);
			projected_points[l].y *= (win_height / 2.0f);

			projected_points[l].y *= -1;

			projected_points[l].x += (win_width / 2.0f);
			projected_points[l].y += (win_height / 2.0f);

			//transformed_vertices[l] = vec3d_from_vec4d(projected_points[l]);

		}
			projected_triangle.avg_depth = (transformed_vertices[0].z
				+ transformed_vertices[1].z
			 	+ transformed_vertices[2].z) / 3;

			float luminosity = -(vec3d_dot(normal, light.direction));

			uint32_t triangle_color = lumos(this_face.color, luminosity);

			projected_triangle.points[0].x = projected_points[0].x;
			projected_triangle.points[0].y = projected_points[0].y;
			projected_triangle.points[1].x = projected_points[1].x;
			projected_triangle.points[1].y = projected_points[1].y;
			projected_triangle.points[2].x = projected_points[2].x;
			projected_triangle.points[2].y = projected_points[2].y;
			projected_triangle.color = triangle_color;
		array_push(triangles_to_render, projected_triangle);

	}
	int num_triangles = array_length(triangles_to_render);

	sort_quick_triangles(triangles_to_render, 0, num_triangles - 1);

	// for(int j = 0; j < num_triangles; j++)
	// 	for(int k = j; k < num_triangles; k++)
	// 		if (triangles_to_render[j].avg_depth < triangles_to_render[k].avg_depth)
	// 			swap_triangle(&(triangles_to_render[j]), &(triangles_to_render[k]));
	//if (num_faces <= 5)
	//	sort_bubble_faces_array(triangles_to_render, num_faces);
	// else
	// 	sort_quick_faces_array(triangles_to_render, num_faces);
	
	
}

void    render(void)
{
	SDL_RenderClear(game_render);
	draw_grid();
	// printf("Render mode - %d\n", render_method_e);
	// printf("Culling mode - %d\n", cull_method_e);
	int num_triangles = array_length(triangles_to_render);

	for(int q = 0; q < num_triangles; q++)
	{
		triangle_t triangle = triangles_to_render[q];
		
		if (render_method_e == RENDER_FILL_TRIANGLE ||
			render_method_e == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_triangle_filled(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				triangle.color);
		}
		
		if (render_method_e == RENDER_WIRE ||
			render_method_e == RENDER_WIRE_VERTEX ||
			render_method_e == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				COLOR_RED);
		}

		if (render_method_e == RENDER_WIRE_VERTEX)
		{
			draw_rect( triangle.points[0].x - 12,
				triangle.points[0].y, 24, 24, COLOR_GREEN);
			draw_rect( triangle.points[1].x - 12,
				triangle.points[1].y, 24, 24, COLOR_GREEN);
			draw_rect( triangle.points[2].x - 12,
				triangle.points[2].y, 24, 24, COLOR_GREEN);
		}

	}

	array_free(triangles_to_render);
	render_color_buffer();
	clear_color_buffer(COLOR_BLACK);
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
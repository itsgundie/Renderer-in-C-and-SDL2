#include "render.h"
#include "vector.h"
#include "triangle.h"
#include "mesh.h"

mesh_t mesh = {
    .vertices = NULL,
    .normals = NULL,
    .textures = NULL,
    .normal_indices = NULL,
    .texture_indices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0 },
    .translation = { 0, 0, 0 },
    .scaling = { 1.0, 1.0, 1.0 }
};

// vec3d_t cube_vertices[NUM_CUBE_VERTS] = {{0}};

vec3d_t cube_vertices[NUM_CUBE_VERTS] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};


// face_mesh_t cube_faces[NUM_CUBE_FACES] = {{0}};

face_mesh_t cube_faces[NUM_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 1, .b = 3, .c = 4, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // right
    { .a = 4, .b = 3, .c = 5, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 4, .b = 5, .c = 6, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // back
    { .a = 6, .b = 5, .c = 7, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 6, .b = 7, .c = 8, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // left
    { .a = 8, .b = 7, .c = 2, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 8, .b = 2, .c = 1, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // top
    { .a = 2, .b = 7, .c = 5, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 2, .b = 5, .c = 3, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // bottom
    { .a = 6, .b = 8, .c = 1, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 6, .b = 1, .c = 4, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF }
};


void    load_cube_mesh(void)
{
	for( int j = 0; j < NUM_CUBE_VERTS; j++)
	{
		vec3d_t cube_vert = cube_vertices[j];
		array_push(mesh.vertices, cube_vert);
	}
	for ( int k = 0; k < NUM_CUBE_FACES; k++)
	{
		face_mesh_t cube_face = cube_faces[k];
		array_push(mesh.faces, cube_face);
	}
}

// void    load_obj_file_data(char *filename)
// {
// 	FILE *file;
// 	file = fopen(filename, "r");

// 	char line[LINE_SIZE] = {0};

// 	while (fgets(line, LINE_SIZE, file))
// 	{
// 		if (strncmp(line, "v ", 2) == 0)
// 		{
// 			vec3d_t vertex;
// 			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
// 			array_push(mesh.vertices, vertex);
// 		}
// 		if (strncmp(line, "f ", 2) == 0)
// 		{
// 				int32_t vertex_indices[3] = {0};
// 				int32_t texture_indices[3] = {0};
// 				int32_t normal_indices[3] = {0};
				
// 			sscanf(line, "f %d%d%d %d%d%d %d%d%d", 
// 					&vertex_indices[0], &texture_indices[0], &normal_indices[0],
// 					&vertex_indices[1], &texture_indices[1], &normal_indices[1],
// 					&vertex_indices[2], &texture_indices[2], &normal_indices[2]
// 			);
// 			face_mesh_t this_face = {
// 				.a = vertex_indices[0],
// 				.b = vertex_indices[1],
// 				.c = vertex_indices[2]
// 			};
// 			array_push(mesh.faces, this_face);
// 		}
// 	}
// }

void load_obj_file_data(char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    char line[LINE_SIZE];

    face_mesh_t vertex_indices;
    face_mesh_t texture_indices;
    face_mesh_t normal_indices;

    while (fgets(line, LINE_SIZE, file)) {
        switch (line[0]) {
            case 'v': { // Vertex information
                switch (line[1]) {
                    case ' ': {
                        vec3d_t vertex;
                        sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                        array_push(mesh.vertices, vertex);
                    } break;
                    case 'n': {
                        vec3d_t normal;
                        sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                        array_push(mesh.normals, normal);
                    } break;
                    case 't': {
                        vec2d_t texture;
                        sscanf(line, "vt %f %f", &texture.x, &texture.y);
                        array_push(mesh.textures, texture);
                    } break;
                }
            } break;

            case 'f': { // Face information
                int match = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                                   &vertex_indices.a, &texture_indices.a, &normal_indices.a,
                                   &vertex_indices.b, &texture_indices.b, &normal_indices.b,
                                   &vertex_indices.c, &texture_indices.c, &normal_indices.c
                );
                if (match == 9) {
                    array_push(mesh.texture_indices, texture_indices);
                    array_push(mesh.normal_indices, normal_indices);
                } else {
                    match = sscanf(line, "f %d/%d %d/%d %d/%d",
                                   &vertex_indices.a, &texture_indices.a,
                                   &vertex_indices.b, &texture_indices.b,
                                   &vertex_indices.c, &texture_indices.c
                    );
                    if (match == 6) {
                        array_push(mesh.texture_indices, texture_indices);
                    } else {
                        sscanf(line, "f %d %d %d",
                               &vertex_indices.a,
                               &vertex_indices.b,
                               &vertex_indices.c
                        );
                    }
                }
                vertex_indices.color = COLOR_WHITE;
                array_push(mesh.faces, vertex_indices);
            } break;
        }
    }
}
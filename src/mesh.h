#ifndef MESH_H
#define MESH_H

#include "render.h"
#include "triangle.h"
#include "vector.h"

#define NUM_CUBE_VERTS 8
#define NUM_CUBE_FACES (6 * 2)

extern vec3d_t cube_vertices[NUM_CUBE_VERTS];
extern face_mesh_t cube_faces[NUM_CUBE_FACES];

// typedef struct		s_mesh
// {
// 	vec3d_t*		vertices;
// 	face_mesh_t*	faces;
// 	vec3d_t			rotation;
// }					mesh_t;

typedef struct {
    vec3d_t* vertices; // dynamic array of vertices
    vec3d_t* normals; // dynamic array of vertices
    vec2d_t* textures; // dynamic array of vertices
    face_mesh_t* faces;    // dynamic array of faces
    face_mesh_t* normal_indices;    // dynamic array of faces
    face_mesh_t* texture_indices;   // dynamic array of faces
    vec3d_t rotation;  // rotation with x, y, and z values
    vec3d_t translation; // translate x, y and z coordinate
    vec3d_t scaling; // scale by some or all dimensions
} mesh_t;

extern mesh_t mesh;

void        load_cube_mesh(void);
void		load_obj_file_data(char *filename);

#endif
#ifndef VECTOR_H
#define VECTOR_H

typedef struct s_vec2d
{
		float x;
		float y;
}               vec2d_t;

typedef struct s_vec3d
{
				float x;
				float y;
				float z;
}               vec3d_t;

float		vec2d_length(vec2d_t vector);

float		vec3d_length(vec3d_t vector);

vec2d_t		vec2d_add(vec2d_t a, vec2d_t b);

vec3d_t		vec3d_add(vec3d_t a, vec3d_t b);

vec2d_t		vec2d_sub(vec2d_t a, vec2d_t b);

vec3d_t		vec3d_sub(vec3d_t a, vec3d_t b);

vec2d_t		vec2d_mul(vec2d_t vec, float factor);

vec3d_t		vec3d_mul(vec3d_t vec, float factor);

vec2d_t		vec2d_div(vec2d_t vec, float factor);

vec3d_t		vec3d_div(vec3d_t vec, float factor);

float		vec2d_dot(vec2d_t a, vec2d_t b);

float		vec3d_dot(vec3d_t a, vec3d_t b);

vec3d_t		vec3d_cross(vec3d_t a, vec3d_t b);

void		vec2d_normalize(vec2d_t *vector);

void		vec3d_normalize(vec3d_t *vector);

vec3d_t		vec3_rotate_x(vec3d_t vector, float angle);

vec3d_t		vec3_rotate_y(vec3d_t vector, float angle);

vec3d_t		vec3_rotate_z(vec3d_t vector, float angle);


#endif
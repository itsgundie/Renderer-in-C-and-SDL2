#include "array.h"
#include "render.h"
#include "mesh.h"
#include "vector.h"
#include "triangle.h"

float		vec2d_length(vec2d_t vector)
{
  return(sqrt(vector.x * vector.x 
			  + vector.y * vector.y));
}

float		vec3d_length(vec3d_t vector)
{
  return(sqrt(vector.x * vector.x 
			  + vector.y * vector.y 
			  + vector.z * vector.z));
}


vec2d_t		vec2d_add(vec2d_t a, vec2d_t b)
{
	vec2d_t result = {
	  .x = a.x + b.x,
	  .y = a.y + b.y
	};
  return(result);
}


vec3d_t		vec3d_add(vec3d_t a, vec3d_t b)
{
	vec3d_t result = {
	  .x = a.x + b.x,
	  .y = a.y + b.y,
	  .z = a.z + b.z
	};
  return(result);
}

vec2d_t		vec2d_sub(vec2d_t a, vec2d_t b)
{
	vec2d_t result = {
	  .x = a.x - b.x,
	  .y = a.y - b.y
	};
  return(result);
}


vec3d_t		vec3d_sub(vec3d_t a, vec3d_t b)
{
	vec3d_t result = {
	  .x = a.x - b.x,
	  .y = a.y - b.y,
	  .z = a.z - b.z
	};
  return(result);
}


vec2d_t		vec2d_mul(vec2d_t vec, float factor)
{
	vec2d_t result = {
	  .x = vec.x * factor,
	  .y = vec.y * factor
	};
	return(result);
}

vec3d_t		vec3d_mul(vec3d_t vec, float factor)
{
	vec3d_t result = {
	  .x = vec.x * factor,
	  .y = vec.y * factor,
	  .z = vec.z * factor
	};
	return(result);
}

vec2d_t		vec2d_div(vec2d_t vec, float factor)
{
	vec2d_t result = {
	  .x = vec.x / factor,
	  .y = vec.y / factor
	};
	return(result);
}

vec3d_t		vec3d_div(vec3d_t vec, float factor)
{
	vec3d_t result = {
	  .x = vec.x / factor,
	  .y = vec.y / factor,
	  .z = vec.z / factor
	};
	return(result);
}

float     vec2d_dot(vec2d_t a, vec2d_t b)
{
  return((a.x * b.x) + (a.y * b.y));
}


float     vec3d_dot(vec3d_t a, vec3d_t b)
{
  return((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

vec3d_t   vec3d_cross(vec3d_t a, vec3d_t b)
{
	vec3d_t result = {
	  .x = a.y * b.z - a.z * b.y,
	  .y = a.z * b.x - a.x * b.z,
	  .z = a.x * b.y - a.y * b.x
	};
	return(result);
}

void	vec2d_normalize(vec2d_t *vector)
{
	float length = sqrt(vector->x * vector->x 
					+ vector->y * vector->y);
	vector->x /= length;
	vector->y /= length;
}

void	vec3d_normalize(vec3d_t *vector)
{
	float length = sqrt(vector->x * vector->x 
					+ vector->y * vector->y 
					+ vector->z * vector->z);
	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
}

vec3d_t vec3_rotate_x(vec3d_t vector, float angle)
{
	vec3d_t rotated = { .x = vector.x,
						.y = vector.y * cos(angle) - vector.z * sin(angle),
						.z = vector.y * sin(angle) + vector.z * cos(angle)
					  };
	return(rotated);
}


vec3d_t vec3_rotate_y(vec3d_t vector, float angle)
{
	vec3d_t rotated = { .x = vector.x * cos(angle) - vector.z * sin(angle),
						.y = vector.y ,
						.z = vector.x * sin(angle) + vector.z * cos(angle)
					  };
	return(rotated);
}


vec3d_t vec3_rotate_z(vec3d_t vector, float angle)
{
	vec3d_t rotated = { .x = vector.x * cos(angle) - vector.y * sin(angle),
						.y = vector.x * sin(angle) + vector.y * cos(angle),
						.z = vector.z
					  };
	return(rotated);
}
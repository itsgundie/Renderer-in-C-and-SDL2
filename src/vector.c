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
	vec3d_t rotated = { .x = vector.x * cos(angle) + vector.z * sin(angle),
						.y = vector.y ,
						.z = vector.x * -sin(angle) + vector.z * cos(angle)
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

vec3d_t		vec3d_from_vec4d(vec4d_t vec)
{
	vec3d_t		result = {vec.x, vec.y, vec.z};
	return(result);
}

vec4d_t		vec4d_from_vec3d(vec3d_t vec)
{
	vec4d_t		result = {vec.x, vec.y, vec.z, 1.0f};
	return(result);
}



vec3d_t		barycentric_weights(vec2d_t a, vec2d_t b, vec2d_t c, vec2d_t p)
{
	vec2d_t ab = vec2d_sub(b,a);
	vec2d_t bc = vec2d_sub(c,b);
	vec2d_t ac = vec2d_sub(c,a);
	vec2d_t ap = vec2d_sub(p,a);
	vec2d_t bp = vec2d_sub(p,b);

	float area_triangle_abc = (ab.x * ac.y - ab.y * ac.x);

	float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;
	float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;
	float gamma = 1 - alpha - beta;

	vec3d_t weights = {alpha, beta, gamma};

	return(weights);
}
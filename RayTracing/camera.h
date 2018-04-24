#pragma once

#include "ray.h"

class camera
{
public:
	camera()
	{
		lower_left_corner = vec3(-4.0f, -3.0f, -1.0f);
		horizontal = vec3(8.0, 0.0, 0.0);
		vertical = vec3(0.0, 6.0, 0.0);
		origin = vec3(0.0, 0.0, 0.0);
	}
	ray get_ray(float u, float v)
	{
		return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
	}

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};

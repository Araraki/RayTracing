#pragma once

#include "ray.h"

class camera
{
public:
	camera(int h, int w)
	{
		float unitY = float(h) / 100.0f;
		float unitX = float(w) / 100.0f;
		lower_left_corner = vec3(-unitX*0.5f, -unitY*0.5f, -1.0f);
		horizontal = vec3(unitX, 0.0, 0.0);
		vertical = vec3(0.0, unitY, 0.0);
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

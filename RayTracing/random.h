#pragma once

#include "vec3.h"

//#define FRAND (float(rand()) / (RAND_MAX))
inline float frand()
{
	return float(rand()) / RAND_MAX;
}

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0f*vec3(frand(), frand(), frand()) - vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);
	return p;
}
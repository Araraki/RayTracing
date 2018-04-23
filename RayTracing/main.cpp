#include <fstream>
#include "vec3.h"
#include "ray.h"

bool hit_sphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - 4 * a*c;
	return (discriminant > 0);
}

vec3 color(const ray& r)
{
	// sphere
	if (hit_sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, r))
		return vec3(1.0f, 0.0f, 0.0f);
	// background
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f*(unit_direction.y() + 1.0f);
	return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
}

int main(int argc, char* argv[])
{
	int nx = 200;
	int ny = 100;
	std::ofstream outf;
	outf.open("d://Documents//Stanley.Wang//Desktop//out.ppm");
	outf << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	for (auto j = ny - 1; j >= 0; --j)	// Down 2 Up
	{
		for (auto i = 0; i < nx; ++i)	// Left 2 Right
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = color(r);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outf << ir << " " << ig << " " << ib << "\n";
		}
	}
	outf.close();
}

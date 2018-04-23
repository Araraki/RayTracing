#include <fstream>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "sphere.h"
#include "hitable_list.h"

vec3 color(const ray& r, hitable* world)
{
	hit_record rec;
	if (world->hit(r, 0.0f, FLT_MAX, rec))
	{
		return 0.5f*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
	}	
}

int main(int argc, char* argv[])
{
	int nx = 800;
	int ny = 600;
	std::ofstream outf;
	outf.open("d://Documents//Stanley.Wang//Desktop//out.ppm");
	outf << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lower_left_corner(-4.0f, -3.0f, -1.0f);
	vec3 horizontal(8.0, 0.0, 0.0);
	vec3 vertical(0.0, 6.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	hitable* list[2];
	list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100);
	hitable* world = new hitable_list(list, 2);

	for (auto j = ny - 1; j >= 0; --j)	// Down 2 Up
	{
		for (auto i = 0; i < nx; ++i)	// Left 2 Right
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = color(r, world);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outf << ir << " " << ig << " " << ib << "\n";
		}
	}
	outf.close();
}

#include <fstream>
#include <cstdlib>
#include <ctime>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "random.h"
#include "material.h"

vec3 color(const ray& r, hitable* world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation*color(scattered, world, depth + 1);
		else
			return vec3(0.0f, 0.0f, 0.0f);
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
	int ns = 100;

	std::ofstream outf;
	outf.open("d://Documents//Stanley.Wang//Desktop//out.ppm");
	outf << "P3\n" << nx << " " << ny << "\n255\n";

	camera cam;

	hitable* list[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5f, new lambertian(vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new sphere(vec3(0, -100.5f, -1), 100, new lambertian(vec3(0.8f, 0.8f, 0.3f)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5f, new metal(vec3(0.9f, 0.9f, 0.9f), 1.0f));
	hitable* world = new hitable_list(list, 4);

	for (auto j = ny - 1; j >= 0; --j)	// Down 2 Up
	{
		for (auto i = 0; i < nx; ++i)	// Left 2 Right
		{
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + frand()) / float(nx);
				float v = float(j + frand()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0f);
				col += color(r, world, 0);
			}			
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outf << ir << " " << ig << " " << ib << "\n";
		}
	}
	outf.close();
}

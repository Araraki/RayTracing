#define _USE_MATH_DEFINES

#include <iomanip>
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
			return {0.0f, 0.0f, 0.0f};
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
	}	
}

hitable* random_scene()
{
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, -1), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));

	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = frand();
			vec3 center(a + 0.9*frand(), 0.2, b + 0.9*frand());
			if ((center-vec3(4,0.2,0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(frand()*frand(), frand()*frand(), frand()*frand())));
				else if (choose_mat < 0.95)
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 *(1 + frand()), 0.5 *(1 + frand()), 0.5 *(1 + frand())), 0.5*frand()));
				else
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

int main(int argc, char* argv[])
{
	const int nx = 1600;
	const int ny = 900;

	int ns = 128; 

	std::ofstream outf;
	outf.open("C://Users//Stanley//Desktop//out.ppm");
	outf << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10;
	float aperture = 0.1f;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx)/float(ny), aperture, dist_to_focus);

	//hitable* list[5];
	//list[0] = new sphere(vec3(0, 0, -1), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
	//list[1] = new sphere(vec3(0, -100.5f, -1), 100, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
	//list[2] = new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.0f));
	//list[3] = new sphere(vec3(-1, 0, -1), 0.48f, new dielectric(1.5f));
	//list[4] = new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(1.5f));
	//hitable* world = new hitable_list(list, 5);

	//float R = cos(M_PI / 4);
	//hitable* list[2];
	//list[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0.0f, 0.0f, 0.1f)));
	//list[1] = new sphere(vec3( R, 0, -1), R, new lambertian(vec3(1.0f, 0.0f, 0.0f)));
	//hitable* world = new hitable_list(list, 2);

	hitable* world = random_scene();

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
		std::cout << (1.0f - float(j) / ny)*100 << "%\n";
	}
	outf.close();
}

#include <time.h>
#include "Headers/Hitable_list.h"
#include "Headers/Sphere.h"
#include "Headers/Camera.h"
#include "Headers/Material.h"
#include <fstream>
Vector3 ColorAtRay(const Ray& ray, HitableList& world, int depth);

Vector3 RandomPointInUnitSphere();
unsigned long int count = 0;

//> "$(ProjectDir)Renders\Output.ppm"

int main()
{
	 
	const int Width = 200;
	const int Height = 100;
	const int Samples = 200;
	const int MaxDepth = 100;

	std::cout << "P3\n";
	std::cout << Width << " " << Height << "\n255\n"; 

	Vector3 LookFrom(-2, 2, 1);
	Vector3 LookAt(0, 0, -1);
	Vector3 ViewUp(0, 1, 0);
	double focalDistance = (LookFrom - LookAt).length();
	double aperture = 0.01;
	Camera cam(30,(Width/(double)Height),aperture,focalDistance,LookFrom,LookAt,ViewUp);
	
	HitableList World(make_shared<Sphere>( Vector3(0, 0, -1) , 0.5, make_shared<Lambertian>(Vector3(0.7,0.3,0.3))));
	World.Add(make_shared<Sphere>( Vector3(0, -100.5, -1) , 100, make_shared<Lambertian>(Vector3(0.8,0.8,0))));
	World.Add(make_shared<Sphere>(Vector3(1, 0, -1), 0.5, make_shared<Metal>(Vector3(0.8, 0.6, 0.2), 0.0)));
	World.Add(make_shared<Sphere>(Vector3(-1, 0, -1), 0.5, make_shared<Dielectric>(1.5)));
	//World.Add(make_shared<Sphere>(Vector3(-1, 0, -1), -0.48, make_shared<Dielectric>(1.5)));
	Vector3 color(0, 0, 0);
	for (int y = Height - 1; y >= 0; --y)
	{
		for (int x = 0; x < Width; ++x)
		{			
			std::cerr << "\rPercent Complete : " << ((Width)*(Height-y-1)+(x+1))*100/(float)(Width*Height) << "  x : " << x << "  y : " << y << ' ' << std::flush;
			 
		

			for (int s = 0; s < Samples; s++)
			{
				double U = (x + RandomDouble()) / (double)Width;
				double V = (y + RandomDouble()) / (double)Height;
				Ray ray = cam.GetRayAtUV(U, V);
				Vector3 SamplingColor= ColorAtRay(ray, World, MaxDepth);
				double red = SamplingColor.r();
				color += SamplingColor;
			}
			color /= (double)Samples;
			color = Vector3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b()));			 
			int r = static_cast<int>(256 *  Clamp(color.r(),0,0.999));
			int g = static_cast<int>(256 *  Clamp(color.g(),0,0.999));
			int b = static_cast<int>(256 *  Clamp(color.b(),0,0.999));
			std::cout << r << " " << g << " " << b<<" ";				 
		}
		std::cout << std::endl;		 
	}

	std::cerr << "\n DONE";
	std::cin.get();
}

Vector3 ColorAtRay(const Ray& ray, HitableList& world, int depth)
{
	if (depth <= 0)
		return Vector3(0, 0, 0);

	HitRecord hitRecord;
	if (world.isHit(ray, 0.01, DBL_MAX, hitRecord))
	{
		Vector3 Attenuation;
		Ray ScatteredRay;
		if ( hitRecord.mat_ptr->scatter(ray, hitRecord, Attenuation, ScatteredRay))
			return Attenuation * ColorAtRay(ScatteredRay, world, depth - 1);	 
		return Vector3(0, 0, 0);
	}
	else
	{
		Vector3 NormalizedDirection = ray.Ray_Direction().normalized();
		double NormalizedY = 0.5*(NormalizedDirection.y() + 1);
		return (1 - NormalizedY)*Vector3(1, 1, 1) + NormalizedY * Vector3(0.5, 0.7, 1.0);
	}
}






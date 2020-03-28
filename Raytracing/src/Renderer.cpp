#include <time.h>
#include "Headers/Hitable_list.h"
#include "Headers/Sphere.h"
#include "Headers/Camera.h"
#include "Headers/Material.h"
#include <fstream>
Vector3 ColorAtRay(const Ray& ray, HitableList* world, int depth);
HitableList* GetRandomWorld();
Vector3 RandomPointInUnitSphere();
unsigned long int count = 0;



int main()
{
	srand((unsigned)time(0));
	int Width = 1920;
	int Height = 1080;
	int Samples = 100;
	std::fstream OutputFile("Output.txt", std::ios::out | std::ios::trunc);
	OutputFile << "P3\n";
	OutputFile << Width << " " << Height << "\n255\n";
	Vector3 LookFrom(13, 2, 3);
	Vector3 LookAt(0, 0, 0);
	Vector3 ViewUp(0, 1, 0);
	double focalDistance = (LookFrom - LookAt).length();
	double aperture = 0.1;
	Camera cam(30,(Width/(double)Height),aperture,focalDistance,LookFrom,LookAt,ViewUp);
	HitableList* World = GetRandomWorld(); 
	for (int y = Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < Width; x++)
		{			
			std::cout << "Percent Complete : " << ((Width)*(Height-y-1)+(x+1))*100/(float)(Width*Height) << "  x : " << x << "  y : " << y ;
			Vector3 color(0,0,0);
			for (int s = 0; s < Samples; s++)
			{
				float U = (x + rand01()) / (float)Width;
				float V = (y + rand01()) / (float)Height;
				Ray ray = cam.GetRayAtUV(U, V);
				color += ColorAtRay(ray, World, 0);
			}
			color /= (double)Samples;
			color = Vector3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b()));
			int r = (int)(255.99 * color.r());
			int g = (int)(255.99 * color.g());
			int b = (int)(255.99 * color.b());
			std::cout << "\r";
			//std::cout << r << " " << g << " " << b<<"  ";			 
			OutputFile << r << " " << g << " " << b << "  ";
		}
		//std::cout << std::endl;
		OutputFile << std::endl;
	}
	OutputFile.close();
	std::cin.get();
}

Vector3 ColorAtRay(const Ray& ray, HitableList* world, int depth)
{
	HitRecord hitRecord;
	if (world->isHit(ray, 0.01, DBL_MAX, hitRecord))
	{
		Vector3 Attenuation;
		Ray ScatteredRay;
		if (depth < 50 && hitRecord.mat_ptr->scatter(ray, hitRecord, Attenuation, ScatteredRay))
			return Attenuation * ColorAtRay(ScatteredRay, world, depth + 1);
		else
			return Vector3(0, 0, 0);
	}
	else
	{
		Vector3 NormalizedDirection = ray.Ray_Direction().normalized();
		double NormalizedY = 0.5*(NormalizedDirection.y() + 1);
		return (1 - NormalizedY)*Vector3(1.0, 1.0, 1.0) + NormalizedY * Vector3(.5, .7, 1.0);
	}
}

HitableList * GetRandomWorld()
{
	Hitable** ModelArrays= new Hitable*[500];
	ModelArrays[0] = new Sphere(Vector3(0, -1000.5, -1), 1000, new Lambertian(Vector3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double MaterialProbability = rand01(); 
			Vector3 Center(a + 0.9*rand01(), 0.2, b + 0.9*rand01());
			if ((Center - Vector3(0, 1, 0)).length() > 2)
			{
				if (MaterialProbability < 0.8)
					ModelArrays[i++] = new Sphere(Center, 0.2, new Lambertian(Vector3(rand01()*rand01(), rand01()*rand01(), rand01()*rand01())));
				else if(MaterialProbability < 0.9)
					ModelArrays[i++] = new Sphere(Center, 0.2, new Metal(Vector3(0.5*(1+rand01()), 0.5*(1 + rand01()), 0.5*(1 + rand01())),0.5*rand01()));
				else
					ModelArrays[i++] = new Sphere(Center, 0.2, new Dielectric(1.5));
			}

		}
	}
	ModelArrays[i++] = new Sphere(Vector3(0,1,0), 1, new Dielectric(1.5));
	ModelArrays[i++] = new Sphere(Vector3(-4,1,0), 1, new Lambertian(Vector3(0.3, 0.5, 0.1)));
	ModelArrays[i++] = new Sphere(Vector3(4,1,0),1, new Metal(Vector3(0.7, 0.6, 0.5), 0.0));
	return new HitableList(ModelArrays, i);
}




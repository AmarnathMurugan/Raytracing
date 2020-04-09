#include "Headers/Hitable_list.h"
#include "Headers/Sphere.h"
#include "Headers/Camera.h"
#include "Headers/Material.h"
#include <thread>
#include <mutex>
#include <chrono>

#pragma region FUNCTION_SIGNATURES
	HitableList GetWorld();
	Vector3 ColorAtRay(const Ray& ray, HitableList& world, int depth);
	void GetChunkRange(int& Start, int& End);
	void RenderImage(int ThreadIndex, HitableList& World, Camera& cam);

#pragma endregion
	   
#pragma region PUBLIC_VARIABLES
	const int imageWidth = 200;
	const int imageHeight = 100;
	const int Samples = 200;
	const int MaxDepth = 100;

	const int NumberOfThreads = std::thread::hardware_concurrency();
	const int ChunkSize = imageHeight / NumberOfThreads;

	int NextHeight = imageHeight - 1;
	Vector3 finalBuffer[imageHeight][imageWidth];
	int Intbuffer[imageHeight][imageWidth][3];
	std::mutex ChunkRangeMutex;
	bool isComplete = false;
#pragma endregion
	   
int main()
{ 	
	std::cout << "P3\n";
	std::cout << imageWidth << " " << imageHeight << "\n255\n"; 

	Vector3 LookFrom(0, 0, 3);
	Vector3 LookAt(0, 0, -1);
	Vector3 ViewUp(0, 1, 0);
	double focalDistance = (LookFrom - LookAt).length();
	double aperture = 0.01;
	Camera cam(30,(imageWidth/(double)imageHeight),aperture,focalDistance,LookFrom,LookAt,ViewUp);
	
	HitableList World = GetWorld();

	auto StartTime = std::chrono::steady_clock::now();

	std::vector<std::thread> threads;
	threads.reserve(NumberOfThreads);
	for (int i = 0; i < NumberOfThreads; i++)
		threads.emplace_back(std::thread(RenderImage, i, std::ref(World), std::ref(cam)));	
	for (std::thread &t : threads)
		if (t.joinable()) t.join();
	

	for (int y = imageHeight - 1; y >= 0; --y)
	{
		for (int x = 0; x < imageWidth; ++x)
		{
			int r = static_cast<int>(finalBuffer[y][x].r());
			int g = static_cast<int>(finalBuffer[y][x].g());
			int b = static_cast<int>(finalBuffer[y][x].b());
			std::cout << r << " " << g << " " << b << " ";
		}
		std::cout << "\n";
	}
 
	auto EndTime = std::chrono::steady_clock::now();
	std::cerr << "\n\aCompleted in : " << std::chrono::duration_cast<std::chrono::seconds>(EndTime - StartTime).count();

	std::cin.get();
}

HitableList GetWorld()
{
	HitableList World(make_shared<Sphere>(Vector3(0, 0, -1), 0.5, make_shared<Lambertian>(Vector3(0.7, 0.3, 0.3))));
	World.Add(make_shared<Sphere>(Vector3(0, -100.5, -1), 100, make_shared<Lambertian>(Vector3(0.8, 0.8, 0))));
	World.Add(make_shared<Sphere>(Vector3(1, 0, -1), 0.5, make_shared<Metal>(Vector3(0.8, 0.6, 0.2), 0.0)));
	World.Add(make_shared<Sphere>(Vector3(-1, 0, -1), 0.5, make_shared<Dielectric>(1.5)));
	return World;
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
		NormalizedY = Clamp(Remap(NormalizedY, 0.4, 0.6, 0, 1),0,1);
		Vector3 finalColor = (1 - NormalizedY)*Vector3(171, 169, 197) / 255 + NormalizedY * Vector3(225, 194, 183) / 255;
		return finalColor * finalColor;
	}
}

void GetChunkRange(int& StartValue, int& EndValue)
{
	StartValue = NextHeight;
	EndValue = (int)ffmax((NextHeight - ChunkSize + 1), 0);
	NextHeight -= ChunkSize;
	if (NextHeight < 0)
		isComplete = true;
}

void RenderImage(int ThreadIndex, HitableList& World, Camera& cam)
{
	int StartValue, EndValue;
	Vector3 color;

	ChunkRangeMutex.lock();
	GetChunkRange(StartValue, EndValue);
	ChunkRangeMutex.unlock();

	for (int y = StartValue; y >= EndValue; --y)
	{
		for (int x = 0; x < imageWidth; ++x)
		{		

			for (int s = 0; s < Samples; s++)
			{
				double U = (x + RandomDouble()) / (double)imageWidth;
				double V = (y + RandomDouble()) / (double)imageHeight;
				Ray ray = cam.GetRayAtUV(U, V);
				Vector3 SamplingColor = ColorAtRay(ray, World, MaxDepth);
				double red = SamplingColor.r();
				color += SamplingColor;
			}
			color /= (double)Samples;
			finalBuffer[y][x] = Vector3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b()));	
			finalBuffer[y][x].SetValues(255 * Clamp(finalBuffer[y][x].r(), 0, 0.999),
										255 * Clamp(finalBuffer[y][x].g(), 0, 0.999),
										255 * Clamp(finalBuffer[y][x].b(), 0, 0.999));			
		}		
	} 
}



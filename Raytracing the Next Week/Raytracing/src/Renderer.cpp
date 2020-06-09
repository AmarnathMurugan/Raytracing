#include "Headers/Cuboid.h"
#include "Headers/Sphere.h"
#include "Headers/MovableSphere.h"
#include "Headers/Camera.h"
#include "Headers/Material.h"
#include "Headers/bvh_node.h"
#include "Headers/Volume.h"
#include "Headers/rt_stb.h"
#include <thread>
#include <mutex>
#include <chrono>


#pragma region FUNCTION_SIGNATURES
	HitableList GetWorld();
	HitableList GetCornellBox();
	HitableList GetfinalScene();
	Vector3 ColorAtRay(const Ray& ray, HitableList& world, int depth, bool isUseSkybox=true);
	void GetChunkRange(int& Start, int& End);
	void RenderImage(int ThreadIndex, HitableList& World, Camera& cam);
#pragma endregion
	   
#pragma region PUBLIC_VARIABLES
	const int imageWidth = 192;
	const int imageHeight = 108;
	const int Samples = 100;
	const int MaxDepth = 60;

	const int NumberOfThreads = std::thread::hardware_concurrency();
	const int ChunkSize = imageHeight / NumberOfThreads;

	int NextHeight = imageHeight - 1;
	Vector3 finalBuffer[imageHeight][imageWidth];
	std::mutex ChunkRangeMutex;
	bool isComplete = false;

	const Vector3 BackgroundColor(0, 0, 0);
#pragma endregion
	   
int main()
{ 	
	std::cout << "P3\n";
	std::cout << imageWidth << " " << imageHeight << "\n255\n"; 

	Vector3 LookFrom(100,230, -1000);
	Vector3 LookAt(100, 230, 0);
	Vector3 ViewUp(0, 1, 0);
	double focalDistance = (LookFrom - LookAt).length();
	double aperture = 0.02;
	Camera cam(35,(imageWidth/(double)imageHeight),aperture,focalDistance,LookFrom,LookAt,ViewUp,0,1);
	
	//HitableList World = GetWorld();
	HitableList World = GetfinalScene();

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
	auto checTex = make_shared<CheckeredTexture>(make_shared<ConstantTexture>(Vector3(1, 1, 1)), make_shared<ConstantTexture>(Vector3(.3, .3, .3)));
	auto NoiseTexture = make_shared<PerlinTexture>(8,7,PerlinTexture::NoiseType::MarbleNoise);	
	auto LightMaterial = make_shared<DiffuseLight>(make_shared<ConstantTexture>(Vector3(4, 4, 4)));
	
	int nx, ny, nn;
	unsigned char* data = stbi_load("Textures\\earth.jpg",&nx,&ny,&nn,0);
	if (stbi_failure_reason()) std::cerr << stbi_failure_reason();
	auto imgTexture = make_shared<ImageTexture>(data, nx, ny,Vector3(0.25,0,0));

	HitableList World(make_shared<Sphere>(Vector3(0, -1000, 0), 1000, make_shared<Lambertian>(NoiseTexture)));
	World.Add(make_shared<Sphere>(Vector3(0, 2, 0), 2, make_shared<Lambertian>(imgTexture)));
	for(int i=-5; i<15;i++)
		for (int j = 0; j < 10; j++)
		{
			double MaterialProbability = RandomDouble();
			Vector3 center(i + RandomDouble()*0.9, 0.06, j + RandomDouble()*0.9);
			if(MaterialProbability < 0.6)
				World.Add(make_shared<Sphere>(center, 0.06, make_shared<Lambertian>(make_shared<ConstantTexture>(Vector3(RandomDouble(), RandomDouble(), RandomDouble())))));
			else if(MaterialProbability < 0.8)
				World.Add(make_shared<Sphere>(center, 0.06, make_shared<Metal>(make_shared<ConstantTexture>(Vector3(RandomDouble(), RandomDouble(), RandomDouble())),RandomDouble())));
			else
				World.Add(make_shared<Sphere>(center, 0.06, make_shared<Dielectric>(1.5)));
			
		}
	 
	return HitableList(make_shared<bvh_node>(World,0,1));
}

HitableList GetCornellBox()
{
	auto red = make_shared<Lambertian>(make_shared<ConstantTexture>(Vector3(0.65, 0.05, 0.05)));
	auto white = make_shared<Lambertian>(make_shared<ConstantTexture>(Vector3(0.73, 0.73, 0.73)));
	auto green = make_shared<Lambertian>(make_shared<ConstantTexture>(Vector3(0.12, 0.45, 0.15)));
	auto light = make_shared<DiffuseLight>(make_shared<ConstantTexture>(Vector3(6, 6, 6)));
	auto isoLight = make_shared<Isotropic>(make_shared<PerlinTexture>(.04,8, PerlinTexture::NoiseType::PerlinNoise), Vector3(0.5, 0.5, 0.5));
	auto isoDark = make_shared<Isotropic>(make_shared<ConstantTexture>(Vector3(0.7, 0.7, 0.7)));
 
	HitableList World(make_shared<RectYZ>(150, 350, 0, 200, 200, green, true));
	World.Add(make_shared<RectYZ>(150, 350, 0, 200, 0, red));
	World.Add(make_shared<RectXZ>(0, 200, 0, 200, 350, white,true));
	World.Add(make_shared<RectXZ>(0, 200, 0, 200, 150, white));
	World.Add(make_shared<RectXY>(0, 200, 150, 350, 200, white));
	World.Add(make_shared<RectXZ>(41, 160, 46, 156, 349, light, true));

	shared_ptr<Hitable> tallCubiod = make_shared<RotateY>(make_shared<Cuboid>(Vector3(-400, -400, -400), Vector3(400, 400, 400),white), -15);
	//tallCubiod = make_shared<Translate>(tallCubiod, Vector3(130,0,295));
	tallCubiod = make_shared<Volume>(isoLight, tallCubiod, 0.005,false);
	shared_ptr<Hitable> smolCuboid = make_shared<RotateY>(make_shared<Cuboid>(Vector3(0, 0, 0), Vector3(70, 70, 70), white), 18);
	smolCuboid = make_shared<Translate>(smolCuboid, Vector3(100, 0, 65));
	smolCuboid = make_shared<Volume>(isoDark, smolCuboid, 0.01);
	shared_ptr<Hitable> Stand = make_shared<Cuboid>(Vector3(-20, -800, -20), Vector3(220, -0.5, 200), white);
	World.Add(smolCuboid);
	//World.Add(tallCubiod);
	World.Add(Stand);
	//return HitableList(make_shared<bvh_node>(World, 0, 1));
	return World;
}

HitableList GetfinalScene()
{
	HitableList boxes;
	double x, x1, z, z1,y;
	auto white = make_shared<Lambertian>(make_shared<ConstantTexture>(Vector3(0.75, 0.75, 0.75)));
	for(int i=0;i<20;i++)
		for (int j = 0; j < 20; j++)
		{
			x = -400 + i * 50;
			z = -400 + j * 50;
			x1 = x + 50;
			z1 = z + 50;
			y = RandomDouble(10, 100);
			boxes.Add(make_shared<Cuboid>(Vector3(x, 0, z), Vector3(x1, y, z1), white));
		}
	HitableList Cornell = GetCornellBox();
	HitableList objs(make_shared<bvh_node>(boxes, 0, 1));
	objs.Add(make_shared<bvh_node>(Cornell,0,1));
	return objs;
}

Vector3 ColorAtRay(const Ray& ray, HitableList& world, int depth,  bool isUseSkybox)
{
	if (depth <= 0)
		return BackgroundColor;

	HitRecord hitRecord;
	if (world.isHit(ray, 0.001, DBL_MAX, hitRecord))
	{
		Vector3 Attenuation;
		Ray ScatteredRay;		 
		//if (hitRecord.HitPoint.y() < 270 && hitRecord.HitPoint.x()==555 && depth == MaxDepth) __debugbreak();
		Vector3 emmisionColor = hitRecord.mat_ptr->emit(hitRecord.U, hitRecord.V, hitRecord.HitPoint);
		if (hitRecord.mat_ptr->scatter(ray, hitRecord, Attenuation, ScatteredRay))
			return emmisionColor + Attenuation * ColorAtRay(ScatteredRay, world, depth - 1, isUseSkybox);
		else
			return emmisionColor;
	}
	else
	{
		if (!isUseSkybox)
			return BackgroundColor;
		Vector3 NormalizedDirection = ray.Ray_Direction().normalized();
		double NormalizedY = 0.5*(NormalizedDirection.y() + 1);		
		NormalizedY = Clamp(Remap(NormalizedY, 0.0, 0.6, 0, 1),0,1);
		Vector3 finalColor = (1 - NormalizedY)*Vector3(225, 194, 183)*0.8 / 255 + NormalizedY * Vector3(171, 169, 197)*0.3 / 255;
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
				//if (x == 20 && y == 60) __debugbreak();
				Ray ray = cam.GetRayAtUV(U, V);
				Vector3 SamplingColor = ColorAtRay(ray, World, MaxDepth,true);			
				double red = SamplingColor.r();
				color += SamplingColor;
			}
			color /= (double)Samples;
			finalBuffer[y][x] = Vector3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b()));	
			finalBuffer[y][x].SetValues(256 * Clamp(finalBuffer[y][x].r(), 0, .999),
										256 * Clamp(finalBuffer[y][x].g(), 0, .999),
										256 * Clamp(finalBuffer[y][x].b(), 0, .999));			
		}		
	} 
}



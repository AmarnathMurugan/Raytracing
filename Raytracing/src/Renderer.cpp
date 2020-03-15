//#include<iostream>
//#include "Headers/Vector3.h"
#include "Headers/Ray.h"

Vector3 ColorAtRay(const Ray& ray);
double isPointAtSphere(Vector3 center, double Radius, const Ray& ray);

int main()
{
	std::cout << "P3\n";
	int Width = 200;
	int Height = 100;
	Vector3 LowerLeftCorner(-2, -1, -1);
	Vector3 Horizontal(4, 0, 0);
	Vector3 Vertical(0, 2, 0);
	Vector3 Origin(0, 0, 0);
	std::cout << Width << " " << Height << "\n255\n";
	for (int y = Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < Width; x++)
		{
			float U = x / (float)Width;
			float V = y / (float)Height;
			Ray ray(Origin, LowerLeftCorner + U*Horizontal + V * Vertical);
			Vector3 color = ColorAtRay(ray);
			int r = (int)(255.99 * color.r());
			int g = (int)(255.99 * color.g());
			int b = (int)(255.99 * color.b());
			std::cout << r << " " << g << " " << b<<"  ";
			 
		}
		std::cout << std::endl;
	}


	std::cin.get();
}

Vector3 ColorAtRay(const Ray& ray)
{
	Vector3 SphereCenter(0, 0, -1);
	double SphereRadius = 0.5;
	double t = isPointAtSphere(SphereCenter, SphereRadius, ray);
	if (t >= 0)
	{
		Vector3 Normal = (ray.Point_On_Ray(t) - SphereCenter).normalized();
		return 0.5 * Vector3(Normal.r() + 1, Normal.g() + 1, Normal.b() + 1);
	}
	Vector3 NormalizedDirection = ray.Ray_Direction().normalized();
	double NormalizedY = 0.5*(NormalizedDirection.y() + 1);
	return (1 - NormalizedY)*Vector3(1.0, 1.0, 1.0) + NormalizedY * Vector3(.5, .7, 1.0);

}

double isPointAtSphere(Vector3 center, double Radius, const Ray& ray)
{
	Vector3 DirectionFromCenterToStart = ray.Start_Point() - center;
	double A = DotProduct(ray.Ray_Direction(), ray.Ray_Direction());
	double B = 2 * DotProduct(ray.Ray_Direction(), DirectionFromCenterToStart);
	double C = DotProduct(DirectionFromCenterToStart, DirectionFromCenterToStart) - (Radius * Radius);
	double Discriminant = (B*B) - 4 * A*C;
	if (Discriminant > 0)
		return (-B - sqrt(Discriminant)) / (2.0 * A);
	else
		return -1.0; // negative t implies that the object is behind camera
	 
}


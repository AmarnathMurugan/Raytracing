#ifndef UTILS_H_
#define UTILS_H_

#include "Vector3.h"
#include <functional>
#include <random>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

using std::shared_ptr;
using std::make_shared;

inline double RandomDouble()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
	//return ((float)rand() / RAND_MAX);
}

inline double RandomDouble(double min, double max)
{
	return min + (max - min)*RandomDouble();
}

Vector3 RandomPointInUnitSphere()
{
	Vector3 point;
	do
	{	
		point.SetValues(RandomDouble(-1,1), RandomDouble(-1, 1), RandomDouble(-1, 1));
	} while (point.SqrdLength() >= 1);
	return point;
}

Vector3 RandomPointInUnitCircle()
{
	 
	Vector3 point;
	do
	{
		point.SetValues(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
	} while (DotProduct(point, point) >= 1);
	return point;
}

double Clamp(double value, double min, double max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
#endif // !UTILS_H_


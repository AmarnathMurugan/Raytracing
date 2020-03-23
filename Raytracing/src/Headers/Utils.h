#ifndef UTILS_H_
#define UTILS_H_

#include "Vector3.h"

float rand01()
{
	return ((float)rand() / RAND_MAX);
}

Vector3 RandomPointInUnitSphere()
{
	Vector3 Offset(1, 1, 1);
	Vector3 point;
	do
	{
		point = 2.0 * Vector3(rand01(), rand01(), rand01()) - Offset;
	} while (point.SqrdLength() >= 1);
	return point;
}


#endif // !UTILS_H_


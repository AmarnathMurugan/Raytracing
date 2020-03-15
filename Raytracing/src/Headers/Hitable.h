#ifndef HITABLE_H_
#define HITABLE_H_

#include "Ray.h"

	struct HitRecord
	{
		float t;
		Vector3 HitPoint;
		Vector3 Normal;
	};

	class Hitable
	{
		public:
			virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) = 0;
	};

#endif // !HITABLE_H_


#ifndef HITABLE_H_
#define HITABLE_H_

#include "Ray.h"

	struct HitRecord
	{
		double t;
		Vector3 HitPoint;
		Vector3 Normal;
	};

	class Hitable
	{
		public:
			virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const = 0;
	};

#endif // !HITABLE_H_


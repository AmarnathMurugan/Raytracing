#ifndef HITABLE_H_
#define HITABLE_H_

#include "Ray.h"
#include "aabb.h"

	class Material;

	struct HitRecord
	{
		double t,U,V;
		Vector3 HitPoint;
		Vector3 Normal;
		bool isFrontFace;
		std::shared_ptr<Material> mat_ptr;

		inline void SetFaceNormal(const Ray& ray, const Vector3 OutwardNormal)
		{
			isFrontFace = DotProduct(ray.Ray_Direction(), OutwardNormal) < 0;
			Normal = isFrontFace ? OutwardNormal : -OutwardNormal;
		}
	};

	class Hitable
	{
		public:
			virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const = 0;
			virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const = 0;
	};

#endif // !HITABLE_H_


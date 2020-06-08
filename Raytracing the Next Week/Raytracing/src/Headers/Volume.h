#ifndef VOLUME_H
#define VOLUME_H

	#include "Material.h"

	class Volume : public Hitable
	{
	public:
		Volume(shared_ptr<Isotropic> mat, shared_ptr<Hitable> b, double density, bool isUniform = true) : phaseFunction(mat), Bounds(b), NegativeInverseDensity(-1/density), isUniformDistribution(isUniform) {}
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;
		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const
		{
			return Bounds->GetBoundingBox(time1, time2, OutputBox);
		}
		

	private:
		shared_ptr<Isotropic> phaseFunction;
		shared_ptr<Hitable> Bounds;
		double NegativeInverseDensity;
		bool isUniformDistribution;
	};
	
	bool Volume::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		
		HitRecord r1, r2;
		if (!Bounds->isHit(ray, -DBL_MAX, DBL_MAX, r1)) return false;
		if (!Bounds->isHit(ray, r1.t+0.0001, DBL_MAX, r2)) return false;
		
		if (r1.t < t_min) r1.t = t_min;
		if (r2.t > t_max) r2.t = t_max;
		if (r2.t <= r1.t) return false;
		if (r1.t < 0) r1.t = 0;
		double RayLength = ray.Ray_Direction().length();
		double LengthOfBounds = (r2.t - r1.t)*RayLength;
		double hitDistance = 0, OffsetDistance, densityValue;
		Vector3 hitPoint;
		do
		{
			hitDistance += log(1 - RandomDouble()) * NegativeInverseDensity;
			OffsetDistance = r1.t + hitDistance / RayLength;
			hitPoint = ray.Point_On_Ray(OffsetDistance);
			densityValue = phaseFunction->tex->Value(0, 0, hitPoint).x();
			densityValue *= densityValue;
			if (densityValue > RandomDouble()) break;
		} while (true && !isUniformDistribution);
		if (hitDistance > LengthOfBounds)
			return false;
		hitRecord.t = OffsetDistance;
		hitRecord.HitPoint = hitPoint;
		hitRecord.Normal = Vector3(1, 0, 0);
		hitRecord.isFrontFace = true;
		hitRecord.mat_ptr = phaseFunction;
		return true;
	}
 
#endif // !CONSTANTVOLUME_H


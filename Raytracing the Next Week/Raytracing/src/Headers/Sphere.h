#ifndef SPHERE_H_
#define SPHERE_H_

#include "Hitable.h"
 
	class Material;
		
	class Sphere : public Hitable
	{
	public:
		Vector3 Center;
		double Radius;
		shared_ptr<Material> mat;
		
		Sphere(Vector3 center, double radius, shared_ptr<Material> m) : Center(center), Radius(radius), mat(m) {}
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;
		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const;
 

	};
 

	bool Sphere::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		Vector3 DirectionFromCenterToStart = ray.Start_Point() - this->Center;
		double A = DotProduct(ray.Ray_Direction(), ray.Ray_Direction());
		double B =  DotProduct(ray.Ray_Direction(), DirectionFromCenterToStart);
		double C = DotProduct(DirectionFromCenterToStart, DirectionFromCenterToStart) - (Radius * Radius);
		double Discriminant = (B*B) - A*C; // 4 is in B squared, so when root is taken it'd be cancelled with the 2 in denominator
		if (Discriminant > 0)
		{
			double temp = (-B - sqrt(Discriminant)) / A;
			if (t_min < temp && temp < t_max)
			{
				hitRecord.t = temp;
				hitRecord.HitPoint = ray.Point_On_Ray(temp);
				Vector3 OutwardNormal = (hitRecord.HitPoint - this->Center) / Radius;
				hitRecord.SetFaceNormal(ray, OutwardNormal);
				hitRecord.mat_ptr = mat;
				GetSphereUVs((hitRecord.HitPoint - Center) / Radius, hitRecord.U, hitRecord.V);
				return true;
			}
			temp = (-B + sqrt(Discriminant)) / A;
			if (t_min < temp && temp < t_max)
			{
				hitRecord.t = temp;
				hitRecord.HitPoint = ray.Point_On_Ray(temp);
				Vector3 OutwardNormal = (hitRecord.HitPoint - this->Center) / Radius;
				hitRecord.SetFaceNormal(ray, OutwardNormal);
				hitRecord.mat_ptr = mat;
				GetSphereUVs((hitRecord.HitPoint - Center) / Radius, hitRecord.U, hitRecord.V);
				return true;
			}
		}	 
		return false; 
		
	}

	bool Sphere::GetBoundingBox(double time1, double time2, aabb& OutputBox) const
	{
		OutputBox = aabb(Center - Vector3(this->Radius, this->Radius, this->Radius), Center + Vector3(this->Radius, this->Radius, this->Radius));
		return true;
	}

	

#endif // !SPHERE_H_


#ifndef SPHERE_H_
#define SPHERE_H_

#include "Hitable.h"
 
	class Material;
		
	class Sphere : public Hitable
	{
	public:
		Vector3 Center;
		double Radius;
		Material* mat;
		
		Sphere(Vector3 center, double radius) : Center(center), Radius(radius) {}
		Sphere(Vector3 center, double radius, Material* m);
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;

 

	};
 

	inline Sphere::Sphere(Vector3 center, double radius, Material* m)
	{
		Center = center;
		Radius = radius;
		mat = m;
	}

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
				return true;
			}
		}	 
		return false; 
		
	}

	 

 

#endif // !SPHERE_H_


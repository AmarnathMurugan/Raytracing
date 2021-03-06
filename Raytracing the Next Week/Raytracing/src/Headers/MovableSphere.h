#ifndef MOVABLESPHERE_H_
#define MOVABLESPHERE_H_

#include "Hitable.h"

class Material;

class MovableSphere : public Hitable
{
public:
	Vector3 StartCenter,EndCenter;
	double Radius;
	std::shared_ptr<Material> mat;

public:
	MovableSphere(Vector3 StartPos,Vector3 EndPos, double radius, std::shared_ptr<Material> m) : StartCenter(StartPos), EndCenter(EndPos), Radius(radius), mat(m) {}
	virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;
	Vector3 GetCenterAtTime(double t) const;
	virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const;
};


bool MovableSphere::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
{
	Vector3 DirectionFromCenterToStart = ray.Start_Point() - GetCenterAtTime(ray.Time());
	double A = DotProduct(ray.Ray_Direction(), ray.Ray_Direction());
	double B = DotProduct(ray.Ray_Direction(), DirectionFromCenterToStart);
	double C = DotProduct(DirectionFromCenterToStart, DirectionFromCenterToStart) - (Radius * Radius);
	double Discriminant = (B*B) - A * C;  
	if (Discriminant > 0)
	{
		double temp = (-B - sqrt(Discriminant)) / A;
		if (t_min < temp && temp < t_max)
		{
			hitRecord.t = temp;
			hitRecord.HitPoint = ray.Point_On_Ray(temp);
			Vector3 OutwardNormal = (hitRecord.HitPoint - GetCenterAtTime(ray.Time())) / Radius;
			hitRecord.SetFaceNormal(ray, OutwardNormal);
			hitRecord.mat_ptr = mat;
			GetSphereUVs((hitRecord.HitPoint - GetCenterAtTime(ray.Time()))/Radius, hitRecord.U, hitRecord.V);
			return true;
		}
		temp = (-B + sqrt(Discriminant)) / A;
		if (t_min < temp && temp < t_max)
		{
			hitRecord.t = temp;
			hitRecord.HitPoint = ray.Point_On_Ray(temp);
			Vector3 OutwardNormal = (hitRecord.HitPoint - GetCenterAtTime(ray.Time())) / Radius;
			hitRecord.SetFaceNormal(ray, OutwardNormal);
			hitRecord.mat_ptr = mat;
			GetSphereUVs((hitRecord.HitPoint - GetCenterAtTime(ray.Time()))/Radius, hitRecord.U, hitRecord.V);
			return true;
		}
	}
	return false;

}

inline Vector3 MovableSphere::GetCenterAtTime(double t) const
{
	return ((1 - t)*StartCenter +( t * EndCenter));
}

bool MovableSphere::GetBoundingBox(double time1, double time2, aabb& OutputBox) const
{
	aabb box1(GetCenterAtTime(time1) - Vector3(this->Radius, this->Radius, this->Radius), GetCenterAtTime(time1) + Vector3(this->Radius, this->Radius, this->Radius));
	aabb box2(GetCenterAtTime(time2) - Vector3(this->Radius, this->Radius, this->Radius), GetCenterAtTime(time2) + Vector3(this->Radius, this->Radius, this->Radius));
	OutputBox = EnclosingBox(box1, box2);
	return true;
}

#endif // !MOVABLESPHERE_H_


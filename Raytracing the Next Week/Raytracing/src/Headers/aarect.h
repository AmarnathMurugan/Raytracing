#ifndef AARECT_H
#define AARECT_H

	#include "Material.h"	

	class RectXY : public Hitable
	{
	public:
		RectXY(double _x0, double _x1, double _y0, double _y1,double _z, shared_ptr<Material> m) :x0(_x0), x1(_x1), y0(_y0), y1(_y1),z(_z),mat(m) {}
 
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;		

		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const 
		{
			OutputBox = aabb(Vector3(x0, y0, z-0.001), Vector3(x1, y1, z+0.001));
			return true;
		}
	private:
		shared_ptr<Material> mat;
		double x0, x1, y0, y1, z;
	};

	bool RectXY::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		double t = (this->z - ray.Start_Point().z())/ray.Ray_Direction().z();
		if(t < t_min || t > t_max)
			return false;
		Vector3 pt = ray.Point_On_Ray(t);
		if (pt.x() < x0 || pt.x() > x1 || pt.y() < y0 || pt.y() > y1) 
			return false;
		hitRecord.t = t;
		hitRecord.HitPoint = pt;
		hitRecord.U = (pt.x() - x0) / (x1 - x0);
		hitRecord.V = (pt.y() - y0) / (y1 - y0);		
		hitRecord.SetFaceNormal(ray, Vector3(0, 0, -1));
		hitRecord.mat_ptr = this->mat;
		return true;
	}

#endif // !AARECT_H

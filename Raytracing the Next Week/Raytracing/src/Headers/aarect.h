#ifndef AARECT_H
#define AARECT_H

	#include "Material.h"	

	class RectXY : public Hitable
	{
	public:
		RectXY(double _x0, double _x1, double _y0, double _y1,double _z, shared_ptr<Material> m, bool _flipNormal = false) :x0(_x0), x1(_x1), y0(_y0), y1(_y1),z(_z),mat(m),flipNormal(_flipNormal) {}
 
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;		

		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const 
		{
			OutputBox = aabb(Vector3(x0, y0, z-0.001), Vector3(x1, y1, z+0.001));
			return true;
		}
	private:
		shared_ptr<Material> mat;
		double x0, x1, y0, y1, z;
		bool flipNormal;
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
		if(!flipNormal)
			hitRecord.SetFaceNormal(ray, Vector3(0, 0, -1));
		else
			hitRecord.SetFaceNormal(ray, Vector3(0, 0, 1));
		hitRecord.mat_ptr = this->mat;
		return true;
	}

	//=============== RectXZ ================

	class RectXZ : public Hitable
	{
	public:
		RectXZ(double _x0, double _x1, double _z0, double _z1, double _y, shared_ptr<Material> m, bool _flipNormal = false) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), y(_y), mat(m),flipNormal(_flipNormal) {}

		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;

		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const
		{
			OutputBox = aabb(Vector3(x0, y - 0.001, z0 ), Vector3(x1, y + 0.001, z1));
			return true;
		}
		 

	private:
		shared_ptr<Material> mat;
		double x0, x1, z0, z1, y;
		bool flipNormal = false;
	};	
 
	bool RectXZ::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		double t = (this->y - ray.Start_Point().y()) / ray.Ray_Direction().y();
		if (t < t_min || t > t_max)
			return false;
		Vector3 pt = ray.Point_On_Ray(t);
		if (pt.x() < x0 || pt.x() > x1 || pt.z() < z0 || pt.z() > z1)
			return false;
		hitRecord.t = t;
		hitRecord.HitPoint = pt;
		hitRecord.U = (pt.x() - x0) / (x1 - x0);
		hitRecord.V = (pt.y() - z0) / (z1 - z0);	
		if(!flipNormal)
			hitRecord.SetFaceNormal(ray, Vector3(0, 1, 0));
		else
			hitRecord.SetFaceNormal(ray, Vector3(0, -1, 0));
		hitRecord.mat_ptr = this->mat;
		return true;
	}

	//=============== RectYZ ================

	class RectYZ : public Hitable
	{
	public:
		RectYZ(double _y0, double _y1, double _z0, double _z1, double _x, shared_ptr<Material> m, bool _flipNormal = false) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), x(_x), mat(m),flipNormal(_flipNormal) {}

		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;

		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const
		{
			OutputBox = aabb(Vector3(x - 0.001,y0, z0), Vector3(x + 0.001,y1, z1));
			return true;
		}


	private:
		shared_ptr<Material> mat;
		double y0, y1, z0, z1, x;
		bool flipNormal;
	};

	bool RectYZ::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		double t = (this->x - ray.Start_Point().x()) / ray.Ray_Direction().x();
		if (t < t_min || t > t_max)
			return false;
		Vector3 pt = ray.Point_On_Ray(t);
		if (pt.y() < y0 || pt.y() > y1 || pt.z() < z0 || pt.z() > z1)
			return false;
		hitRecord.t = t;
		hitRecord.HitPoint = pt;
		hitRecord.U = (pt.y() - y0) / (y1 - y0);
		hitRecord.V = (pt.z() - z0) / (z1 - z0);
		if(!flipNormal)
			hitRecord.SetFaceNormal(ray, Vector3(1, 0, 0));
		else
			hitRecord.SetFaceNormal(ray, Vector3(-1, 0, 0));
		hitRecord.mat_ptr = this->mat;
		return true;
	}

#endif // !AARECT_H

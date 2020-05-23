#ifndef CUBOID_H
#define CUBIOD_H
	
	#include "Hitable_list.h"	
	#include "aarect.h"
	class Cuboid : public Hitable
	{
	public:
		Cuboid(Vector3 _min, Vector3 _max, shared_ptr<Material> mat);
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;

		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const
		{
			OutputBox = aabb(minPos,maxPos);
			return true;
		}

	private:
		Vector3 minPos, maxPos;
		shared_ptr<Material> mat;
		HitableList rects;
	};

	Cuboid::Cuboid(Vector3 _min, Vector3 _max, shared_ptr<Material> m)
	{
		minPos = _min;
		maxPos = _max;
		mat = m;
		rects.Add(make_shared<RectXZ>(minPos.x(), maxPos.x(), minPos.z(), maxPos.z(), minPos.y(), mat,true));
		rects.Add(make_shared<RectXY>(minPos.x(), maxPos.x(), minPos.y(), maxPos.y(), minPos.z(), mat));
		rects.Add(make_shared<RectYZ>(minPos.y(), maxPos.y(), minPos.z(), maxPos.z(), minPos.x(), mat));
		rects.Add(make_shared<RectXY>(minPos.x(), maxPos.x(), minPos.y(), maxPos.y(), maxPos.z(), mat,true));
		rects.Add(make_shared<RectYZ>(minPos.y(), maxPos.y(), minPos.z(), maxPos.z(), maxPos.x(), mat,true));
		rects.Add(make_shared<RectXZ>(minPos.x(), maxPos.x(), minPos.z(), maxPos.z(), maxPos.y(), mat));
	}

	inline bool Cuboid::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		return rects.isHit(ray, t_min, t_max, hitRecord);
	}
 

#endif // !CUBOID_H

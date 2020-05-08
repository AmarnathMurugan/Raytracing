#ifndef AABB_H
#define AABB_H

#include "Ray.h"
#include "Utils.h"
class aabb
{
public:
	aabb() {}
	aabb(const Vector3 &min, const Vector3 &max) :_min(min), _max(max) {}
	Vector3 min() const { return _min; }
	Vector3 max() const { return _max; }

	bool hit(const Ray &ray, double tmin, double tmax) const;
	

public:
	Vector3 _min, _max;
};

bool aabb::hit(const Ray &ray, double tmin, double tmax) const
{
	double denominator, minimum, maximum, t0, t1;
	for (int i = 0; i < 3; i++)
	{
		denominator = 1 / ray.Ray_Direction()[i];
		t0 = (this->min()[i] - ray.Start_Point()[i])*denominator;
		t1 = (this->max()[i] - ray.Start_Point()[i])*denominator;
		if (denominator < 0) std::swap(t0, t1);
		minimum = t0 > tmin ? t0 : tmin;
		maximum = t1 < tmax ? t1 : tmax;
		if (minimum >= maximum) return false;
	}
	return true;
}

aabb EnclosingBox(const aabb& box1, const aabb& box2)
{
	Vector3 minimumValues(ffmin(box1.min()[0], box2.min()[0]), ffmin(box1.min()[1], box2.min()[1]), ffmin(box1.min()[2], box2.min()[2]));
	Vector3 maximumValues(ffmin(box1.max()[0], box2.max()[0]), ffmin(box1.max()[1], box2.max()[1]), ffmin(box1.max()[2], box2.max()[2]));
	return aabb(minimumValues, maximumValues);
}


#endif // !AABB_H


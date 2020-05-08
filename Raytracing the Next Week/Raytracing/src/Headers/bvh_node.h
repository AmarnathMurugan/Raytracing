#ifndef BVH_NODE
#define BVH_NODE

#include <algorithm>
#include "Hitable_list.h"
#include "Utils.h"

inline bool box_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b, int axis);
bool box_x_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b);
bool box_y_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b);
bool box_z_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b);
class bvh_node : public Hitable
{
public:
	
	bvh_node(HitableList& list, double time1, double time2) : bvh_node(list.Objects,0,list.Objects.size(),time1,time2){}
	bvh_node(std::vector<std::shared_ptr<Hitable>> objs, int start, int end, double time1, double time2);

	virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;
	virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const;

private:
	std::shared_ptr<Hitable> left, right;
	aabb boundingBox;	
};

bvh_node::bvh_node(std::vector<std::shared_ptr<Hitable>> objs, int start, int end, double time1, double time2)
{
	int axis = (int)RandomDouble(0, 2);
	auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
	size_t span = end - start;

	if (span == 1)
	{
		left = right = objs[start];
	}
	else if (span == 2)
	{
		if (comparator(objs[start], objs[start + 1]))
		{
			left = objs[start];
			right = objs[start + 1];
		}
		else
		{
			left = objs[start+1];
			right = objs[start];
		}
	}
	else
	{
		std::sort(objs.begin() + start, objs.begin() + end, comparator);
		auto mid = start + span / 2;
		left = make_shared<bvh_node>(objs, start, mid, time1, time2);
		right = make_shared<bvh_node>(objs, mid,end, time1, time2);
	}

	aabb leftBox, rightBox;
	if (!left->GetBoundingBox(time1, time2, leftBox) || !right->GetBoundingBox(time1, time2, rightBox))
		std::cerr << "No bounding box in BVH constructor";
	boundingBox = EnclosingBox(leftBox, rightBox);
}

inline bool box_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b, int axis)
{
	aabb box_a, box_b;
	if (!a->GetBoundingBox(0, 0, box_a) || !b->GetBoundingBox(0, 0, box_b))
		std::cerr << "No bounding box in BVH constructor";
	return box_a.min().values[axis] < box_b.min().values[axis];
}

bool box_x_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b)
{
	return box_compare(a,b,0);
}

bool box_y_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b)
{
	return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b)
{
	return box_compare(a, b, 2);
}

bool bvh_node::GetBoundingBox(double time1, double time2, aabb & OutputBox) const
{
	OutputBox = boundingBox;
	return true;
}

inline bool bvh_node::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
{
	if(!boundingBox.hit(ray,t_min,t_max))
		return false;
	bool isLeft = left->isHit(ray, t_min, t_max,hitRecord);
	bool isRight = right->isHit(ray, t_min, isLeft?hitRecord.t:t_max, hitRecord);
	return true;
}

#endif // !BVH_NODE


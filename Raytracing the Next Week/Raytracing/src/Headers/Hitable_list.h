#ifndef HITABLE_LIST_H_
#define HITABLE_LIST_H_
	
#include "Hitable.h"	
#include<memory>
#include<vector>

using std::shared_ptr;
using std::make_shared;

	class HitableList : public Hitable
	{
	public:
		HitableList() {}
		HitableList(shared_ptr<Hitable> obj) { Add(obj); }
		void Add(shared_ptr<Hitable> obj) { Objects.push_back(obj); } //try emplace_back
		void Clear() { Objects.clear(); }

		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;
		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const ;
		

	public:
		std::vector<shared_ptr<Hitable>> Objects;
	};

	
 

	inline bool HitableList::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		HitRecord TempHitRecord;
		bool isAnythingHit = false;
		double ClosestValueYet = t_max;
		for (int i = 0; i < Objects.size(); i++)
		{
			if (Objects[i]->isHit(ray, t_min, ClosestValueYet, TempHitRecord))
			{
				isAnythingHit = true;
				ClosestValueYet = TempHitRecord.t;
				hitRecord = TempHitRecord;
			}
		}
		return isAnythingHit;
	}

	bool HitableList::GetBoundingBox(double time1, double time2, aabb& OutputBox) const
	{
		if (Objects.empty()) return false;
		aabb tempBox;
		bool firstTrue = Objects[0]->GetBoundingBox(time1, time2, tempBox);
		if (!firstTrue)
			return false;
		OutputBox = tempBox;
		for (const auto& obj : Objects)
		{
			if (!obj->GetBoundingBox(time1, time2, tempBox))
				return false;
			OutputBox = EnclosingBox(OutputBox, tempBox);
		}
		return true;
	}


#endif // !HITABLE_LIST_H_

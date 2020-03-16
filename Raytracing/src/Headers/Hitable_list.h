#ifndef HITABLE_LIST_H_
#define HITABLE_LIST_H_
	
	#include "Hitable.h"	

	class HitableList : Hitable
	{
	public:
		HitableList();
		HitableList(Hitable** list, int size);
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;
		Hitable** hList;
		int ListSize;

	private:

	};

	HitableList::HitableList()
	{

	}

	inline HitableList::HitableList(Hitable** list, int size)
	{
		hList = list;
		ListSize = size;
	}

	inline bool HitableList::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		HitRecord TempHitRecord;
		bool isAnythingHit = false;
		double ClosestValueYet = t_max;
		for (int i = 0; i < ListSize; i++)
		{
			if (hList[i]->isHit(ray, t_min, ClosestValueYet, TempHitRecord))
			{
				isAnythingHit = true;
				ClosestValueYet = TempHitRecord.t;
				hitRecord = TempHitRecord;
			}
		}
		return isAnythingHit;
	}



#endif // !HITABLE_LIST_H_

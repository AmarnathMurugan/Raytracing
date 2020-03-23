#ifndef RAY_H_
#define RAY_H_

#include "Vector3.h"

	class Ray
	{
		public:
			Ray();
			Ray(const Vector3& startPoint,const Vector3& rayDirection);
			Vector3 Start_Point() const { return StartPoint; }
			Vector3 Ray_Direction() const { return RayDirection; }
			Vector3 Point_On_Ray(double t) const;
		private:
			Vector3 StartPoint, RayDirection;
	};

	Ray::Ray()
	{
	}

	inline Ray::Ray(const Vector3& startPoint, const Vector3& rayDirection)
	{
		StartPoint = startPoint;
		RayDirection = rayDirection;
	}	 

	inline Vector3 Ray::Point_On_Ray(double t) const
	{
		return StartPoint + (t * RayDirection) ;
	}
	
#endif // !RAY_H_


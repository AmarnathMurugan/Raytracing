#ifndef CAMERA_H_
#define CAMERA_H_
	
#include "Ray.h"

	class Camera
	{
		public:
			Camera();
			
			Ray GetRayAtUV(float U, float V);
	
	private:	
		Vector3 Origin;
		Vector3 LowerLeftCorner;
		Vector3 Horizontal;
		Vector3 Vertical;

	};

	Camera::Camera()
	{
		LowerLeftCorner.SetValues(-2, -1, -1);
		Horizontal.SetValues(4, 0, 0);
		Vertical.SetValues(0, 2, 0);
		Origin.SetValues(0, 0, 0);
	}

	inline Ray Camera::GetRayAtUV(float U, float V)
	{
		return Ray(Origin, LowerLeftCorner + U * Horizontal + V * Vertical);
	}
 

#endif // !CAMERA_H_


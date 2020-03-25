#ifndef CAMERA_H_
#define CAMERA_H_
	
#include "Ray.h"
#include "Utils.h"

	class Camera
	{
		public:
			Camera(double vFov, double aspect, Vector3 LookFrom, Vector3 LookAt, Vector3 ViewUp);
			
			Ray GetRayAtUV(float U, float V);
	
	private:	
		Vector3 Origin;
		Vector3 LowerLeftCorner;
		Vector3 Horizontal;
		Vector3 Vertical;
		double VerticalFov;
		double AspectRatio;
	};

	Camera::Camera(double vFov, double aspect, Vector3 LookFrom, Vector3 LookAt, Vector3 ViewUp)
	{
		VerticalFov = vFov * M_PI/(double)180;
		AspectRatio = aspect;
		double halfHeight = tan(VerticalFov / 2);
		double halfWidth = AspectRatio * halfHeight;
		Origin = LookFrom;		
		Vector3 w = (LookFrom - LookAt).normalized();
		Vector3 u = Cross(ViewUp,w).normalized();
		Vector3 v = Cross(w, u);
		LowerLeftCorner = Origin - halfWidth * u - halfHeight * v - w;
		Horizontal = 2 * halfWidth * u;
		Vertical = 2 * halfHeight * v;
	}

	inline Ray Camera::GetRayAtUV(float U, float V)
	{
		return Ray(Origin, LowerLeftCorner + U * Horizontal + V * Vertical - Origin);
	}
 

#endif // !CAMERA_H_


#ifndef CAMERA_H_
#define CAMERA_H_
	
#include "Ray.h"
#include "Utils.h"

	class Camera
	{
		public:
			Camera(double vFov, double aspect,double aperture, double focalDistance, Vector3 LookFrom, Vector3 LookAt, Vector3 ViewUp);
			
			Ray GetRayAtUV(float U, float V);
	
	private:	
		Vector3 Origin;
		Vector3 LowerLeftCorner;
		Vector3 Horizontal;
		Vector3 Vertical;
		Vector3 w, u, v;
		double VerticalFov;
		double AspectRatio;
		double LensRadius;
	};

	Camera::Camera(double vFov, double aspect, double aperture, double focalDistance, Vector3 LookFrom, Vector3 LookAt, Vector3 ViewUp)
	{
		VerticalFov = vFov * M_PI/(double)180;
		AspectRatio = aspect;
		LensRadius = aperture / 2;
		double halfHeight = tan(VerticalFov / 2);
		double halfWidth = AspectRatio * halfHeight;
		Origin = LookFrom;		
		this->w = (LookFrom - LookAt).normalized();
		this->u = Cross(ViewUp,w).normalized();
		this->v = Cross(w, u);
		LowerLeftCorner = Origin - halfWidth * focalDistance * u - halfHeight * focalDistance * v - focalDistance * w;
		Horizontal = 2 * halfWidth * focalDistance * u;
		Vertical = 2 * halfHeight * focalDistance * v;
	}

	inline Ray Camera::GetRayAtUV(float U, float V)
	{
		Vector3 RandomPointInLens = this->LensRadius * RandomPointInUnitCircle();
		Vector3 Offset = RandomPointInLens.x() * this->u + RandomPointInLens.y() * this->v;
		return Ray(Origin + Offset, LowerLeftCorner + U * Horizontal + V * Vertical - Origin - Offset);
	}
 

#endif // !CAMERA_H_


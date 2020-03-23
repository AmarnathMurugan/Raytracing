#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Hitable.h"
#include "Utils.h"

	 
	class Material
	{
		public:
			virtual bool scatter(const Ray& SourceRay,const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) = 0;
	};

	class lambertian : public Material
	{
	public:
		
		lambertian(const Vector3& a) : Albedo(a) {}
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay);
		
		Vector3 Albedo;

	};


	bool lambertian::scatter(const Ray& SourceRay, const HitRecord & hitRecord, Vector3& attenuation, Ray & scatteredRay)
	{
		Vector3 target = hitRecord.HitPoint + hitRecord.Normal + RandomPointInUnitSphere();
		scatteredRay = Ray(hitRecord.HitPoint, (target - hitRecord.HitPoint));
		attenuation = this->Albedo;
		return true;
	}



	class Metal : public Material
	{
	public:
		
		Metal(const Vector3& a, float f) : Albedo(a) { if (f <= 1) fuzziness = f; else  fuzziness = 1; }
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay);

		Vector3 Albedo;
		float fuzziness;
	};
	 

	bool Metal::scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay)
	{
	 
		Vector3 ReflectedRay = ReflectVector(SourceRay.Ray_Direction() , hitRecord.Normal);
		scatteredRay = Ray(hitRecord.HitPoint, ReflectedRay+ this->fuzziness * RandomPointInUnitSphere());
		attenuation = this->Albedo;
		return (DotProduct(scatteredRay.Ray_Direction(), hitRecord.Normal) > 0);
	}


#endif // !MATERIAL_H_


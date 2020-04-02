#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Hitable.h"
#include "Utils.h"

	double GetReflectionProbability(double RefIndx, double Cos);

	//====== BASE CLASS =====

	class Material
	{
		public:
			virtual bool scatter(const Ray& SourceRay,const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const = 0;
	};


	//===== LAMBERTIAN ======

	class Lambertian : public Material
	{
	public:
		
		Lambertian(const Vector3& a) : Albedo(a) {}
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const;
		
		Vector3 Albedo;

	};


	bool Lambertian::scatter(const Ray& SourceRay, const HitRecord & hitRecord, Vector3& attenuation, Ray & scatteredRay) const
	{
		Vector3 target = hitRecord.HitPoint + hitRecord.Normal + RandomPointInUnitSphere();
		scatteredRay = Ray(hitRecord.HitPoint, (target - hitRecord.HitPoint));
		attenuation = this->Albedo;
		return true;
	}

	//====== METAL ======

	class Metal : public Material
	{
	public:
		
		Metal(const Vector3& a, double f) : Albedo(a) { if (f <= 1) fuzziness = f; else  fuzziness = 1; }
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const;

		Vector3 Albedo;
		double fuzziness;
	};
	 

	bool Metal::scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const 
	{
	 
		Vector3 ReflectedRay = ReflectVector(SourceRay.Ray_Direction() , hitRecord.Normal);
		scatteredRay = Ray(hitRecord.HitPoint, ReflectedRay+ this->fuzziness * RandomPointInUnitSphere());
		attenuation = this->Albedo;
		return (DotProduct(scatteredRay.Ray_Direction(), hitRecord.Normal) > 0);
	}

	//====== DIELECTRIC ======

	class Dielectric : public Material
	{
	public:
		Dielectric(double ref_indx) : RefractiveIndex(ref_indx) {}

		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const;
		bool RefractRay(const Vector3& IncidentRay, const double NiOverNt, const Vector3& OutwardNormal, Vector3& RefractedDirection) const;
		

		double RefractiveIndex;

	};

	bool Dielectric::scatter(const Ray & SourceRay, const HitRecord & hitRecord, Vector3 & attenuation, Ray & scatteredRay) const
	{
		Vector3 OutwardNormal , RefractedRayDirection;
		double NiOverNt , ReflectionProbability, Cosine;
		attenuation = Vector3(1, 1, 1);
		Vector3 reflectedDirection = ReflectVector(SourceRay.Ray_Direction(), hitRecord.Normal);

		//When entering glass
		if (DotProduct(SourceRay.Ray_Direction(), hitRecord.Normal) > 0)
		{
			OutwardNormal = -hitRecord.Normal;
			NiOverNt = this->RefractiveIndex;
			Cosine = this->RefractiveIndex * DotProduct(SourceRay.Ray_Direction(), hitRecord.Normal) / SourceRay.Ray_Direction().length();
		}
		//When exiting glass
		else
		{
			OutwardNormal = hitRecord.Normal;
			NiOverNt = 1 / this->RefractiveIndex;
			Cosine = -DotProduct(SourceRay.Ray_Direction(), hitRecord.Normal) / SourceRay.Ray_Direction().length();
		}
			
		if (RefractRay(SourceRay.Ray_Direction(), NiOverNt, OutwardNormal, RefractedRayDirection))
		{
			ReflectionProbability = GetReflectionProbability(RefractiveIndex, Cosine);			
		}
		else
		{		 
			ReflectionProbability = 1;
		}

		if (RandomDouble() < ReflectionProbability)
		{
			scatteredRay = Ray(hitRecord.HitPoint, reflectedDirection);
		}
		else
		{
			scatteredRay = Ray(hitRecord.HitPoint, RefractedRayDirection);
		}
		return true;
	}

	double GetReflectionProbability(double RefIndx,double Cos)
	{
		double r0 = (1 - RefIndx) / (1 + RefIndx);
		r0 *= r0;
		return r0 + (1 - r0) * pow((1 - Cos), 5);
	} 

	bool Dielectric::RefractRay(const Vector3& IncidentRay, const double NiOverNt, const Vector3& OutwardNormal, Vector3& RefractedDirection) const
	{
		Vector3 UnitIncidentRay = IncidentRay.normalized();
		double CosThetaIncident = DotProduct(UnitIncidentRay, OutwardNormal);
		double discriminant = 1.0 - NiOverNt * NiOverNt * (1 - CosThetaIncident * CosThetaIncident);
		if (discriminant > 0)
		{
			RefractedDirection = NiOverNt * (UnitIncidentRay - CosThetaIncident * OutwardNormal) - sqrt(discriminant) * OutwardNormal;
			return true;
		}
		else
			return false;
	}
	 

#endif // !MATERIAL_H_


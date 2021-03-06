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
		Vector3 target = hitRecord.HitPoint + hitRecord.Normal + RandomPointOnUnitSphere();
		scatteredRay = Ray(hitRecord.HitPoint, (target - hitRecord.HitPoint));
		attenuation = this->Albedo;
		return true;
	}

	//====== METAL ======

	class Metal : public Material
	{
	public:
		
		Metal(const Vector3& a, double f) : Albedo(a),fuzziness(f>1 ? 1:f) { }
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const;

		Vector3 Albedo;
		double fuzziness;
	};
	 

	bool Metal::scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const 
	{
	 
		Vector3 ReflectedRay = ReflectVector(SourceRay.Ray_Direction().normalized() , hitRecord.Normal);
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
		Vector3 RefractRay(const Vector3& IncidentRay, const double NiOverNt, const Vector3& OutwardNormal) const;
		

		double RefractiveIndex;

	};

	bool Dielectric::scatter(const Ray & SourceRay, const HitRecord & hitRecord, Vector3 & attenuation, Ray & scatteredRay) const
	{
		 
		
		attenuation = Vector3(1.0, 1.0, 1.0);

		double NiOverNt = (hitRecord.isFrontFace) ? (1 / RefractiveIndex) : RefractiveIndex;

		double CosThetaIncident = ffmin(DotProduct(-SourceRay.Ray_Direction().normalized(), hitRecord.Normal), 1.0);
		double SinThetaIncident = sqrt(1.0 - CosThetaIncident * CosThetaIncident);

		Vector3 reflectedDirection = ReflectVector(SourceRay.Ray_Direction().normalized(), hitRecord.Normal);
		
		if (NiOverNt*SinThetaIncident > 1.0)
		{
			scatteredRay = Ray(hitRecord.HitPoint, reflectedDirection);
			return true;
		}
	 
		double ReflectionProbability = GetReflectionProbability(RefractiveIndex, CosThetaIncident);	
		
		 
		if (RandomDouble() < ReflectionProbability)
		{
			scatteredRay = Ray(hitRecord.HitPoint, reflectedDirection);
			return true;
		}
		else
		{
			Vector3 RefractedRayDirection = RefractRay(SourceRay.Ray_Direction(), NiOverNt, hitRecord.Normal);
			scatteredRay = Ray(hitRecord.HitPoint, RefractedRayDirection);
			return true;
		}
	 
	}

	double GetReflectionProbability(double RefIndx,double Cos)
	{
		double r0 = (1 - RefIndx) / (1 + RefIndx);
		r0 *= r0;
		return r0 + (1 - r0) * pow((1 - Cos), 5);
	} 

	Vector3 Dielectric::RefractRay(const Vector3& IncidentRay, const double NiOverNt, const Vector3& OutwardNormal) const
	{
		Vector3 UnitIncidentRay = IncidentRay.normalized();
		double CosThetaIncident = DotProduct(-UnitIncidentRay, OutwardNormal);
		Vector3 Rparallel = NiOverNt * (UnitIncidentRay + CosThetaIncident * OutwardNormal);
		Vector3 Rperpendicular = -sqrt(1 - Rparallel.SqrdLength())*OutwardNormal;
		return  (Rparallel + Rperpendicular);
	}
	 

#endif // !MATERIAL_H_


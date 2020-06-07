#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Hitable.h"
#include "Utils.h"
#include "Texture.h"
	double GetReflectionProbability(double RefIndx, double Cos);

	//====== BASE CLASS =====

	class Material
	{
		public:
			virtual bool scatter(const Ray& SourceRay,const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const = 0;
			virtual Vector3 emit(double U, double V, Vector3 &p) const { return Vector3(0, 0, 0); }

		public:
			Vector3 TintColor;
	};


	//===== LAMBERTIAN ======

	class Lambertian : public Material
	{
	public:
		
		Lambertian(shared_ptr<Texture> a,const Vector3& tc = Vector3(1,1,1)) : Albedo(a) 
		{
			TintColor = tc;
		}
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const;
		
		shared_ptr<Texture> Albedo;
		
	};


	bool Lambertian::scatter(const Ray& SourceRay, const HitRecord & hitRecord, Vector3& attenuation, Ray & scatteredRay) const
	{
		Vector3 target = hitRecord.HitPoint + hitRecord.Normal + RandomPointOnUnitSphere();
		scatteredRay = Ray(hitRecord.HitPoint, (target - hitRecord.HitPoint),SourceRay.Time());
		attenuation = Albedo->Value(hitRecord.U,hitRecord.V,hitRecord.HitPoint) * TintColor;
		return true;
	}

	//====== METAL ======

	class Metal : public Material
	{
	public:
		
		Metal(shared_ptr<Texture> a, double f, const Vector3& tc = Vector3(1, 1, 1)) : Albedo(a),fuzziness(f>1 ? 1:f) { TintColor = tc; }
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const;

		shared_ptr<Texture> Albedo;
		double fuzziness;
	};
	 

	bool Metal::scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const 
	{
	 
		Vector3 ReflectedRay = ReflectVector(SourceRay.Ray_Direction().normalized() , hitRecord.Normal);
		scatteredRay = Ray(hitRecord.HitPoint, ReflectedRay+ this->fuzziness * RandomPointInUnitSphere(), SourceRay.Time());
		attenuation = Albedo->Value(hitRecord.U, hitRecord.V, hitRecord.HitPoint) * TintColor;
		return (DotProduct(scatteredRay.Ray_Direction(), hitRecord.Normal) > 0);
	}

	//====== DIELECTRIC ======

	class Dielectric : public Material
	{
	public:
		Dielectric(double ref_indx, const Vector3& tc = Vector3(1, 1, 1)) : RefractiveIndex(ref_indx) { TintColor = tc; }

		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const;
		Vector3 RefractRay(const Vector3& IncidentRay, const double NiOverNt, const Vector3& OutwardNormal) const;
		

		double RefractiveIndex;

	};

	bool Dielectric::scatter(const Ray & SourceRay, const HitRecord & hitRecord, Vector3 & attenuation, Ray & scatteredRay) const
	{
		 
		
		attenuation = Vector3(1.0, 1.0, 1.0) * TintColor;

		double NiOverNt = (hitRecord.isFrontFace) ? (1 / RefractiveIndex) : RefractiveIndex;

		double CosThetaIncident = ffmin(DotProduct(-SourceRay.Ray_Direction().normalized(), hitRecord.Normal), 1.0);
		double SinThetaIncident = sqrt(1.0 - CosThetaIncident * CosThetaIncident);

		Vector3 reflectedDirection = ReflectVector(SourceRay.Ray_Direction().normalized(), hitRecord.Normal);
		
		if (NiOverNt*SinThetaIncident > 1.0)
		{
			scatteredRay = Ray(hitRecord.HitPoint, reflectedDirection, SourceRay.Time());
			return true;
		}
	 
		double ReflectionProbability = GetReflectionProbability(RefractiveIndex, CosThetaIncident);	
		
		 
		if (RandomDouble() < ReflectionProbability)
		{
			scatteredRay = Ray(hitRecord.HitPoint, reflectedDirection, SourceRay.Time());
			return true;
		}
		else
		{
			Vector3 RefractedRayDirection = RefractRay(SourceRay.Ray_Direction(), NiOverNt, hitRecord.Normal);
			scatteredRay = Ray(hitRecord.HitPoint, RefractedRayDirection, SourceRay.Time());
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
	
	//====== DIFFUSE LIGHT ======

	class DiffuseLight : public Material
	{
	public:
		DiffuseLight(shared_ptr<Texture> tex) : EmmisionTexture(tex) {}
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const { return false; }
		virtual Vector3 emit(double U, double V, Vector3 &p) const
		{
			return EmmisionTexture->Value(U, V, p);
		}
	private:
		shared_ptr<Texture>  EmmisionTexture;
	};

	class Isotropic : public Material
	{
	public:
		Isotropic(shared_ptr<Texture> t, const Vector3& tc = Vector3(1, 1, 1)) : tex(t) { TintColor = tc; }
		virtual bool scatter(const Ray& SourceRay, const HitRecord& hitRecord, Vector3& attenuation, Ray& scatteredRay) const  
		{
			scatteredRay = Ray(hitRecord.HitPoint, RandomPointOnUnitSphere(), SourceRay.Time());
			attenuation = tex->Value(hitRecord.U, hitRecord.V, hitRecord.HitPoint) * TintColor;
			return true;
		}
	
	public:
		shared_ptr<Texture> tex;
	};
 

	 

#endif // !MATERIAL_H_


#ifndef HITABLE_H_
#define HITABLE_H_

#include "Ray.h"
#include "aabb.h"

	class Material;

	struct HitRecord
	{
		double t,U,V;
		Vector3 HitPoint;
		Vector3 Normal;
		bool isFrontFace;
		std::shared_ptr<Material> mat_ptr;

		inline void SetFaceNormal(const Ray& ray, const Vector3 OutwardNormal)
		{
			isFrontFace = DotProduct(ray.Ray_Direction(), OutwardNormal) < 0;
			Normal = isFrontFace ? OutwardNormal : -OutwardNormal;
		}
	};

	//====================== HITABLE ======================

	class Hitable
	{
		public:
			virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const = 0;
			virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const = 0;
	};

	//====================== TRANSLATE ======================

	class Translate : public Hitable
	{
	public:
		Translate(shared_ptr<Hitable> o, const Vector3& _offset) : obj(o), Offset(_offset) {}
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;
		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const;

	private:
		shared_ptr<Hitable> obj;
		Vector3 Offset;
	};

	bool Translate::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{
		Ray OffsetRay(ray.Start_Point() - Offset, ray.Ray_Direction(), ray.Time());
		if (!obj->isHit(OffsetRay, t_min, t_max, hitRecord))
			return false;
		hitRecord.HitPoint += Offset;
		hitRecord.SetFaceNormal(OffsetRay, hitRecord.Normal);
		return true;
	}

	bool Translate::GetBoundingBox(double time1, double time2, aabb & OutputBox) const
	{
		if (!obj->GetBoundingBox(time1, time2, OutputBox)) return false;
		OutputBox = aabb(OutputBox.min() + Offset, OutputBox.max() + Offset);
		return true;
	}
	
	//====================== ROTATE_Y ======================

	class RotateY : public Hitable
	{
	public:
		RotateY(shared_ptr<Hitable> o, double angle);
		virtual bool isHit(const Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const;
		virtual bool GetBoundingBox(double time1, double time2, aabb& OutputBox) const;

	public:
		shared_ptr<Hitable> obj;
		double theta, Cos, Sin;
		bool isHasBb;
		aabb boundingBox;
	};

	RotateY::RotateY(shared_ptr<Hitable> o, double angle)
	{
		obj = o;
		theta = Deg2Rads(angle);
		Sin = sin(theta);
		Cos = cos(theta);
		isHasBb = obj->GetBoundingBox(0, 1, boundingBox);
		if (isHasBb)
		{
			Vector3 min(DBL_MAX, DBL_MAX, DBL_MAX);
			Vector3 max(-DBL_MAX, -DBL_MAX, -DBL_MAX);

			for (int i = 0; i < 2; i++)
				for (int j = 0; j < 2; j++)
					for (int k = 0; k < 2; k++)
					{
						double x = (1 - i)*boundingBox.min().x() + i * boundingBox.max().x();
						double y = (1 - j)*boundingBox.min().y() + j * boundingBox.max().y();
						double z = (1 - k)*boundingBox.min().z() + k * boundingBox.max().z();

						double rotX = x * Cos + z * Sin;
						double rotZ = z * Cos - x * Sin;
						Vector3 rotPt(rotX, y, rotZ);
						for (int m = 0; m < 3; m++)
						{
							min[m] = ffmin(min[m], rotPt[m]);
							max[m] = ffmax(max[m], rotPt[m]);
						}
					}
			boundingBox = aabb(min, max);
		}
	}

	inline bool RotateY::GetBoundingBox(double time1, double time2, aabb & OutputBox) const
	{
		if (!isHasBb) return false;
		OutputBox = boundingBox;
		return true;
	}

	inline bool RotateY::isHit(const Ray & ray, double t_min, double t_max, HitRecord & hitRecord) const
	{

		double rotX = ray.Start_Point().x() * Cos - ray.Start_Point().z() * Sin;
		double rotZ = ray.Start_Point().z() * Cos + ray.Start_Point().x() * Sin;
		Vector3 RotStPt(rotX, ray.Start_Point().y(), rotZ);

		rotX = ray.Ray_Direction().x() * Cos - ray.Ray_Direction().z() * Sin;
		rotZ = ray.Ray_Direction().z() * Cos + ray.Ray_Direction().x() * Sin;
		Vector3 RotDir(rotX, ray.Ray_Direction().y(), rotZ);

		Ray rotRay(RotStPt, RotDir, ray.Time());

		if(!obj->isHit(rotRay,t_min,t_max,hitRecord))
			return false;

		hitRecord.HitPoint[0] = hitRecord.HitPoint.x() * Cos + hitRecord.HitPoint.z() * Sin;
		hitRecord.HitPoint[2] = hitRecord.HitPoint.z() * Cos - hitRecord.HitPoint.x() * Sin;

		hitRecord.Normal[0] = hitRecord.Normal.x() * Cos + hitRecord.Normal.z() * Sin;
		hitRecord.Normal[2] = hitRecord.Normal.z() * Cos - hitRecord.Normal.x() * Sin;

		hitRecord.SetFaceNormal(rotRay, hitRecord.Normal);

		return true;
	}

	
 

#endif // !HITABLE_H_


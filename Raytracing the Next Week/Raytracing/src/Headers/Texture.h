#ifndef TEXTURE_H
#define TEXTURE_H

	#include "Vector3.h"
	class Texture
	{
	public:
	
		virtual Vector3 Value(double U, double V,const Vector3& p) const = 0;

	};

	class ConstantTexture : public Texture
	{
	public:
		ConstantTexture(Vector3 c) : color(c) {}
		virtual Vector3 Value(double U, double V,const Vector3& p) const
		{
			return color;
		}

	public:
		Vector3 color;
	};
 
	class CheckeredTexture : public Texture
	{
		public:
			CheckeredTexture(shared_ptr<Texture> t1, shared_ptr<Texture> t2) : odd(t1), even(t2) {}

			virtual Vector3 Value(double U, double V, const Vector3& p) const
			{
				double sign = sin(20 * p.x())*sin(20 * p.y())*sin(20 * p.z());
				if (sign < 0)
					return odd->Value(U, V, p);
				else
					return even->Value(U, V, p);
			}
		
		public:
			shared_ptr<Texture> odd, even;
	};

	 
#endif // !TEXTURE_H


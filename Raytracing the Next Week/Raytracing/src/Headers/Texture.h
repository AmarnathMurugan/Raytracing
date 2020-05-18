#ifndef TEXTURE_H
#define TEXTURE_H

	
	#include "Perlin.h"
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

	class PerlinTexture : public Texture
	{
	public:
		enum NoiseType
		{
			PerlinNoise,TurbulentNoise,MarbleNoise
		};
		PerlinTexture(double scale = 1,double layers=5, NoiseType type=NoiseType::PerlinNoise) :Scale(scale),Layers(layers),noiseType(type) {}
		virtual Vector3 Value(double U, double V, const Vector3& p) const
		{ 	
			switch (noiseType)			
			{
				case NoiseType::PerlinNoise:
					return Vector3(1, 1, 1)*0.5*(1+ noise.PerlinNoise(Scale * p));
				case NoiseType::TurbulentNoise:
					return Vector3(1, 1, 1) * noise.turbulentNoise(Scale * p,Layers);		
				case NoiseType::MarbleNoise:
					return Vector3(1, 1, 1) * 0.5 * (1 + sin(Scale*p.z() + 10 * noise.turbulentNoise(p, Layers)));				
			}
		}

	private:
		Perlin noise;
		NoiseType noiseType;
		double Scale,Layers;
	};

	 

	 
#endif // !TEXTURE_H


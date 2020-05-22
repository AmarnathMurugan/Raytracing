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
			CheckeredTexture(shared_ptr<Texture> t1, shared_ptr<Texture> t2, double scale=20) : odd(t1), even(t2), Scale(scale) {}

			virtual Vector3 Value(double U, double V, const Vector3& p) const
			{
				double sign = sin(Scale * p.x()) * sin(Scale * p.y()) * sin(Scale * p.z());
				if (sign < 0)
					return odd->Value(U, V, p);
				else
					return even->Value(U, V, p);
			}
		
		private:
			shared_ptr<Texture> odd, even;
			double Scale;
	};

	class PerlinTexture : public Texture
	{
	public:
		enum NoiseType
		{
			PerlinNoise,TurbulentNoise,MarbleNoise
		};
		PerlinTexture(double scale = 1,double layers=5, NoiseType type=NoiseType::PerlinNoise, Vector3 c = Vector3(1,1,1)) :Scale(scale),Layers(layers),noiseType(type), Color(c) {}
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
				default: return Color * 0.5 * (1 + noise.PerlinNoise(Scale * p));
			}
		}

	private:
		Perlin noise;
		NoiseType noiseType;
		Vector3 Color;
		double Scale,Layers;
	};

	class ImageTexture: public Texture
	{
	public:
		ImageTexture(unsigned char* imgData, int x, int y,const Vector3& offset = Vector3(0,0,0)) :pixels(imgData), size_x(x), size_y(y), Offset(offset) {}
		virtual Vector3 Value(double U, double V, const Vector3& p) const
		{
			if (pixels == nullptr) return Vector3(1, 0, 1);
			int i = static_cast<int>((U+Offset.x())*size_x) % size_x;
			int j = static_cast<int>((1-V+Offset.y())*size_y) % size_y;
			if (i < 0) i = 0;
			if (j < 0) j = 0;
			if (i > size_x-1) i = size_x-1;
			if (j > size_y-1) j = size_y-1;

			double r = static_cast<int>(pixels[j * 3 * size_x + i * 3 + 0]) / 255.0;
			double g = static_cast<int>(pixels[j * 3 * size_x + i * 3 + 1]) / 255.0;
			double b = static_cast<int>(pixels[j * 3 * size_x + i * 3 + 2]) / 255.0;

			return Vector3(r, g, b);
		}
	private:
		unsigned char* pixels;
		int size_x, size_y;
		Vector3 Offset;
	};
 
	 

	 
#endif // !TEXTURE_H


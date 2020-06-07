#ifndef PERLIN_H
#define PERLIN_H

	#include "Vector3.h"
	#include "Utils.h"
	class Perlin
	{
	public:
		Perlin();
		~Perlin();
		double PerlinNoise(const Vector3& p) const;
		double turbulentNoise(const Vector3 &p, double layers) const;
	private:
		static const int RandCount = 256;
		static const int mask = RandCount-1;
		Vector3 *randVectors;
		int *perm_x,*perm_y,*perm_z;

		int* GetPermValues();
	};

	Perlin::Perlin()
	{
		randVectors = new Vector3[RandCount];
		for (int i = 0; i < RandCount; i++)
		{
			randVectors[i] = RandomPointOnUnitSphere();
		}
		perm_x = GetPermValues();
		perm_y = GetPermValues();
		perm_z = GetPermValues();
	}

	Perlin::~Perlin()
	{
		delete[] randVectors;
		delete[] perm_x;
		delete[] perm_y;
		delete[] perm_z;
	}

	double Perlin::PerlinNoise(const Vector3 &p) const
	{		 

		int x = (int)(floor(p.x()));
		int y = (int)(floor(p.y()));
		int z = (int)(floor(p.z()));
		double u = (p.x() - floor(p.x()));
		double v = (p.y() - floor(p.y()));
		double w = (p.z() - floor(p.z()));
		double t1 = SmoothStep(u);
		double t2 = SmoothStep(v);
		double t3 = SmoothStep(w);

		Vector3 cornerValues[2][2][2];
		double dotPrdtValues[2][2][2];

		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
				{
					int index = perm_x[(x + i) & mask] ^ perm_y[(y + j) & mask] ^ perm_z[(z + k) & mask];
					auto temp = randVectors[index];
					cornerValues[i][j][k] = temp;
					dotPrdtValues[i][j][k] = DotProduct(cornerValues[i][j][k], Vector3(i-u, j-v, k-w));
				}

		double accumulatedValue = 0;

		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
				{
					accumulatedValue += (t1 * i + (1-t1) * (1 - i)) * 
										(t2 * j + (1-t2) * (1 - j)) * 
										(t3 * k + (1-t3) * (1 - k)) * dotPrdtValues[i][j][k];
				}	
		return accumulatedValue;
	}

	double Perlin::turbulentNoise(const Vector3 &p, double layers) const
	{
		double accumulatedValue = 0;
		double frequency = 1;
		double scale = 1;
		for (size_t i = 0; i < layers; i++)
		{
			accumulatedValue += PerlinNoise(p * frequency) * scale;
			scale /= 2;
			frequency *= 2;
		}
		return fabs(accumulatedValue);
	}

	int* Perlin::GetPermValues()
	{
		int *arr = new int[RandCount];
		for (int i = 0; i < RandCount; i++)
		{
			int temp = RandomInt(0,i);
			if (temp != i)
			{
				arr[i] = arr[temp];
				arr[temp] = i;
			}
			else
				arr[i] = i;
		}
		return arr;
	}
#endif // !PERLIN_H


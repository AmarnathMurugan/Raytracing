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
	private:
		static const int pointCount = 256;
		static const int mask = pointCount-1;
		double *randvalues;
		int *perm_x,*perm_y,*perm_z;

		int* GetPermValues();
	};

	Perlin::Perlin()
	{
		randvalues = new double[pointCount];
		for (int i = 0; i < pointCount; i++)
		{
			randvalues[i] = RandomDouble();
		}
		perm_x = GetPermValues();
		perm_y = GetPermValues();
		perm_z = GetPermValues();
	}

	Perlin::~Perlin()
	{
		delete[] randvalues;
		delete[] perm_x;
		delete[] perm_y;
		delete[] perm_z;
	}

	double Perlin::PerlinNoise(const Vector3 &p) const
	{
		int x = ((int)(10*p.x())) & mask;
		int y = ((int)(10*p.y())) & mask;
		int z = ((int)(10*p.z())) & mask;
		return randvalues[perm_x[x]^perm_y[y]^perm_z[z]];
	}

	int* Perlin::GetPermValues()
	{
		int *arr = new int[pointCount];
		for (int i = 0; i < pointCount; i++)
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


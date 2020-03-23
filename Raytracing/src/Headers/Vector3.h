#ifndef VECTOR3_H_
#define VECTOR3_H_

#include<iostream>	


	class Vector3
	{
		public:

			Vector3();
			Vector3(double x, double y, double z);
			inline void SetValues(double x, double y, double z);

			inline double x() const { return values[0]; }
			inline double y() const { return values[1]; }
			inline double z() const { return values[2]; }
			inline double r() const { return values[0]; }
			inline double g() const { return values[1]; }
			inline double b() const { return values[2]; }

			inline const Vector3& operator +() const { return *this; }
			inline Vector3 operator -() const { return Vector3(-values[0], -values[1], -values[2]); }
			inline double operator [](int i) const { return values[i]; }
			inline double& operator [](int i) { return values[i]; }

			inline Vector3& operator +=(const Vector3& v2);
			inline Vector3& operator -=(const Vector3& v2);
			inline Vector3& operator *=(const float t);
			inline Vector3& operator /=(const float t);

			inline double length() const;
			inline double SqrdLength() const;
			inline Vector3 normalized() const;
			inline void normalize();

			inline double dot(const Vector3& v2) const;		 
			inline Vector3 Cross(const Vector3& v2) const;		 




			friend inline std::istream& operator >>(std::istream &is, Vector3 & v1);


	private:
			double values[3];

	};

	//CONSTRUCTORS   

	Vector3::Vector3()
	{
		values[0] = 0;
		values[1] = 0;
		values[2] = 0;
	}

	Vector3::Vector3(double x, double y, double z)
	{
		values[0] = x;
		values[1] = y;
		values[2] = z;
	}

	inline void Vector3::SetValues(double x, double y, double z)
	{
		values[0] = x;
		values[1] = y;
		values[2] = z;
	}
 

	//VECTOR DETAILS

	inline double Vector3::length() const
	{
		return sqrt((this->values[0] * this->values[0] + this->values[1] * this->values[1] + this->values[2] * this->values[2]));
	}

	inline double Vector3::SqrdLength() const
	{
		return ((this->values[0] * this->values[0] + this->values[1] * this->values[1] + this->values[2] * this->values[2]));
	}

	inline Vector3 Vector3::normalized() const
	{
		double len = this->length();
		return Vector3(this->values[0] / len, this->values[1] / len, this->values[2] / len);
	}

	inline void Vector3::normalize()
	{
		double len = this->length();
		for (int i = 0; i < 3; i++)
			this->values[i] /= len;
	}

	
	
	// VECTOR ARITHMETIC

	inline Vector3  operator +(const Vector3 &v1, const Vector3 &v2)
	{
		return Vector3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
	}


	inline Vector3  operator -(const Vector3 &v1, const Vector3 &v2)
	{
		return Vector3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
	}


	inline Vector3  operator *(const Vector3 &v1, double t)
	{
		return Vector3(v1.x() *t , v1.y()*t, v1.z() *t);
	}

	inline Vector3  operator *( double t, const Vector3 &v1)
	{
		return Vector3(v1.x() *t, v1.y()*t, v1.z() *t);
	}

	inline Vector3  operator *(const Vector3 &v1, const Vector3 &v2)
	{
		return Vector3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
	}


	inline Vector3  operator /(const Vector3 &v1, double t)
	{
		return Vector3(v1.x() /t, v1.y()/t, v1.z() /t);
	}

	inline Vector3  operator /(double t, const Vector3 &v1)
	{
		return Vector3(v1.x() /t, v1.y()/t, v1.z()/t);
	}

	inline Vector3& Vector3::operator+=(const Vector3 &v2)
	{
		values[0] += v2.x();
		values[1] += v2.y();
		values[2] += v2.z();
		return *this;
	}

	inline Vector3 & Vector3::operator-=(const Vector3 & v2)
	{
		values[0] += v2.x();
		values[1] += v2.y();
		values[2] += v2.z();
		return *this;
	}

	inline Vector3 & Vector3::operator*=(const float t)
	{
		values[0] *= t;
		values[1] *= t;
		values[2] *= t;
		return *this;
	}

	inline Vector3 & Vector3::operator/=(const float t)
	{
		values[0] /= t;
		values[1] /= t;
		values[2] /= t;
		return *this;
	}



	//STREAM OVERLOADS

	inline std::istream& operator>>(std::istream & is, Vector3 & v1)
	{
		is >> v1.values[0] >> v1.values[1] >> v1.values[2];
		return is;
	}

	inline std::ostream& operator<<(std::ostream & os, const Vector3 & v1)
	{
		os << "(" << v1.x() << "," << v1.y() << "," << v1.z() << ")";
		return os;
	}

	//VECTOR - VECTOR OPERATIONS

	inline double Vector3::dot(const Vector3 & v2) const
	{
		return (this->x()*v2.x() + this->y()*v2.y() + this->z()*v2.z());
	}

	inline double  DotProduct(const Vector3 & v1, const Vector3 & v2)
	{
		return (v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z());
	}



	inline Vector3  Cross(const Vector3 & v1, const Vector3 & v2)
	{
		return Vector3((v1.y()*v2.z() - v1.z()*v2.y()),
			-(v1.x()*v2.z() - v1.z()*v2.x()),
			(v1.x()*v2.y() - v1.y()*v2.x()));
	}

	inline Vector3 Vector3::Cross(const Vector3 & v2) const
	{
		return Vector3((this->y()*v2.z() - this->z()*v2.y()),
			-(this->x()*v2.z() - this->z()*v2.x()),
			(this->x()*v2.y() - this->y()*v2.x()));
	}


	Vector3 ReflectVector(const Vector3& SrcVec, const Vector3& Normal)
	{
		return SrcVec - (2 * DotProduct(SrcVec, Normal)) * Normal;
	}

#endif // !VECTOR3_H_




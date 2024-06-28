#pragma once
#include <cmath>

class MyVec3
{
public:

	float x;
	float y;
	float z;

	//Constructor

	MyVec3() = default;
	MyVec3(float x, float y, float z) :x(x), y(y), z(z) {}
	MyVec3(const MyVec3& v) :x(v.x), y(v.y), z(v.z) {}

	//Operator

	MyVec3& operator = (const MyVec3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	bool operator==(const MyVec3& v) const { return (x == v.x && y == v.y && z == v.z); }

	MyVec3 operator+(const MyVec3& v) const { return MyVec3(x + v.x, y + v.y, z + v.z); }
	MyVec3 operator-(const MyVec3& v) const { return MyVec3(x - v.x, y - v.y, z - v.z); }
	MyVec3 operator*(float k) const { return MyVec3(x * k, y * k, z * k); }
	MyVec3 operator/(float k) const { return MyVec3(x / k, y / k, z / k); }
	float operator*(const MyVec3& v) const { return (x * v.x) + (y * v.y) + (z * v.z); }

	float Lenght() const { return std::sqrtf((*this) * (*this)); }
	MyVec3 Normalize() const {
		const float leng = Lenght();
		return leng == 0 ? *this : *this / leng;
	}

};
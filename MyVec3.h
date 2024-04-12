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

	//Function

	/*MyVec3 Add(const MyVec3& v1, const MyVec3& v2);
	MyVec3 Subtract(const MyVec3& v1, const MyVec3& v2);
	MyVec3 Multiply(float scalar, const MyVec3& v);
	float Dot(const MyVec3& v1, const MyVec3& v2);
	float Length(const MyVec3& v);
	MyVec3 Norm(const MyVec3& v);*/

};
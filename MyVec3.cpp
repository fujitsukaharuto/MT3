#include "MyVec3.h"
#include <cmath>

//MyVec3 MyVec3::Add(const MyVec3& v1, const MyVec3& v2)
//{
//	MyVec3 result{};
//
//	result.x = v1.x + v2.x;
//	result.y = v1.y + v2.y;
//	result.z = v1.z + v2.z;
//
//	return result;
//}
//
//MyVec3 MyVec3::Subtract(const MyVec3& v1, const MyVec3& v2)
//{
//	MyVec3 result{};
//
//	result.x = v1.x - v2.x;
//	result.y = v1.y - v2.y;
//	result.z = v1.z - v2.z;
//
//	return result;
//}
//
//MyVec3 MyVec3::Multiply(float scalar, const MyVec3& v)
//{
//	MyVec3 result{};
//
//	result.x = v.x * scalar;
//	result.y = v.y * scalar;
//	result.z = v.z * scalar;
//
//	return result;
//}
//
//float MyVec3::Dot(const MyVec3& v1, const MyVec3& v2)
//{
//	float result;
//
//	result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
//
//	return result;
//}
//
//float MyVec3::Length(const MyVec3& v)
//{
//	float result;
//
//	result = std::sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
//
//	return result;
//}
//
//MyVec3 MyVec3::Norm(const MyVec3& v)
//{
//	MyVec3 result = v;
//	const float leng = Length(v);
//	if (leng!=0)
//	{
//		result.x = v.x / leng;
//		result.x = v.y / leng;
//		result.x = v.z / leng;
//	}
//	return result;
//}

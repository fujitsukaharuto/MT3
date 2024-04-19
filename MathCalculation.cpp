#include "MathCalculation.h"

MyVec2 LengthDis(const MyVec2& endVec, const MyVec2& startVec)
{
	MyVec2 result{};
	result.x = endVec.x - startVec.x;
	result.y = endVec.y - startVec.y;

	return result;
}

float Length(const MyVec2& one, const MyVec2& two)
{
	MyVec2 result{};
	float length = 0;

	result = LengthDis(one, two);

	length = sqrtf(powf(result.x, 2) + powf(result.y, 2));

	return length;
}

bool IsCircleHit(const MyVec2& one, const MyVec2& two, const float& radius1, const float& radius2)
{
	float leng = Length(one, two);
	float rad = radius1 + radius2;
	if (leng < rad)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Lerp(float start, float end, float t)
{
	return (1 - t) * start + t * end;
}

MyVec2 Lerp(MyVec2 start, MyVec2 end, float t)
{
	MyVec2 result{};

	result.x = (1 - t) * start.x + t * end.x;
	result.y = (1 - t) * start.y + t * end.y;

	return result;
}

//Quad QuadPos(const MyVec2& pos, const float& radius)
//{
//	Quad result{};
//	result.lt = { pos.x - radius,pos.y + radius };
//	result.rt = { pos.x + radius,result.lt.y };
//	result.lb = { result.lt.x,pos.y - radius };
//	result.rb = { result.rt.x,result.lb.y };
//	return result;
//}

MyVec3 Cross(const MyVec3& v1, const MyVec3& v2)
{
	MyVec3 result;

	result = { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };

	return result;
}

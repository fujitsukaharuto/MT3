#include "MathCalculation.h"

float Clampf(float x, float min, float max)
{
	return x < min ? min : (x > max ? max : x);
}

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

MyVec3 Project(const MyVec3& v1, const MyVec3& v2)
{
	MyVec3 normVec = v2.Normalize();

	float dot = v1 * normVec;

	return normVec * dot;
}

MyVec3 ClosestPoint(const MyVec3& point, const Segument& segument)
{
	MyVec3 t = point - segument.origin;
	t = Project(t, segument.diff);

	MyVec3 result = segument.origin + t;

	return result;
}

MyVec3 Perpendicular(const MyVec3& vector)
{
	if (vector.x!=0.0f||vector.y!=0.0f)
	{
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

MyVec3 Lerp(const MyVec3& v1, const MyVec3& v2, float t)
{
	MyVec3 result;
	result = v1 + (v2 - v1) * t;
	return result;
}

MyVec3 Sleap(const MyVec3& v1, const MyVec3& v2, float t)
{
	if (v1 == v2) {
		return v1;
	}
	float dot = v1 * v2;
	dot = std::max(-1.0f, std::min(1.0f, dot));
	float theta = std::acosf(dot);

	float sinTheta = std::sinf(theta);
	MyVec3 newVec = v1;
	if (sinTheta != 0.0f) {
		float w1 = std::sinf((1 - t) * theta) / sinTheta;
		float w2 = std::sinf(t * theta) / sinTheta;
		newVec = v1 * w1 + v2 * w2;
	}

	float length1 = v1.Lenght();
	float length2 = v2.Lenght();

	float lenrth = Lerp(length1, length2, t);

	return newVec * lenrth;
}

float Clamp(float x, float min, float max)
{
	return x < min ? min : (x > max ? max : x);
}

MyVec3 CatmullRomPoint(const MyVec3& p0, const MyVec3& p1, const MyVec3& p2, const MyVec3& p3, float t)
{

	const float s = 0.5f;

	float t2 = t * t;
	float t3 = t2 * t;

	MyVec3 e3 = (p0 * (-1)) + (p1 * 3) - (p2 * 3) + p3;
	MyVec3 e2 = p0 * 2 - (p1 * 5) + p2 * 4 + (p3 * (-1));
	MyVec3 e1 = (p0 * (-1)) + p2;
	MyVec3 e0 = p1 * 2;

	return (e3 * t3 + e2 * t2 + e1 * t + e0) * s;
}

MyVec3 CatmullRom(const std::vector<MyVec3>& control, float t)
{
	assert(control.size() >= 4);

	size_t division = control.size() - 1;
	float areaWidth = 1.0f / division;

	float t_2 = std::fmod(t, areaWidth) * division;
	t_2 = Clamp(t_2, 0.0f, 1.0f);

	size_t index = static_cast<size_t>(t / areaWidth);
	index = ((std::min)(index, control.size() - 2));

	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	if (index == 0) {
		index0 = index1;
	}
	if (index3 >= control.size()) {
		index3 = index2;
	}
	const MyVec3& p0 = control[index0];
	const MyVec3& p1 = control[index1];
	const MyVec3& p2 = control[index2];
	const MyVec3& p3 = control[index3];
	return CatmullRomPoint(p0, p1, p2, p3, t_2);
}

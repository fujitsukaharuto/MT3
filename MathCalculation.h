#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <vector>
#include <assert.h>

#include "MyVec2.h"
#include "MyVec3.h"

struct Line
{
	MyVec3 origin;
	MyVec3 diff;
};

struct Ray
{
	MyVec3 origin;
	MyVec3 diff;
};

struct Segument
{
	MyVec3 origin;
	MyVec3 diff;
};

struct Plane
{
	MyVec3 normal;
	float distance;
};

struct Triangle
{
	MyVec3 vertices[3];
};

struct AABB
{
	MyVec3 min;
	MyVec3 max;
};

struct OBB
{
	MyVec3 center;
	MyVec3 orientations[3];
	MyVec3 size;
};

float Clampf(float x, float min, float max);

/// <summary>
/// 2次元ベクトルの2点の距離
/// </summary>
MyVec2 LengthDis(const MyVec2& endVec, const MyVec2& startVec);

/// <summary>
/// 2次元ベクトルの2点の距離(当たり判定用)
/// </summary>
float Length(const MyVec2& one, const MyVec2& two);

/// <summary>
/// 円との当たり判定
/// </summary>
bool IsCircleHit(const MyVec2& one, const MyVec2& two,const float& radius1, const float& radius2);

/// <summary>
/// 線形補間(float)
/// </summary>
float Lerp(float start, float end, float t);

/// <summary>
/// 線形補間(Vector2)
/// </summary>
MyVec2 Lerp(MyVec2 start, MyVec2 end, float t);

///// <summary>
///// Quadの４頂点求める
///// </summary>
//Quad QuadPos(const MyVec2& pos, const float& radius);

MyVec3 Cross(const MyVec3& v1, const MyVec3& v2);

MyVec3 Project(const MyVec3& v1, const MyVec3& v2);

MyVec3 ClosestPoint(const MyVec3& point, const Segument& segument);

MyVec3 Perpendicular(const MyVec3& vector);

MyVec3 Lerp(const MyVec3& v1, const MyVec3& v2, float t);

MyVec3 Sleap(const MyVec3& v1, const MyVec3& v2, float t);

float Clamp(float x, float min, float max);

MyVec3 CatmullRomPoint(const MyVec3& p0, const MyVec3& p1, const MyVec3& p2, const MyVec3& p3, float t);

MyVec3 CatmullRom(const std::vector<MyVec3>& control, float t);
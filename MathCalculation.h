#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

#include "MyVec2.h"
#include "MyVec3.h"

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

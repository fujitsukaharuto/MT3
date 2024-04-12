#include <Novice.h>
#include "MyVec3.h"

const char kWindowTitle[] = "LE2A_15_フジツカ_ハルト_MT3";

static const int kCol = 60;
void VectorPrint(int x, int y, const MyVec3& vec, const char* lab);

MyVec3 Add(const MyVec3& v1, const MyVec3& v2);
MyVec3 Subtract(const MyVec3& v1, const MyVec3& v2);
MyVec3 Multiply(float scalar, const MyVec3& v);
float Dot(const MyVec3& v1, const MyVec3& v2);
float Length(const MyVec3& v);
MyVec3 Norm(const MyVec3& v);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	MyVec3 v1 = { 1.0f,3.0f,-5.0f };
	MyVec3 v2 = { 4.0f,-1.0f,2.0f };
	float k = 4.0f;

	MyVec3 resultAdd = Add(v1, v2);
	MyVec3 resultSub = Subtract(v1, v2);
	MyVec3 resultMulti = Multiply(k, v1);
	float resultDot = Dot(v1, v2);
	float resultLeng = Length(v1);
	MyVec3 resultNorm = Norm(v2);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorPrint(0, 0, resultAdd, " : Add");
		VectorPrint(0, kCol, resultSub, " : Sub");
		VectorPrint(0, kCol * 2, resultMulti, " : Multiply");
		Novice::ScreenPrintf(0, kCol * 3, "%.02f : Dot", resultDot);
		Novice::ScreenPrintf(0, kCol * 4, "%.02f : Length", resultLeng);
		VectorPrint(0, kCol * 5, resultNorm, " : Normalize");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void VectorPrint(int x, int y, const MyVec3& vec, const char* lab)
{
	Novice::ScreenPrintf(x, y, "%.02f", vec.x);
	Novice::ScreenPrintf(x + kCol, y, "%.02f", vec.y);
	Novice::ScreenPrintf(x + kCol * 2, y, "%.02f", vec.z);
	Novice::ScreenPrintf(x + kCol * 3, y, "%s", lab);
}

MyVec3 Add(const MyVec3& v1, const MyVec3& v2)
{
	MyVec3 result{};

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

MyVec3 Subtract(const MyVec3& v1, const MyVec3& v2)
{
	MyVec3 result{};

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

MyVec3 Multiply(float scalar, const MyVec3& v)
{
	MyVec3 result{};

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;

	return result;
}

float Dot(const MyVec3& v1, const MyVec3& v2)
{
	float result;

	result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	return result;
}

float Length(const MyVec3& v)
{
	float result;

	result = std::sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	return result;
}

MyVec3 Norm(const MyVec3& v)
{
	MyVec3 result = v;
	const float leng = Length(v);
	if (leng != 0)
	{
		result.x = v.x / leng;
		result.y = v.y / leng;
		result.z = v.z / leng;
	}
	return result;
}
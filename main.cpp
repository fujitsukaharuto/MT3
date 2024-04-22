#include <Novice.h>
#include "Vector2Matrix.h"
#include "MatrixCalculation.h"
#include "MathCalculation.h"

const char kWindowTitle[] = "LE2A_15_フジツカ_ハルト_MT3";

static const int kRow = 20;
static const int kCol = 60;
void VectorPrint(int x, int y, const MyVec3& vec, const char* lab);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& mat);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	MyVec3 v1(1.2f, -3.9f, 2.5f);
	MyVec3 v2(2.8f, 0.4f, -1.3f);
	MyVec3 cross = Cross(v1, v2);

	MyVec3 rotate{};
	MyVec3 translate{};

	MyVec3 cameraPosition{ 0.0f,0.0f,-30.0f };
	MyVec3 cameraDir{ 0.0f,0.0f,1.0f };
	MyVec3 kLocalVer[3]{
		{0.0f,1.0f,0.0f},{1.0f,-1.0f,0.0f},{-1.0f,-1.0f,0.0f}
	};

	MyVec3 ndcVer[3];

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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

		rotate.y += 0.02f;
		const float kSpeed = 0.2f;
		if (keys[DIK_W])
		{
			translate.z += kSpeed;
		}
		else if (keys[DIK_S])
		{
			translate.z -= kSpeed;
		}
		if (keys[DIK_A])
		{
			translate.x -= kSpeed;
		}
		else if (keys[DIK_D])
		{
			translate.x += kSpeed;
		}
		if (keys[DIK_UP])
		{
			translate.y += kSpeed;
		}

		Matrix4x4 worldMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMat=MakeAffineMatrix({ 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, cameraPosition);
		Matrix4x4 viewMat = Inverse(cameraMat);
		Matrix4x4 projectionMat = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.f);
		Matrix4x4 worldViewProjectionMat = Multiply(viewMat, projectionMat);
		worldViewProjectionMat = Multiply(worldMat, worldViewProjectionMat);

		Matrix4x4 viewportMat= MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);
		MyVec3 screenVertices[3];
		for (uint32_t i = 0; i < 3; i++)
		{
			ndcVer[i] = Transform(kLocalVer[i], worldViewProjectionMat);
			screenVertices[i] = Transform(ndcVer[i], viewportMat);
		}
		MyVec3 crossv1 = ndcVer[1] - ndcVer[0];
		MyVec3 crossv2 = ndcVer[2] - ndcVer[1];
		MyVec3 crossdir = Cross(crossv1, crossv2);
		float dot = cameraDir * crossdir;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorPrint(0, 0, cross, "Cross");
		VectorPrint(0, kRow, rotate, "Rotate");
		VectorPrint(0, kRow*2, translate, "Trans");
		if (dot<0)
		{
			Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);
		}
		
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

void MatrixScreenPrintf(int x, int y, const Matrix4x4& mat)
{
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			Novice::ScreenPrintf(
				x + column * kCol, y + row * kRow, "%6.02f", mat.m[row][column]);
		}
	}
}

#include <Novice.h>
#include "Vector2Matrix.h"
#include "MatrixCalculation.h"

const char kWindowTitle[] = "LE2A_15_フジツカ_ハルト_MT3";

static const int kRow = 20;
static const int kCol = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& mat);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Matrix4x4 m1 =
	{ 3.2f,0.7f,9.6f,4.4f,
	5.5f,1.3f,7.8f,2.1f,
	6.9f,8.0f,2.6f,1.0f,
	0.5f,7.2f,5.1f,3.3f };

	Matrix4x4 m2 =
	{ 4.1f,6.5f,3.3f,2.2f,
	8.8f,0.6f,9.9f,7.7f,
	1.1f,5.5f,6.6f,0.0f,
	3.3f,9.9f,8.8f,2.2f };

	Matrix4x4 resultAdd = Add(m1, m2);
	Matrix4x4 resulMulti = Multiply(m1, m2);
	Matrix4x4 resultSub = Sub(m1, m2);
	Matrix4x4 inverseM1 = Inverse(m1);
	Matrix4x4 inverseM2 = Inverse(m2);
	Matrix4x4 transM1 = Transpose(m1);
	Matrix4x4 transM2 = Transpose(m2);
	Matrix4x4 identity = MakeIdentity4x4();


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



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, resultAdd);
		MatrixScreenPrintf(0, kRow * 5, resultSub);
		MatrixScreenPrintf(0, kRow * 5 * 2, resulMulti);
		MatrixScreenPrintf(0, kRow * 5 * 3, inverseM1);
		MatrixScreenPrintf(0, kRow * 5 * 4, inverseM2);
		MatrixScreenPrintf(kCol * 5, 0, transM1);
		MatrixScreenPrintf(kCol * 5, kRow * 5, transM2);
		MatrixScreenPrintf(kCol * 5, kRow * 5 * 2, identity);

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

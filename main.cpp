#include <Novice.h>
#include "Vector2Matrix.h"
#include "MatrixCalculation.h"
#include "MathCalculation.h"
#include "imgui.h"
#include <cmath>

const char kWindowTitle[] = "LE2A_15_フジツカ_ハルト_MT3";

static const int kRow = 20;
static const int kCol = 60;
void VectorPrint(int x, int y, const MyVec3& vec, const char* lab);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& mat);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProject, const Matrix4x4& viewPort, uint32_t color);
void DrawGrid(const Matrix4x4& viewProject, const Matrix4x4& viewport);
bool IsCollision(const Sphere& s1, const Sphere& s2);
bool IsCollision(const Sphere& sphere, const Plane& plane);
bool IsCollision(const Segument& segument, const Plane& plane);
bool IsCollision(const Triangle& triangle, const Segument& segument);
void DrawPlane(const Plane& plane, const Matrix4x4& vieProMat, const Matrix4x4& port, uint32_t color);
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjection, Matrix4x4& viewport, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	MyVec3 rotate{ 0.0f,0.0f,0.0f };
	MyVec3 translate{ 0.0f,0.0f,0.0f };

	MyVec3 cameraPosition{ 0.0f,0.0f,-10.49f };
	MyVec3 camerarota{ 0.58f,-4.61f,0.0f };
	MyVec3 cameraDir{ 0.0f,0.0f,1.0f };



	Plane plane{};
	plane.normal = { 1.0f,1.0f,0.0f };
	plane.distance = 1.0f;

	uint32_t color = 0xffffffff;

	Segument segument{};
	segument.origin = { -0.56f,0.0f,0.0f };
	segument.diff = { 1.0f,0.5f,0.2f };
	/*float lenght = 1.0f;*/

	Triangle triangle = { MyVec3{-1.0f,0.0f,0.0f},MyVec3{0.0f,1.0f,0.0f},MyVec3{1.0f,0.0f,0.0f} };

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

#ifdef _DEBUG

		ImGui::Begin("Sphere,Camera");
		ImGui::DragFloat3("cameraTrans", &cameraPosition.x, 0.01f);
		ImGui::DragFloat2("cameraRotate", &camerarota.x, 0.01f);

		ImGui::DragFloat3("segument origin", &segument.origin.x, 0.01f);
		ImGui::DragFloat3("segument diff", &segument.diff.x, 0.01f);
		/*lenght = MyVec3(segument.diff - segument.origin).Lenght();
		ImGui::DragFloat("length", &lenght, 0.01f, 0.0f);*/
		
		ImGui::DragFloat3("Triangle v1", &triangle.vertices[0].x,0.01f);
		ImGui::DragFloat3("Triangle v2", &triangle.vertices[1].x,0.01f);
		ImGui::DragFloat3("Triangle v3", &triangle.vertices[2].x,0.01f);

		/*ImGui::DragFloat3("Plane Normal", &plane.normal.x, 0.01f);
		plane.normal = plane.normal.Normalize();
		ImGui::DragFloat("Plane Dis", &plane.distance, 0.01f);*/
		ImGui::End();

#endif // _DEBUG

		if (keys[DIK_LEFT])
		{
			camerarota.y += 0.01f;
		}
		if (keys[DIK_RIGHT])
		{
			camerarota.y -= 0.01f;
		}
		if (keys[DIK_DOWN])
		{
			camerarota.x -= 0.01f;
		}
		if (keys[DIK_UP])
		{
			camerarota.x += 0.01f;
		}

		Matrix4x4 worldMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, cameraPosition);
		cameraMat = Multiply(cameraMat, MakeRotateXMatrix(camerarota.x));
		cameraMat = Multiply(cameraMat, MakeRotateYMatrix(camerarota.y));
		Matrix4x4 viewMat = Inverse(cameraMat);
		Matrix4x4 projectionMat = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.f);
		Matrix4x4 viewProject= Multiply(viewMat, projectionMat);
		//Matrix4x4 worldViewProjectionMat = Multiply(viewMat, projectionMat);
		//worldViewProjectionMat = Multiply(worldMat, worldViewProjectionMat);

		Matrix4x4 viewportMat= MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);

		MyVec3 originPoint = Transform(Transform(segument.origin, viewProject), viewportMat);
		MyVec3 diffPoint = Transform(Transform((segument.origin+segument.diff), viewProject), viewportMat);

		if (IsCollision(triangle, segument))
		{
			color = RED;
		}
		else
		{
			color = WHITE;
		}
		
		

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		DrawGrid(viewProject, viewportMat);

		
		Novice::DrawLine(int(originPoint.x), int(originPoint.y),
			int(diffPoint.x), int(diffPoint.y), color);
		DrawTriangle(triangle, viewProject, viewportMat, WHITE);



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

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProject, const Matrix4x4& viewPort, uint32_t color)
{
	const uint32_t kSubdivision = 16;
	const float kLonEvery = (static_cast<float>(M_PI) * 2.0f) / static_cast<float>(kSubdivision);
	const float kLatEvery = static_cast<float>(M_PI) / static_cast<float>(kSubdivision);

	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++)
	{
		float lat = -(static_cast<float>(M_PI)) / 2.0f + kLatEvery * latIndex;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++)
		{
			float lon = lonIndex * kLonEvery;
			MyVec3 a, b, c;
			a = { (std::cosf(lat) * std::cosf(lon)),std::sinf(lat) ,(std::cosf(lat) * std::sinf(lon))  };
			a = a * sphere.radius;
			b = { (std::cosf(lat + kLatEvery) * std::cosf(lon)),std::sinf(lat + kLatEvery) ,(std::cosf(lat + kLatEvery) * std::sinf(lon)) };
			b = b * sphere.radius;
			c = { (std::cosf(lat) * std::cosf(lon + kLonEvery)),std::sinf(lat) ,(std::cosf(lat) * std::sinf(lon + kLonEvery))  };
			c = c * sphere.radius;

			Matrix4x4 worldMata = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, a);
			Matrix4x4 worldMatb = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, b);
			Matrix4x4 worldMatc = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, c);

			Matrix4x4 worldViewProjectionMata = Multiply(worldMata, viewProject);
			Matrix4x4 worldViewProjectionMatb = Multiply(worldMatb, viewProject);
			Matrix4x4 worldViewProjectionMatc = Multiply(worldMatc, viewProject);
		
			MyVec3 ndcVera = Transform(MyVec3{ sphere.ceneter.x,sphere.ceneter.y,sphere.ceneter.z }, worldViewProjectionMata);
			MyVec3 ndcVerb = Transform(MyVec3{ sphere.ceneter.x,sphere.ceneter.y,sphere.ceneter.z }, worldViewProjectionMatb);
			MyVec3 ndcVerc = Transform(MyVec3{ sphere.ceneter.x,sphere.ceneter.y,sphere.ceneter.z }, worldViewProjectionMatc);

			MyVec3 screenVera = Transform(ndcVera, viewPort);
			MyVec3 screenVerb = Transform(ndcVerb, viewPort);
			MyVec3 screenVerc = Transform(ndcVerc, viewPort);

			Novice::DrawLine(int(screenVera.x), int(screenVera.y), int(screenVerb.x), int(screenVerb.y), color);
			Novice::DrawLine(int(screenVera.x), int(screenVera.y), int(screenVerc.x), int(screenVerc.y), color);
		}
	}

}

void DrawGrid(const Matrix4x4& viewProject, const Matrix4x4& viewport)
{

	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; xIndex++)
	{
		MyVec3 st, end;
		st = { (-kGridHalfWidth + kGridEvery * xIndex),0.0f,-kGridHalfWidth };
		end = { (-kGridHalfWidth + kGridEvery * xIndex),0.0f,kGridHalfWidth };
		
		Matrix4x4 worldMata = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, st);
		Matrix4x4 worldMatb = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, end);
		
		Matrix4x4 worldViewProjectionMata = Multiply(worldMata, viewProject);
		Matrix4x4 worldViewProjectionMatb = Multiply(worldMatb, viewProject);

		MyVec3 ndcVera = Transform(MyVec3{ 0.0f,0.0f,0.0f }, worldViewProjectionMata);
		MyVec3 ndcVerb = Transform(MyVec3{ 0.0f,0.0f,0.0f }, worldViewProjectionMatb);
		
		MyVec3 screenVera = Transform(ndcVera, viewport);
		MyVec3 screenVerb = Transform(ndcVerb, viewport);

		if (xIndex==5)
		{
			Novice::DrawLine(int(screenVera.x), int(screenVera.y),
				int(screenVerb.x), int(screenVerb.y), RED);
		}
		else
		{
			Novice::DrawLine(int(screenVera.x), int(screenVera.y),
				int(screenVerb.x), int(screenVerb.y), WHITE);
		}
		
	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; zIndex++)
	{
		MyVec3 st, end;
		st = { -kGridHalfWidth,0.0f,(-kGridHalfWidth + kGridEvery * zIndex) };
		end = { kGridHalfWidth,0.0f,(-kGridHalfWidth + kGridEvery * zIndex) };
		
		Matrix4x4 worldMata = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, st);
		Matrix4x4 worldMatb = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, end);

		Matrix4x4 worldViewProjectionMata = Multiply(worldMata, viewProject);
		Matrix4x4 worldViewProjectionMatb = Multiply(worldMatb, viewProject);

		MyVec3 ndcVera = Transform(MyVec3{ 0.0f,0.0f,0.0f }, worldViewProjectionMata);
		MyVec3 ndcVerb = Transform(MyVec3{ 0.0f,0.0f,0.0f }, worldViewProjectionMatb);

		MyVec3 screenVera = Transform(ndcVera, viewport);
		MyVec3 screenVerb = Transform(ndcVerb, viewport);
		
		if (zIndex==5)
		{
			Novice::DrawLine(int(screenVera.x), int(screenVera.y),
				int(screenVerb.x), int(screenVerb.y), RED);
		}
		else {
			Novice::DrawLine(int(screenVera.x), int(screenVera.y),
				int(screenVerb.x), int(screenVerb.y), WHITE);
		}
	}
}

bool IsCollision(const Sphere& s1, const Sphere& s2)
{
	MyVec3 dis = s2.ceneter - s1.ceneter;
	float leng = dis.Lenght();
	if (leng < (s1.radius + s2.radius))
	{
		return true;
	}

	return false;
}

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	float dot = plane.normal * sphere.ceneter;
	float k = dot - plane.distance;
	k = std::sqrtf(k * k);

	if (k<sphere.radius)
	{
		return true;
	}

	return false;
}

bool IsCollision(const Segument& segument, const Plane& plane)
{
	float dot = segument.diff * plane.normal;
	if (dot==0.0f)
	{
		return false;
	}
	float t = plane.distance - segument.origin * plane.normal;
	t = t / dot;
	if (0 <= t && t <= 1)
	{
		return true;
	}
	return false;
}

bool IsCollision(const Triangle& triangle, const Segument& segument)
{
	Plane plate;
	plate.normal = Cross((triangle.vertices[1] - triangle.vertices[0]), (triangle.vertices[2] - triangle.vertices[1]));
	plate.normal = plate.normal.Normalize();
	plate.distance = triangle.vertices[0] * plate.normal;
	float dot = segument.diff * plate.normal;
	if (dot == 0.0f)
	{
		return false;
	}
	float t = plate.distance - segument.origin * plate.normal;
	t = t / dot;
	if (0 <= t && t <= 1)
	{
		MyVec3 p = segument.origin + (segument.diff * t);
		MyVec3 cross01 = Cross((triangle.vertices[1] - triangle.vertices[0]), (p - triangle.vertices[1]));
		MyVec3 cross12 = Cross((triangle.vertices[2] - triangle.vertices[1]), (p - triangle.vertices[2]));
		MyVec3 cross20 = Cross((triangle.vertices[0] - triangle.vertices[2]), (p - triangle.vertices[0]));
		if ((cross01 * plate.normal) >= 0.0f &&
			(cross12 * plate.normal) >= 0.0f &&
			(cross20 * plate.normal) >= 0.0f)
		{
			return true;
		}
	}
	return false;
}

void DrawPlane(const Plane& plane, const Matrix4x4& vieProMat, const Matrix4x4& viewport, uint32_t color)
{
	MyVec3 center = plane.normal * plane.distance;
	MyVec3 perpendiculars[4];
	perpendiculars[0] = Perpendicular(plane.normal);
	perpendiculars[0] = perpendiculars[0].Normalize();

	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };

	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);

	perpendiculars[3]= { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	MyVec3 points[4];
	for (int32_t index = 0; index < 4; ++index)
	{
		MyVec3 extend = perpendiculars[index] * 2.0f;
		MyVec3 point = center + extend;
		points[index] = Transform(Transform(point, vieProMat), viewport);
	}


	Novice::DrawLine(int(points[1].x), int(points[1].y),
		int(points[2].x), int(points[2].y), color);

	Novice::DrawLine(int(points[1].x), int(points[1].y),
		int(points[3].x), int(points[3].y), color);

	Novice::DrawLine(int(points[0].x), int(points[0].y),
		int(points[2].x), int(points[2].y), color);

	Novice::DrawLine(int(points[3].x), int(points[3].y),
		int(points[0].x), int(points[0].y), color);

}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjection, Matrix4x4& viewport, uint32_t color)
{
	MyVec3 points[3];
	points[0] = Transform(Transform(triangle.vertices[0], viewProjection), viewport);
	points[1] = Transform(Transform(triangle.vertices[1], viewProjection), viewport);
	points[2] = Transform(Transform(triangle.vertices[2], viewProjection), viewport);

	Novice::DrawTriangle(int(points[0].x), int(points[0].y),
		int(points[1].x), int(points[1].y),
		int(points[2].x), int(points[2].y), color, kFillModeWireFrame);
}

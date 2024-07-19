#include <Novice.h>
#include "Vector2Matrix.h"
#include "MatrixCalculation.h"
#include "MathCalculation.h"
#include "imgui.h"
#include <cmath>
#include <algorithm>

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
bool IsCollision(const AABB& aabb1, const AABB& aabb2);
bool IsCollision(const AABB& aabb, const Sphere& sphere);
bool IsCollision(const AABB& aabb, const Segument& segument);
bool IsCollision(const OBB& obb, const Sphere& sphere);
bool IsCollision(const OBB& obb, const Segument& segu);
bool IsCollision(const OBB& obb1, const OBB& obb2);
void DrawPlane(const Plane& plane, const Matrix4x4& vieProMat, const Matrix4x4& port, uint32_t color);
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjection, Matrix4x4& viewport, uint32_t color);
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjection, const Matrix4x4 viewPort, uint32_t color);
void DrawOBB(const OBB& obb, const Matrix4x4& viewprojection, const Matrix4x4& viewPort, uint32_t color);
void DrawBezier(const std::vector<MyVec3> control, const Matrix4x4& viewProjection, const Matrix4x4 viewPort, uint32_t color);
void DrawCatmullRom(const std::vector<MyVec3> control, const Matrix4x4& viewProjection, const Matrix4x4 viewPort, uint32_t color);

Matrix4x4 operator*(const Matrix4x4 a, const Matrix4x4 b) { return Multiply(a, b); }

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);


	/*MyVec3 rotate{ 0.0f,0.0f,0.0f };
	MyVec3 translate{ 0.0f,0.0f,0.0f };*/


	//カメラ
	MyVec3 cameraPosition{ 0.0f,0.0f,-10.49f };
	MyVec3 camerarota{ 0.5f,0.0f,0.0f };
	MyVec3 cameraDir{ 0.0f,0.0f,1.0f };

	
	MyVec3 originCenetr{ 0.0f,0.0f,0.0f };
	const MyVec3 kGravity{ 0.0f,-9.8f,0.0f };
	float deltaTime = 1.0f / 60.0f;


	//バネ
	/*Spring spring{};
	spring.anchor = { 0.0f,1.0f,0.0f };
	spring.naturalLength = 0.7f;
	spring.stiffness = 100.0f;
	spring.dempingCoefficient = 2.0f;*/


	//ボール
	Ball ball{};
	ball.position = { 0.8f,1.2f,0.3f };
	ball.mass = 2.0f;
	ball.aceleration = { 0.0f,-9.8f,0.0f };
	ball.radius = 0.05f;
	ball.color = WHITE;
	/*Sphere ballDraw{};
	ballDraw.ceneter = ball.position;
	ballDraw.radius = ball.radius;*/


	// 平面
	Plane plane;
	plane.normal = MyVec3(-0.2f, 0.9f, -0.3f).Normalize();
	plane.distance = 0.0f;


	//円運動
	/*float angularVelocity = 3.14f;
	float angle = 0.0f;
	float r = 0.8f;
	Sphere p{};
	p.ceneter = { r,0.0f,0.0f };
	p.radius = 0.08f;*/


	//振り子
	/*Pendulum pendulum{};
	pendulum.anchor = { 0.0f,1.0f,0.0f };
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;
	Sphere p{};
	p.ceneter = { 0.0f,0.0f,0.0f };
	p.radius = 0.08f;*/


	//円錐振り子
	/*ConicalPendulum conicalPendulum;
	conicalPendulum.anchor = { 0.0f,1.0f,0.0f };
	conicalPendulum.length = 0.8f;
	conicalPendulum.halfApexAngle = 0.7f;
	conicalPendulum.angle = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;
	Sphere p{};
	p.ceneter = { 0.0f,0.2f,0.0f };
	p.radius = 0.08f;
	conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
	conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
	float conicalRad = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
	float conicalheight = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
	p.ceneter.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * conicalRad;
	p.ceneter.y = conicalPendulum.anchor.y - conicalheight;
	p.ceneter.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * conicalRad;*/


	float timer = 0;
	bool isStart = false;
	bool isReset = false;

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

		if (isStart)
		{
			/*ImGui::DragFloat("length", &conicalPendulum.length, 0.01f, 0.01f, 10.0f);
			ImGui::DragFloat("halfApexAngle", &conicalPendulum.halfApexAngle, 0.01f, 0.05f, 1.5f);*/
		}

		if (ImGui::Button("Start"))
		{
			isStart = true;
		}
		if (ImGui::Button("Reset"))
		{
			isReset = true;
			isStart = false;
			ball.position = { 0.8f,1.2f,0.3f };
			ball.aceleration = { 0.0f,-9.8f,0.0f };
			ball.velocity = { 0.0f, 0.0f, 0.0f };
			timer = 0.0f;
		}
		ImGui::Text("time:%f", timer);

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


		//バネ
		if (isStart)
		{
			/*MyVec3 diff = ball.position - spring.anchor;
			float length = diff.Lenght();
			if (length != 0.0f)
			{
				MyVec3 direction = diff.Normalize();
				MyVec3 restPos = spring.anchor + direction * spring.naturalLength;
				MyVec3 displacement = (ball.position - restPos) * length;
				MyVec3 restoringForce = displacement * -spring.stiffness;
				MyVec3 dempingForce = ball.velocity * -spring.dempingCoefficient;
				MyVec3 force = restoringForce + dempingForce;
				ball.aceleration = force / ball.mass + kGravity;
			}
			ball.velocity += ball.aceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;
			ballDraw.ceneter = ball.position;*/


			/*p.ceneter.x = originCenetr.x + std::cos(angle) * r;
			p.ceneter.y = originCenetr.y + std::sin(angle) * r;
			p.ceneter.z = originCenetr.z;
			angle+=angularVelocity*deltaTime;*/


			/*pendulum.angularAcceleration =
				-(9.8f / pendulum.length) * std::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;

			ball.position.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
			ball.position.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
			ball.position.z = pendulum.anchor.z;
			p.ceneter = ball.position;*/


			/*conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
			conicalRad = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
			conicalheight = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
			p.ceneter.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * conicalRad;
			p.ceneter.y = conicalPendulum.anchor.y - conicalheight;
			p.ceneter.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * conicalRad;*/


			ball.velocity += ball.aceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;
			if (IsCollision(Sphere{ ball.position,ball.radius }, plane))
			{
				MyVec3 reflected = Reflect(ball.velocity, plane.normal);
				MyVec3 projectToNormal = Project(reflected, plane.normal);
				MyVec3 movingDirection = reflected - projectToNormal;
				const float e = 0.6f;
				ball.velocity = projectToNormal * e + movingDirection;

				//Capsule capsule;
				//capsule.radius = ball.radius;
				//capsule.segument.origin = ball.position;
				//capsule.segument.diff = plane.normal * 4.0f;

				//float dot = capsule.segument.diff * plane.normal;
				//float t = plane.distance - capsule.segument.origin * plane.normal;
				//if (dot != 0.0f)
				//{
					//t = t / dot;
					//if (-0.5f <= t && t <= 1.0f)
					//{
						//MyVec3 aPoint = capsule.segument.diff * t;
						//MyVec3 addLength = (plane.normal) * capsule.radius;
						//aPoint += addLength;
						//ball.position += aPoint;
					//}
				//}

			}

			timer += 1.0f * deltaTime;
		}


		/*Matrix4x4 worldMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);*/
		Matrix4x4 cameraMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, cameraPosition);
		cameraMat = Multiply(cameraMat, MakeRotateXMatrix(camerarota.x));
		cameraMat = Multiply(cameraMat, MakeRotateYMatrix(camerarota.y));
		Matrix4x4 viewMat = Inverse(cameraMat);
		Matrix4x4 projectionMat = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.f);
		Matrix4x4 viewProject= Multiply(viewMat, projectionMat);
		Matrix4x4 viewportMat = MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);
		/*Matrix4x4 worldViewProjectionMat = Multiply(viewMat, projectionMat);
		worldViewProjectionMat = Multiply(worldMat, worldViewProjectionMat);

		MyVec3 originPoint = Transform(Transform(segument.origin, viewProject), viewportMat);
		MyVec3 diffPoint = Transform(Transform((segument.origin+segument.diff), viewProject), viewportMat);*/

		/*MyVec3 originPoint = Transform(Transform(conicalPendulum.anchor, viewProject), viewportMat);
		MyVec3 diffPoint = Transform(Transform(p.ceneter, viewProject), viewportMat);*/


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		DrawGrid(viewProject, viewportMat);

		/*DrawAABB(aabb1, viewProject, viewportMat, color);*/
		//Novice::DrawLine(int(originPoint.x), int(originPoint.y),
		//	int(diffPoint.x), int(diffPoint.y), WHITE);

		/*Novice::DrawLine(int(originPoint.x), int(originPoint.y),
			int(diffPoint.x), int(diffPoint.y), WHITE);
		DrawSphere(p, viewProject, viewportMat, ball.color);*/

		DrawPlane(plane, viewProject, viewportMat, WHITE);

		DrawSphere(Sphere{ ball.position,ball.radius }, viewProject, viewportMat, WHITE);


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

bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
	{
		return true;
	}
	return false;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	MyVec3 closestPoint{ std::clamp(sphere.ceneter.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.ceneter.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.ceneter.z,aabb.min.z,aabb.max.z) };
	float leng = (closestPoint - sphere.ceneter).Lenght();
	if (leng<=sphere.radius)
	{
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Segument& segument)
{
	/*assert(!(segument.diff.x == 0 && segument.diff.y == 0 && segument.diff.z == 0));*/
	if ((segument.diff.x == 0) && (segument.diff.y == 0) && (segument.diff.z == 0))
	{
		if ((segument.origin.x >= aabb.min.x && segument.origin.x <= aabb.max.x)&&
			(segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y)&&
			(segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z))
		{
			return true;
		}
		return false;
	}
	
	if (segument.diff.x==0)
	{
		if (!(segument.origin.x >= aabb.min.x && segument.origin.x <= aabb.max.x))
		{
			return false;
		}
		if (segument.diff.y == 0)
		{
			if ((segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y))
			{
				if ((segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z) ||
					((segument.origin.z + segument.diff.z) >= aabb.min.z && (segument.origin.z + segument.diff.z) <= aabb.max.z) ||
					(segument.origin.z <= aabb.min.z && (segument.origin.z + segument.diff.z) >= aabb.max.z) ||
					((segument.origin.z + segument.diff.z) <= aabb.min.z && segument.origin.z >= aabb.max.z))
				{
					return true;
				}
			}
			return false;
		}
		if (segument.diff.z == 0)
		{
			if ((segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z))
			{
				if ((segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y) ||
					((segument.origin.y + segument.diff.y) >= aabb.min.y && (segument.origin.y + segument.diff.y) <= aabb.max.y) ||
					(segument.origin.y <= aabb.min.y && (segument.origin.y + segument.diff.y) >= aabb.max.y) ||
					((segument.origin.y + segument.diff.y) <= aabb.min.y && segument.origin.y >= aabb.max.y))
				{
					return true;
				}
			}
			return false;
		}
		float tymin = (aabb.min.y - segument.origin.y) / segument.diff.y;
		float tymax = (aabb.max.y - segument.origin.y) / segument.diff.y;
		float tzmin = (aabb.min.z - segument.origin.z) / segument.diff.z;
		float tzmax = (aabb.max.z - segument.origin.z) / segument.diff.z;
		float tneary = min(tymin, tymax);
		float tnearz = min(tzmin, tzmax);
		float tfary = max(tymin, tymax);
		float tfarz = max(tzmin, tzmax);
		float tmin = max(tneary, tnearz);
		float tmax = min(tfary, tfarz);
		tmin = Clampf(tmin, 0.0f, 1.0f);
		tmax = Clampf(tmax, 0.0f, 1.0f);
		if (tmin<=tmax)
		{
			if (tmin == 0 && tmax == 0)
			{
				if ((segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y) &&
					(segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z))
				{
					return true;
				}
				return false;
			}
			if (tmin == 1 && tmax == 1)
			{
				if ((segument.origin.y + segument.diff.y >= aabb.min.y && segument.origin.y + segument.diff.y <= aabb.max.y) &&
					(segument.origin.z + segument.diff.z >= aabb.min.z && segument.origin.z + segument.diff.z <= aabb.max.z))
				{
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	}

	if (segument.diff.y == 0)
	{
		if (!(segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y))
		{
			return false;
		}
		if (segument.diff.x == 0)
		{
			if ((segument.origin.x >= aabb.min.x && segument.origin.x <= aabb.max.x))
			{
				if ((segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z) ||
					((segument.origin.z + segument.diff.z) >= aabb.min.z && (segument.origin.z + segument.diff.z) <= aabb.max.z) ||
					(segument.origin.z <= aabb.min.z && (segument.origin.z + segument.diff.z) >= aabb.max.z) ||
					((segument.origin.z + segument.diff.z) <= aabb.min.z && segument.origin.z >= aabb.max.z))
				{
					return true;
				}
			}
			return false;
		}
		if (segument.diff.z == 0)
		{
			if ((segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z))
			{
				if ((segument.origin.x >= aabb.min.x && segument.origin.x <= aabb.max.x) ||
					((segument.origin.x+segument.diff.x) >= aabb.min.x && (segument.origin.x + segument.diff.x) <= aabb.max.x)||
					(segument.origin.x <= aabb.min.x && (segument.origin.x + segument.diff.x) >= aabb.max.x)||
					((segument.origin.x + segument.diff.x) <= aabb.min.x && segument.origin.x >= aabb.max.x))
				{
					return true;
				}
			}
			return false;
		}
		float txmin = (aabb.min.x - segument.origin.x) / segument.diff.x;
		float txmax = (aabb.max.x - segument.origin.x) / segument.diff.x;
		float tzmin = (aabb.min.z - segument.origin.z) / segument.diff.z;
		float tzmax = (aabb.max.z - segument.origin.z) / segument.diff.z;
		float tnearx = min(txmin, txmax);
		float tnearz = min(tzmin, tzmax);
		float tfarx = max(txmin, txmax);
		float tfarz = max(tzmin, tzmax);
		float tmin = max(tnearx, tnearz);
		float tmax = min(tfarx, tfarz);
		tmin = Clampf(tmin, 0.0f, 1.0f);
		tmax = Clampf(tmax, 0.0f, 1.0f);
		if (tmin <= tmax)
		{
			if (tmin == 0 && tmax == 0)
			{
				if ((segument.origin.x >= aabb.min.x && segument.origin.x <= aabb.max.x) &&
					(segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z))
				{
					return true;
				}
				return false;
			}
			if (tmin == 1 && tmax == 1)
			{
				if ((segument.origin.x + segument.diff.x >= aabb.min.x && segument.origin.x + segument.diff.x <= aabb.max.x) &&
					(segument.origin.z + segument.diff.z >= aabb.min.z && segument.origin.z + segument.diff.z <= aabb.max.z))
				{
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	}

	if (segument.diff.z == 0)
	{
		if (!(segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z))
		{
			return false;
		}
		if (segument.diff.y == 0)
		{
			if ((segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y))
			{
				if ((segument.origin.x >= aabb.min.x && segument.origin.x <= aabb.max.x) ||
					((segument.origin.x + segument.diff.x) >= aabb.min.x && (segument.origin.x + segument.diff.x) <= aabb.max.x) ||
					(segument.origin.x <= aabb.min.x && (segument.origin.x + segument.diff.x) >= aabb.max.x) ||
					((segument.origin.x + segument.diff.x) <= aabb.min.x && segument.origin.x >= aabb.max.x))
				{
					return true;
				}
			}
			return false;
		}
		if (segument.diff.x == 0)
		{
			if ((segument.origin.x >= aabb.min.x && segument.origin.x <= aabb.max.x))
			{
				if ((segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y) ||
					((segument.origin.y + segument.diff.y) >= aabb.min.y && (segument.origin.y + segument.diff.y) <= aabb.max.y) ||
					(segument.origin.y <= aabb.min.y && (segument.origin.y + segument.diff.y) >= aabb.max.y) ||
					((segument.origin.y + segument.diff.y) <= aabb.min.y && segument.origin.y >= aabb.max.y))
				{
					return true;
				}
			}
			return false;
		}
		float txmin = (aabb.min.x - segument.origin.x) / segument.diff.x;
		float txmax = (aabb.max.x - segument.origin.x) / segument.diff.x;
		float tymin = (aabb.min.y - segument.origin.y) / segument.diff.y;
		float tymax = (aabb.max.y - segument.origin.y) / segument.diff.y;
		float tnearx = min(txmin, txmax);
		float tneary = min(tymin, tymax);
		float tfarx = max(txmin, txmax);
		float tfary = max(tymin, tymax);
		float tmin = max(tnearx, tneary);
		float tmax = min(tfarx, tfary);
		tmin = Clampf(tmin, 0.0f, 1.0f);
		tmax = Clampf(tmax, 0.0f, 1.0f);
		if (tmin <= tmax)
		{
			if (tmin == 0 && tmax == 0)
			{
				if ((segument.origin.x >= aabb.min.x && segument.origin.x <= aabb.max.x) &&
					(segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y))
				{
					return true;
				}
				return false;
			}
			if (tmin == 1 && tmax == 1)
			{
				if ((segument.origin.x + segument.diff.x >= aabb.min.x && segument.origin.x + segument.diff.x <= aabb.max.x) &&
					(segument.origin.y + segument.diff.y >= aabb.min.y && segument.origin.y + segument.diff.y <= aabb.max.y))
				{
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	}
	
	float txmin = (aabb.min.x - segument.origin.x) / segument.diff.x;
	float txmax = (aabb.max.x - segument.origin.x) / segument.diff.x;
	float tymin = (aabb.min.y - segument.origin.y) / segument.diff.y;
	float tymax = (aabb.max.y - segument.origin.y) / segument.diff.y;
	float tzmin = (aabb.min.z - segument.origin.z) / segument.diff.z;
	float tzmax = (aabb.max.z - segument.origin.z) / segument.diff.z;
	float tnearx = min(txmin, txmax);
	float tneary = min(tymin, tymax);
	float tnearz = min(tzmin, tzmax);
	float tfarx = max(txmin, txmax);
	float tfary = max(tymin, tymax);
	float tfarz = max(tzmin, tzmax);

	float tmin = max(max(tnearx, tneary), tnearz);
	tmin=Clampf(tmin, 0.0f, 1.0f);
	float tmax = min(min(tfarx, tfary), tfarz);
	tmax=Clampf(tmax, 0.0f, 1.0f);
	if (tmin <= tmax)
	{
		if (tmin == 0 && tmax == 0)
		{
			if ((segument.origin.x>=aabb.min.x&& segument.origin.x <= aabb.max.x)&&
				(segument.origin.y >= aabb.min.y && segument.origin.y <= aabb.max.y)&&
				(segument.origin.z >= aabb.min.z && segument.origin.z <= aabb.max.z))
			{
				return true;
			}
			return false;
		}
		if (tmin == 1 && tmax == 1)
		{
			if ((segument.origin.x + segument.diff.x >= aabb.min.x && segument.origin.x + segument.diff.x <= aabb.max.x) &&
				(segument.origin.y + segument.diff.y >= aabb.min.y && segument.origin.y + segument.diff.y <= aabb.max.y) &&
				(segument.origin.z + segument.diff.z >= aabb.min.z && segument.origin.z + segument.diff.z <= aabb.max.z))
			{
				return true;
			}
			return false;
		}
		return true;
	}

	return false;
}

bool IsCollision(const OBB& obb, const Sphere& sphere)
{
	Matrix4x4 obbWorldMatrix{};
	obbWorldMatrix = { { obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z,0,
		 obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z,0,
		 obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z,0,
		 obb.center.x,obb.center.y,obb.center.z,1
	} };
	Matrix4x4 obbWorldInverse = Inverse(obbWorldMatrix);
	MyVec3 centerInOBBLocalSpace = Transform(sphere.ceneter, obbWorldInverse);

	AABB aabbOBBLocal{ .min = (obb.size) * -1,.max = obb.size };
	Sphere sphereOBBLocal{ centerInOBBLocalSpace,sphere.radius };
	if (IsCollision(aabbOBBLocal, sphereOBBLocal)) { return true; }

	return false;
}

bool IsCollision(const OBB& obb, const Segument& segu)
{
	Matrix4x4 obbWorldMatrix{};
	obbWorldMatrix = { { obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z,0,
		 obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z,0,
		 obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z,0,
		 obb.center.x,obb.center.y,obb.center.z,1
	} };
	Matrix4x4 obbWorldInverse = Inverse(obbWorldMatrix);
	MyVec3 localOrigin = Transform(segu.origin, obbWorldInverse);
	MyVec3 localEnd = Transform(segu.origin + segu.diff, obbWorldInverse);
	Segument localSegument;
	localSegument.origin = localOrigin;
	localSegument.diff = localEnd - localOrigin;

	AABB localOBB = { .min = (obb.size) * -1,.max = obb.size };

	return IsCollision(localOBB, localSegument);
}

bool IsCollision(const OBB& obb1, const OBB& obb2)
{
	MyVec3 obb1vertices[8] = {};
	obb1vertices[0] = { -obb1.size.x,-obb1.size.y,-obb1.size.z };
	obb1vertices[1] = { obb1.size.x,-obb1.size.y,-obb1.size.z };
	obb1vertices[2] = { obb1.size.x,-obb1.size.y,obb1.size.z };
	obb1vertices[3] = { -obb1.size.x,-obb1.size.y,obb1.size.z };
	obb1vertices[4] = { obb1.size.x,obb1.size.y,obb1.size.z };
	obb1vertices[5] = { -obb1.size.x,obb1.size.y,obb1.size.z };
	obb1vertices[6] = { -obb1.size.x,obb1.size.y,-obb1.size.z };
	obb1vertices[7] = { obb1.size.x,obb1.size.y,-obb1.size.z };

	MyVec3 obb2vertices[8] = {};
	obb2vertices[0] = { -obb2.size.x,-obb2.size.y,-obb2.size.z };
	obb2vertices[1] = { obb2.size.x,-obb2.size.y,-obb2.size.z };
	obb2vertices[2] = { obb2.size.x,-obb2.size.y,obb2.size.z };
	obb2vertices[3] = { -obb2.size.x,-obb2.size.y,obb2.size.z };
	obb2vertices[4] = { obb2.size.x,obb2.size.y,obb2.size.z };
	obb2vertices[5] = { -obb2.size.x,obb2.size.y,obb2.size.z };
	obb2vertices[6] = { -obb2.size.x,obb2.size.y,-obb2.size.z };
	obb2vertices[7] = { obb2.size.x,obb2.size.y,-obb2.size.z };

	Matrix4x4 obb1WorldMatrix{};
	obb1WorldMatrix = { { obb1.orientations[0].x,obb1.orientations[0].y,obb1.orientations[0].z,0,
		 obb1.orientations[1].x,obb1.orientations[1].y,obb1.orientations[1].z,0,
		 obb1.orientations[2].x,obb1.orientations[2].y,obb1.orientations[2].z,0,
		 obb1.center.x,obb1.center.y,obb1.center.z,1
	} };
	for (int i = 0; i < 8; i++)
	{
		obb1vertices[i] = Transform(obb1vertices[i], obb1WorldMatrix);
	}

	Matrix4x4 obb2WorldMatrix{};
	obb2WorldMatrix = { { obb2.orientations[0].x,obb2.orientations[0].y,obb2.orientations[0].z,0,
		 obb2.orientations[1].x,obb2.orientations[1].y,obb2.orientations[1].z,0,
		 obb2.orientations[2].x,obb2.orientations[2].y,obb2.orientations[2].z,0,
		 obb2.center.x,obb2.center.y,obb2.center.z,1
	} };
	for (int i = 0; i < 8; i++)
	{
		obb2vertices[i] = Transform(obb2vertices[i], obb2WorldMatrix);
	}

	MyVec3 obb1ProjectV1[8];
	MyVec3 obb1ProjectV2[8];
	MyVec3 obb1ProjectV3[8];
	MyVec3 obb1ProjectV4[8];
	MyVec3 obb1ProjectV5[8];
	MyVec3 obb1ProjectV6[8];
	for (int i = 0; i < 8; i++)
	{

		obb1ProjectV1[i] = Project(obb1vertices[i], obb1.orientations[0]);//x
		obb1ProjectV2[i] = Project(obb1vertices[i], obb1.orientations[1]);//y
		obb1ProjectV3[i] = Project(obb1vertices[i], obb1.orientations[2]);//z
		obb1ProjectV4[i] = Project(obb1vertices[i], obb2.orientations[0]);//x
		obb1ProjectV5[i] = Project(obb1vertices[i], obb2.orientations[1]);//y
		obb1ProjectV6[i] = Project(obb1vertices[i], obb2.orientations[2]);//z
	}
	float min1v1 = obb1ProjectV1[0].x;
	float min1v2 = obb1ProjectV2[0].y;
	float min1v3 = obb1ProjectV3[0].z;
	float min1v4 = obb1ProjectV4[0].x;
	float min1v5 = obb1ProjectV5[0].y;
	float min1v6 = obb1ProjectV6[0].z;

	float max1v1 = obb1ProjectV1[0].x;
	float max1v2 = obb1ProjectV2[0].y;
	float max1v3 = obb1ProjectV3[0].z;
	float max1v4 = obb1ProjectV4[0].x;
	float max1v5 = obb1ProjectV5[0].y;
	float max1v6 = obb1ProjectV6[0].z;
	for (int i = 1; i < 8; i++)
	{
		min1v1 = ((std::min)(min1v1, obb1ProjectV1[i].x));
		min1v2 = ((std::min)(min1v2, obb1ProjectV2[i].y));
		min1v3 = ((std::min)(min1v3, obb1ProjectV3[i].z));
		min1v4 = ((std::min)(min1v4, obb1ProjectV4[i].x));
		min1v5 = ((std::min)(min1v5, obb1ProjectV5[i].y));
		min1v6 = ((std::min)(min1v6, obb1ProjectV6[i].z));

		max1v1 = ((std::max)(max1v1, obb1ProjectV1[i].x));
		max1v2 = ((std::max)(max1v2, obb1ProjectV2[i].y));
		max1v3 = ((std::max)(max1v3, obb1ProjectV3[i].z));
		max1v4 = ((std::max)(max1v4, obb1ProjectV4[i].x));
		max1v5 = ((std::max)(max1v5, obb1ProjectV5[i].y));
		max1v6 = ((std::max)(max1v6, obb1ProjectV6[i].z));
	}
	float L1v1 = max1v1 - min1v1;
	float L1v2 = max1v2 - min1v2;
	float L1v3 = max1v3 - min1v3;
	float L1v4 = max1v4 - min1v4;
	float L1v5 = max1v5 - min1v5;
	float L1v6 = max1v6 - min1v6;


	MyVec3 obb2ProjectV1[8];
	MyVec3 obb2ProjectV2[8];
	MyVec3 obb2ProjectV3[8];
	MyVec3 obb2ProjectV4[8];
	MyVec3 obb2ProjectV5[8];
	MyVec3 obb2ProjectV6[8];
	for (int i = 0; i < 8; i++)
	{

		obb2ProjectV1[i] = Project(obb2vertices[i], obb1.orientations[0]);
		obb2ProjectV2[i] = Project(obb2vertices[i], obb1.orientations[1]);
		obb2ProjectV3[i] = Project(obb2vertices[i], obb1.orientations[2]);
		obb2ProjectV4[i] = Project(obb2vertices[i], obb2.orientations[0]);
		obb2ProjectV5[i] = Project(obb2vertices[i], obb2.orientations[1]);
		obb2ProjectV6[i] = Project(obb2vertices[i], obb2.orientations[2]);
	}
	float min2v1 = obb2ProjectV1[0].x;
	float min2v2 = obb2ProjectV2[0].y;
	float min2v3 = obb2ProjectV3[0].z;
	float min2v4 = obb2ProjectV4[0].x;
	float min2v5 = obb2ProjectV5[0].y;
	float min2v6 = obb2ProjectV6[0].z;

	float max2v1 = obb2ProjectV1[0].x;
	float max2v2 = obb2ProjectV2[0].y;
	float max2v3 = obb2ProjectV3[0].z;
	float max2v4 = obb2ProjectV4[0].x;
	float max2v5 = obb2ProjectV5[0].y;
	float max2v6 = obb2ProjectV6[0].z;
	for (int i = 1; i < 8; i++)
	{
		min2v1 = ((std::min)(min2v1, obb2ProjectV1[i].x));
		min2v2 = ((std::min)(min2v2, obb2ProjectV2[i].y));
		min2v3 = ((std::min)(min2v3, obb2ProjectV3[i].z));
		min2v4 = ((std::min)(min2v4, obb2ProjectV4[i].x));
		min2v5 = ((std::min)(min2v5, obb2ProjectV5[i].y));
		min2v6 = ((std::min)(min2v6, obb2ProjectV6[i].z));

		max2v1 = ((std::max)(max2v1, obb2ProjectV1[i].x));
		max2v2 = ((std::max)(max2v2, obb2ProjectV2[i].y));
		max2v3 = ((std::max)(max2v3, obb2ProjectV3[i].z));
		max2v4 = ((std::max)(max2v4, obb2ProjectV4[i].x));
		max2v5 = ((std::max)(max2v5, obb2ProjectV5[i].y));
		max2v6 = ((std::max)(max2v6, obb2ProjectV6[i].z));
	}
	float L2v1 = max2v1 - min2v1;
	float L2v2 = max2v2 - min2v2;
	float L2v3 = max2v3 - min2v3;
	float L2v4 = max2v4 - min2v4;
	float L2v5 = max2v5 - min2v5;
	float L2v6 = max2v6 - min2v6;


	float sumSpanv1 = L1v1 + L2v1;
	float sumSpanv2 = L1v2 + L2v2;
	float sumSpanv3 = L1v3 + L2v3;
	float sumSpanv4 = L1v4 + L2v4;
	float sumSpanv5 = L1v5 + L2v5;
	float sumSpanv6 = L1v6 + L2v6;

	float longSpanv1 = (std::max)(max1v1, max2v1) - (std::min)(min1v1, min2v1);
	float longSpanv2 = (std::max)(max1v2, max2v2) - (std::min)(min1v2, min2v2);
	float longSpanv3 = (std::max)(max1v3, max2v3) - (std::min)(min1v3, min2v3);
	float longSpanv4 = (std::max)(max1v4, max2v4) - (std::min)(min1v4, min2v4);
	float longSpanv5 = (std::max)(max1v5, max2v5) - (std::min)(min1v5, min2v5);
	float longSpanv6 = (std::max)(max1v6, max2v6) - (std::min)(min1v6, min2v6);
	if (sumSpanv1 < longSpanv1 || sumSpanv2 < longSpanv2 || sumSpanv3 < longSpanv3)
	{
		return false;
	}
	if (sumSpanv4 < longSpanv4 || sumSpanv5 < longSpanv5 || sumSpanv6 < longSpanv6)
	{
		return false;
	}

	return true;
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

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjection, const Matrix4x4 viewPort, uint32_t color)
{
	MyVec3 points[8];
	points[0]= Transform(Transform(aabb.min, viewProjection), viewPort);
	points[1] = Transform(Transform(MyVec3(aabb.max.x,aabb.min.y,aabb.min.z), viewProjection), viewPort);
	points[2] = Transform(Transform(MyVec3(aabb.max.x, aabb.min.y, aabb.max.z), viewProjection), viewPort);
	points[3] = Transform(Transform(MyVec3(aabb.min.x, aabb.min.y, aabb.max.z), viewProjection), viewPort);

	points[4] = Transform(Transform(aabb.max, viewProjection), viewPort);
	points[5] = Transform(Transform(MyVec3(aabb.min.x,aabb.max.y,aabb.max.z), viewProjection), viewPort);
	points[6] = Transform(Transform(MyVec3(aabb.min.x,aabb.max.y,aabb.min.z), viewProjection), viewPort);
	points[7] = Transform(Transform(MyVec3(aabb.max.x,aabb.max.y,aabb.min.z), viewProjection), viewPort);

	Novice::DrawLine(int(points[0].x), int(points[0].y),
		int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y),
		int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y),
		int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y),
		int(points[0].x), int(points[0].y), color);

	Novice::DrawLine(int(points[4].x), int(points[4].y),
		int(points[5].x), int(points[5].y), color);
	Novice::DrawLine(int(points[5].x), int(points[5].y),
		int(points[6].x), int(points[6].y), color);
	Novice::DrawLine(int(points[6].x), int(points[6].y),
		int(points[7].x), int(points[7].y), color);
	Novice::DrawLine(int(points[7].x), int(points[7].y),
		int(points[4].x), int(points[4].y), color);

	Novice::DrawLine(int(points[0].x), int(points[0].y),
		int(points[6].x), int(points[6].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y),
		int(points[7].x), int(points[7].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y),
		int(points[4].x), int(points[4].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y),
		int(points[5].x), int(points[5].y), color);
}

void DrawOBB(const OBB& obb, const Matrix4x4& viewprojection, const Matrix4x4& viewPort, uint32_t color)
{
	Matrix4x4 obbWorldMatrix{};
	obbWorldMatrix = { { obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z,0,
		 obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z,0,
		 obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z,0,
		 obb.center.x,obb.center.y,obb.center.z,1
	} };
	obbWorldMatrix = Multiply(obbWorldMatrix, viewprojection);
	MyVec3 points[8];
	points[0] = { -obb.size.x,-obb.size.y,-obb.size.z };
	points[1] = { obb.size.x,-obb.size.y,-obb.size.z };
	points[2] = { obb.size.x,-obb.size.y,obb.size.z };
	points[3] = { -obb.size.x,-obb.size.y,obb.size.z };
	points[4] = { obb.size.x,obb.size.y,obb.size.z };
	points[5] = { -obb.size.x,obb.size.y,obb.size.z };
	points[6] = { -obb.size.x,obb.size.y,-obb.size.z };
	points[7] = { obb.size.x,obb.size.y,-obb.size.z };

	for (int i = 0; i < 8; i++)
	{
		points[i] = Transform(points[i], obbWorldMatrix);
		points[i] = Transform(points[i], viewPort);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y),
		int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y),
		int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y),
		int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y),
		int(points[0].x), int(points[0].y), color);

	Novice::DrawLine(int(points[4].x), int(points[4].y),
		int(points[5].x), int(points[5].y), color);
	Novice::DrawLine(int(points[5].x), int(points[5].y),
		int(points[6].x), int(points[6].y), color);
	Novice::DrawLine(int(points[6].x), int(points[6].y),
		int(points[7].x), int(points[7].y), color);
	Novice::DrawLine(int(points[7].x), int(points[7].y),
		int(points[4].x), int(points[4].y), color);

	Novice::DrawLine(int(points[0].x), int(points[0].y),
		int(points[6].x), int(points[6].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y),
		int(points[7].x), int(points[7].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y),
		int(points[4].x), int(points[4].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y),
		int(points[5].x), int(points[5].y), color);

}

void DrawBezier(const std::vector<MyVec3> control, const Matrix4x4& viewProjection, const Matrix4x4 viewPort, uint32_t color)
{
	float t = 0;
	const size_t segumentCount = 50;
	std::vector<MyVec3> drawpoint;
	for (int i = 0; i <= segumentCount; i++)
	{
		MyVec3 p0p1 = Lerp(control[0], control[1], t);
		MyVec3 p1p2 = Lerp(control[1], control[2], t);
		MyVec3 p = Lerp(p0p1, p1p2, t);
		t += 0.02f;

		p = Transform(Transform(p, viewProjection), viewPort);
		drawpoint.push_back(p);
	}
	for (int i = 0; i < drawpoint.size()-1;)
	{
		int index1 = i;
		i++;
		int index2 = i;
		Novice::DrawLine(int(drawpoint[index1].x), int(drawpoint[index1].y), int(drawpoint[index2].x), int(drawpoint[index2].y), color);
	}

}

void DrawCatmullRom(const std::vector<MyVec3> control, const Matrix4x4& viewProjection, const Matrix4x4 viewPort, uint32_t color)
{
	std::vector<MyVec3> drawpoint;
	const size_t segumentCount = 100;
	float t = 0;
	for (int i = 0; i <= segumentCount; i++)
	{
		MyVec3 pos = CatmullRom(control, t);
		pos = Transform(Transform(pos, viewProjection), viewPort);
		drawpoint.push_back(pos);
		t += 0.01f;
	}
	for (int i = 0; i < drawpoint.size()-1;)
	{
		int index1 = i;
		i++;
		int index2 = i;
		Novice::DrawLine(int(drawpoint[index1].x), int(drawpoint[index1].y), int(drawpoint[index2].x), int(drawpoint[index2].y), color);
	}

}

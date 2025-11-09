#include "olcConsoleGameEngine.h"
#include "mesh.h"
#include <strstream>
#include <algorithm>
using namespace std;

struct mat4x4
{
	float m[4][4] = { 0 };
};

class olcEngine3D : public olcConsoleGameEngine
{
public:
	olcEngine3D()
	{
		m_sAppName = L"3D Demo";
	}

private:
	mesh meshCube;
	mat4x4 matProj;

	vec3d vCamera;


	float fTheta;

	void MultiplyMatrixVector(const vec3d &i, vec3d &o, const mat4x4 &m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}

	CHAR_INFO GetColor(float lum)
	{
		short bg_col, fg_col;
		wchar_t sym;
		int pixel_bw = (int)(13.0f*lum);
		switch (pixel_bw)
		{
		case 0: 
			bg_col = BG_BLACK; 
			fg_col = FG_BLACK; 
			sym = PIXEL_SOLID;
			break;
		case 1: 
			bg_col = BG_BLACK;
			fg_col = FG_DARK_GREY;
			sym = PIXEL_QUARTER;
			break;
		case 2:
			bg_col = BG_BLACK;
			fg_col = FG_DARK_GREY;
			sym = PIXEL_HALF;
			break;
		case 3:
			bg_col = BG_BLACK;
			fg_col = FG_DARK_GREY;
			sym = PIXEL_THREEQUARTERS;
			break;
		case 4:
			bg_col = BG_BLACK;
			fg_col = FG_DARK_GREY;
			sym = PIXEL_SOLID;
			break;
			
		case 5:
			bg_col = BG_DARK_GREY;
			fg_col = FG_GREY;
			sym = PIXEL_QUARTER;
			break;
		case 6:
			bg_col = BG_DARK_GREY;
			fg_col = FG_GREY;
			sym = PIXEL_HALF;
			break;
		case 7:
			bg_col = BG_DARK_GREY;
			fg_col = FG_GREY;
			sym = PIXEL_THREEQUARTERS;
			break;
		case 8:
			bg_col = BG_DARK_GREY;
			fg_col = FG_GREY;
			sym = PIXEL_SOLID;
			break;

		case 9:
			bg_col = BG_GREY;
			fg_col = FG_WHITE;
			sym = PIXEL_QUARTER;
			break;
		case 10:
			bg_col = BG_GREY;
			fg_col = FG_WHITE;
			sym = PIXEL_HALF;
			break;
		case 11:
			bg_col = BG_GREY;
			fg_col = FG_WHITE;
			sym = PIXEL_THREEQUARTERS;
			break;
		case 12:
			bg_col = BG_GREY;
			fg_col = FG_WHITE;
			sym = PIXEL_SOLID;
			break;
		default:
			bg_col = BG_BLACK;
			fg_col = FG_BLACK;
			sym = PIXEL_SOLID;
		}

		CHAR_INFO c;
		c.Attributes = bg_col | fg_col;
		c.Char.UnicodeChar = sym;
		return c;
	}

public: 
	bool OnUserCreate() override
	{
		//Here we are initializing the points on the Cube. Picture a 3D graph, where you have x, y, z.
		//ALways need to be declared Clockwise
		//meshCube.tris = {

		//	// SOUTH
		//	{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		//	{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		//	// EAST                                                      
		//	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		//	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		//	// NORTH                                                     
		//	{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		//	{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		//	// WEST                                                      
		//	{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		//	{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		//	// TOP                                                       
		//	{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		//	{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		//	// BOTTOM                                                    
		//	{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		//	{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		//};

		meshCube.LoadFromObjectFile("VideoShip.obj");

		// Projection Matrix
		// These are the values that are used to calculate the exact sizing of the objects on screen based on
		// their distance away from the view. This is one of the more computationally confusing aspects. By using this
		// Projection Matrix, it gives a highly customizable viewport for the objects being rendered. 
		float fNear = 0.1f; //Near Plane
		float fFar = 1000.0f; //Far Plane
		float fFov = 90.0f; //Field of View (degrees)
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth(); //Aspect Ratio, will adapt to whatever size console
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f); //Tangent Calculation for Projection Matrix (in radians)

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
		//The rest of the 2d matrix is initialized to 0

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//This command essentially just clears the screen
		int screenWidth = ScreenWidth();
		int screenHeight = ScreenHeight();
		Fill(0, 0, screenWidth, screenHeight, PIXEL_SOLID, FG_BLACK);

		mat4x4 matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		vector<triangle> vecTrianglesToRaster;
		vecTrianglesToRaster.reserve(meshCube.tris.size());

		const vec3d lightDirection = { 0.0f, 0.0f, -1.0f };
		float fHalfScreenWidth = 0.5f * static_cast<float>(screenWidth);
		float fHalfScreenHeight = 0.5f * static_cast<float>(screenHeight);

		//Draw our Triangles
		for (const auto &tri : meshCube.tris)
		{
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			//Translate Cube ( to offset where it is)
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 8.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 8.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 8.0f;

			
			//This is where we use the normal of each triangle in a plane (the line perfectly perpendicular to the plane's axis)
			//	To verify whether that the triangle needs to be rendered on screen. 
			vec3d normal, line1, line2;
			line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
			line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
			line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

			line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
			line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
			line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
			normal.x /= l;
			normal.y /= l;
			normal.z /= l;

			//This will only render triangles if they are facing towards the "camera"
			//if (normal.z < 0) {
			if (normal.x * (triTranslated.p[0].x - vCamera.x) +
				normal.y * (triTranslated.p[0].y - vCamera.y) + 
				normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
			{
				// Illumination for shadows
				float dp = normal.x * lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z;

				//This sets the shading from shadows
				CHAR_INFO c = GetColor(dp);
				triTranslated.col = c.Attributes;
				triTranslated.sym = c.Char.UnicodeChar;

				// Project triangles from 3D --> 2D
				MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
				MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
				MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);
				triProjected.col = triTranslated.col;
				triProjected.sym = triTranslated.sym;

				// Scale into view
				triProjected.p[0].x += 1.0f;
				triProjected.p[0].y += 1.0f;
				triProjected.p[1].x += 1.0f;
				triProjected.p[1].y += 1.0f;
				triProjected.p[2].x += 1.0f;
				triProjected.p[2].y += 1.0f;

				triProjected.p[0].x *= fHalfScreenWidth;
				triProjected.p[0].y *= fHalfScreenHeight;
				triProjected.p[1].x *= fHalfScreenWidth;
				triProjected.p[1].y *= fHalfScreenHeight;
				triProjected.p[2].x *= fHalfScreenWidth;
				triProjected.p[2].y *= fHalfScreenHeight;

				// Store triangle for sorting
				vecTrianglesToRaster.push_back(triProjected);
			}

		}

		// Sort triangles from closest to camera to farthest. This sort function allows the passing of a lambda expression to modify the sorting
		//		Algorithm to our needs
		sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle&t2)
		{
			//This is our approximate solution, gets the z components of the midpoints and averages them
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;
		});

		for (auto &triProjected : vecTrianglesToRaster)
		{
			// Rasterize triangle
			FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				triProjected.sym, triProjected.col);

			/*DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				triProjected.sym, triProjected.col);
			*/
		}

		return true;
	}
};

int main()
{
	olcEngine3D demo;
	const int highResFontWidth = 2;
	const int highResFontHeight = 2;
	if (demo.ConstructConsoleFullScreen(highResFontWidth, highResFontHeight) ||
		demo.ConstructConsole(190, 190, 4, 4)) {
		demo.Start();
	}
	return 0;
}


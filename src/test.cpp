

#include "../lib/graphics.h"
#include <iostream>
#include "../lib/graphics_math.h"
#include "../lib/graphics_utility.h"
#include <time.h>


using namespace std;

void square(GraphicsManager* manager)
{
	Point p0 = MakePoint(100, 200);
	Point p1 = MakePoint(100, 600);
	Point p2 = MakePoint(900, 900);
	Point p3 = MakePoint(900, 100);

	Color blue = MakeColor(37, 255, 255);

	manager->DrawGradientTriangle(p0, p1, p2, blue, 0.3, 0.4, 0.5);
	manager->DrawGradientTriangle(p2, p3, p0, blue, 0.5, 0.6, 0.3);
	manager->RefreshScreen();
}


void triforce(GraphicsManager* manager)
{
	
	Point p0, p1, p2;
	p0.x = 900;
	p0.y = 900;
	p1.x = 100;
	p1.y = 900;
	p2.x = 500;
	p2.y = 100;

	Point p3 = MakePoint(300, 500);
	Point p4 = MakePoint(700, 500);
	Point p5 = MakePoint(500, 900);

	Color yellow = MakeColor(255, 245, 37);
	Color grugle = MakeColor(37, 255, 253);

	manager->DrawGradientTriangle(p1, p3, p5, yellow, 0, 1, 1);
	manager->DrawGradientTriangle(p5, p4, p0, yellow, 1, 1, 0);
	manager->DrawGradientTriangle(p3, p2, p4, yellow, 1, 0, 1);

	manager->DrawGradientTriangle(p3, p5, p4, grugle, 1, 0, 1);
	manager->RefreshScreen();
}


Point ProjectVertex(Point3D vertex, float d)
{
	int dest_x, dest_y;
	dest_x = vertex.x * d / vertex.z;
	dest_y = vertex.y * d / vertex.z;
	return Point{dest_x, dest_y};
}

int main()
{


	
			
	// Initialize window
	GraphicsManager test;
	test.OpenWindow(1000, 1000);

	//triforce(&test);

	// CUBE
	Point3D vAf, vBf, vCf, vDf, vAb, vBb, vCb, vDb;
	
	vAf = {2, 0.5, 5};
	vBf = {2, 1.5, 5};
	vCf = {1, 1.5, 5};
	vDf = {1, 0.5, 5};

	vAb = {2, 0.5, 6};
	vBb = {2, 1.5, 6};
	vCb = {1, 1.5, 6};
	vDb = {1, 0.5, 6};
	

//	square(&test);

//	Point points[9];
//	for(int i = 0; i < 9; ++i)
//	{
//
//		
//		points[i] = RandomPoint();
//		std::cout << "Point: (" << points[i].x << ", " << points[i].y << ")" << std::endl;
//		if ((i + 1) % 3 == 0)
//		{
//			test.DrawFillTriangle(points[i], points[i-1], points[i-2]);
//		}
//	}


	test.RefreshScreen();

	test.StayOpenBlocking();
	test.CloseWindow();

	return 0;
}

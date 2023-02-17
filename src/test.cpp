

#include "../lib/graphics.h"
#include <iostream>
#include "../lib/graphics_math.h"
#include "../lib/graphics_utility.h"
using namespace std;

int main()
{
	Point point1, point2, point3;	
	point1.x = 10;
	point1.y = 200;
	point2.x = 50;
	point2.y = 75;
	point3.x = 400;
	point3.y = 400;


			
	// Initialize window
	GraphicsManager test;
	test.OpenWindow(1000, 1000);

	test.ChangeBrushColor(51, 204, 51);

	test.DrawFillTriangle(point1, point2, point3);

	test.ChangeBrushColor(255, 0, 69);
	test.DrawWireTriangle(30, 30, 130, 230, 330, 50);


	Point p0, p1, p2;
	p0.x = 900;
	p0.y = 900;
	p1.x = 100;
	p1.y = 900;
	p2.x = 500;
	p2.y = 100;
	test.DrawFillTriangle(p0, p1, p2);

	Color grugle;
	grugle.red = 103;
	grugle.green = 200;
	grugle.blue = 150;
	test.DrawGradientTriangle(p0, p1, p2, grugle, 0.3, 0, 3);
	
	test.ChangeBrushColor(255, 255, 255);
	test.DrawWireTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
	
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



#include "../lib/graphics.h"
#include <iostream>
#include "../lib/graphics_math.h"
#include "../lib/graphics_utility.h"
#include <time.h>


using namespace std;

void square(GraphicsManager* manager)
{
	typedef Point2D Point;
	Point p0 = {-400, -300};
	Point p1 = {-400, 100};
	Point p2 = {400, 502};
	Point p3 = {400, -400};

	Color blue = {37, 255, 255};

	manager->DrawGradientTriangle(p0, p1, p2, blue, 0.3, 0.4, 0.5);
	manager->DrawGradientTriangle(p2, p3, p0, blue, 0.5, 0.6, 0.3);
	manager->RefreshScreen();
}


void triforce(GraphicsManager* manager)
{
	typedef Point2D Point;
	Point p0, p1, p2;
	p0 = {-400, -400};
	p1 = {0, 400};
	p2 = {400, -400};

	Point p3 = {0, -400};
	Point p4 = {-200, 0};
	Point p5 = {200, 0};

	Color yellow = {255, 245, 37};
	Color grugle = {37, 255, 253};

	manager->DrawGradientTriangle(p4, p5, p1, yellow, 1, 1, 0);
	manager->DrawGradientTriangle(p0, p4, p3, yellow, 0, 1, 1);
	manager->DrawGradientTriangle(p3, p2, p5, YELLOW, 1, 0, 1);

	manager->DrawGradientTriangle(p3, p5, p4, grugle, 1, 0, 1);
	manager->RefreshScreen();
}




int main()
{


	
			
	// Initialize window
	GraphicsManager test;
	test.OpenWindow(1000, 1000);

	// triforce(&test);

	// CUBE
	Point3D vAf, vBf, vCf, vDf, vAb, vBb, vCb, vDb;
	
	test.SetViewportDistance(4);

	// The four "front" vertices
	vAf = {-200, -50, 5};
	vBf = {-200,  50, 5};
	vCf = {-100,  50, 5};
	vDf = {-100, -50, 5};

	// The four "back" vertices
	vAb = {-200, -50, 6};
	vBb = {-200,  50, 6};
	vCb = {-100,  50, 6};
	vDb = {-100, -50, 6};

	// The front face
	test.DrawLine(test.ProjectVertex(vAf), test.ProjectVertex(vBf), BLUE);
	test.DrawLine(test.ProjectVertex(vBf), test.ProjectVertex(vCf), BLUE);
	test.DrawLine(test.ProjectVertex(vCf), test.ProjectVertex(vDf), BLUE);
	test.DrawLine(test.ProjectVertex(vDf), test.ProjectVertex(vAf), BLUE);

	// The back face
	test.DrawLine(test.ProjectVertex(vAb), test.ProjectVertex(vBb), RED);
	test.DrawLine(test.ProjectVertex(vBb), test.ProjectVertex(vCb), RED);
	test.DrawLine(test.ProjectVertex(vCb), test.ProjectVertex(vDb), RED);
	test.DrawLine(test.ProjectVertex(vDb), test.ProjectVertex(vAb), RED);

	// The front-to-back edges
	test.DrawLine(test.ProjectVertex(vAf), test.ProjectVertex(vAb), GREEN);
	test.DrawLine(test.ProjectVertex(vBf), test.ProjectVertex(vBb), GREEN);
	test.DrawLine(test.ProjectVertex(vCf), test.ProjectVertex(vCb), GREEN);
	test.DrawLine(test.ProjectVertex(vDf), test.ProjectVertex(vDb), GREEN);
	

	// The four "front" vertices
	vAf = {200, 200, 5};
	vBf = {200,  400, 5};
	vCf = {400,  400, 5};
	vDf = {400, 200, 5};

	// The four "back" vertices
	vAb = {200, 200, 6};
	vBb = {200,  400, 6};
	vCb = {400,  400, 6};
	vDb = {400, 200, 6};

	// The front face
	test.DrawLine(test.ProjectVertex(vAf), test.ProjectVertex(vBf), BLUE);
	test.DrawLine(test.ProjectVertex(vBf), test.ProjectVertex(vCf), BLUE);
	test.DrawLine(test.ProjectVertex(vCf), test.ProjectVertex(vDf), BLUE);
	test.DrawLine(test.ProjectVertex(vDf), test.ProjectVertex(vAf), BLUE);

	// The back face
	test.DrawLine(test.ProjectVertex(vAb), test.ProjectVertex(vBb), RED);
	test.DrawLine(test.ProjectVertex(vBb), test.ProjectVertex(vCb), RED);
	test.DrawLine(test.ProjectVertex(vCb), test.ProjectVertex(vDb), RED);
	test.DrawLine(test.ProjectVertex(vDb), test.ProjectVertex(vAb), RED);

	// The front-to-back edges
	test.DrawLine(test.ProjectVertex(vAf), test.ProjectVertex(vAb), GREEN);
	test.DrawLine(test.ProjectVertex(vBf), test.ProjectVertex(vBb), GREEN);
	test.DrawLine(test.ProjectVertex(vCf), test.ProjectVertex(vCb), GREEN);
	test.DrawLine(test.ProjectVertex(vDf), test.ProjectVertex(vDb), GREEN);


	// square(&test);

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


	// Point2D a, b, c, d, e, f, g;
	// a = {0, 0};
	// b = {20, 40};
	// c = {-90, 0};
	// test.DrawFillTriangle(a, b, c, PURPLE);

	test.RefreshScreen();

	test.StayOpenBlocking();
	test.CloseWindow();

	return 0;
}

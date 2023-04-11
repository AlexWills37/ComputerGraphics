

#include "../lib/graphics.h"
#include <iostream>
#include "../lib/graphics_utility.h"
#include <time.h>
#include <functional>


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

void cube(GraphicsManager* manager){
	GraphicsManager test = *manager;

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

}

Point3D TranslatePoint(const Point3D p, float xDiff, float yDiff, float zDiff)
{
	Point3D newPoint;
	newPoint.x = p.x + xDiff;
	newPoint.y = p.y + yDiff;
	newPoint.z = p.z + zDiff;

	return newPoint;
}

void RenderTriangle(GraphicsManager* graphics, Triangle triangle, Point2D projected_vertices[])
{
	graphics->DrawWireTriangle(
		projected_vertices[triangle.p0], projected_vertices[triangle.p1], projected_vertices[triangle.p2],
		triangle.color);
}

void RenderObject(GraphicsManager* graphics, Point3D vertices[], int numVerts, Triangle triangles[], int numTris)
{

	// Get list of projected vertices
	Point2D projected_points[numVerts];
	for (int i = 0; i < numVerts; ++i)
	{
		projected_points[i] = graphics->ProjectVertex(TranslatePoint(vertices[i], -1.5, 0, 7));
	}

	// Render each triangle
	for (int i = 0; i < numTris; ++i)
	{
		RenderTriangle(graphics, triangles[i], projected_points);
	}

}

void render_scene(GraphicsManager* manager){
	GraphicsManager test = *manager;
	// Vertices
	Point3D vertices[8];
	vertices[0] = {1, 1, 1};
	vertices[1] = {-1, 1, 1};
	vertices[2] = {-1, -1, 1};
	vertices[3] = {1, -1, 1};
	vertices[4] = {1, 1, -1};
	vertices[5] = {-1, 1, -1};
	vertices[6] = {-1, -1, -1};
	vertices[7] = {1, -1, -1};


	Triangle triangles[12];
	triangles[0] = {0,1,2,RED};
	triangles[1] = {0,2,3,RED};
	triangles[2] = {4,0,3,GREEN};
	triangles[3] = {4,3,7,GREEN};
	triangles[4] = {5,4,7,INDIGO};
	triangles[5] = {5,7,6,INDIGO};
	triangles[6] = {1,5,6,YELLOW};
	triangles[7] = {1,6,2,YELLOW};
	triangles[8] = {4,5,1,PURPLE};
	triangles[9] = {4,1,0,PURPLE};
	triangles[10] = {2,6,7,BLUE};
	triangles[11] = {2,7,3,BLUE};
	
	RenderObject(manager, vertices, 8, triangles, 12);

}

void RenderInstance(GraphicsManager* graphics, ModelInstance * instance, TransformMatrix camera_matrix)
{
	Model * model = instance->model;

	Point2D projected_points[ model->numVertices ];

	HomCoordinates newPoint;
	Point3D newPointPoint;
	TransformMatrix world_space_transform = instance->transform;	// This converts from model space to world space
	// Project all of the points
	for (int i = 0; i < model->numVertices; ++i)
	{
		// Get the model-space point as homogenous coordinates
		newPoint.data[0] = model->vertices[i].x;
		newPoint.data[1] = model->vertices[i].y;
		newPoint.data[2] = model->vertices[i].z;
		newPoint.data[3] = 1;
		
		// Apply matrix to convert to world space
		newPoint = world_space_transform * newPoint;
		// Apply camera transform
		newPoint = camera_matrix * newPoint;
		newPointPoint = {newPoint.data[0], newPoint.data[1], newPoint.data[2]};

		projected_points[i] = graphics->ProjectVertex(newPointPoint);
	}

	// Render all triangles
	for (int i = 0; i < model->numTriangles; ++i)
	{
		RenderTriangle(graphics, model->triangles[i], projected_points);
	}

}


TransformMatrix GetMatrix(Transform transform)
{
	TransformMatrix m;

	return m;
}

void MatrixTest()
{
	TransformMatrix a = { { {5, 7, 9, 10},
								{2, 3, 3, 8},
								{8, 10, 2, 3},
								{3, 3, 4, 8}}};

	TransformMatrix b = { { {3, 10, 12, 18},
								{12, 1, 4, 9},
								{9, 10, 12, 2},
								{3, 12, 4, 10}}};

	Transform t = Transform(2, 1, 1, 3.14 / 2, 3.14 / 2, 0, 0, 5, 0);
	HomCoordinates c;
	c.data[0] = 1;
	c.data[1] = 0;
	c.data[2] = 0;
	c.data[3] = 1;

	// cout << "Before transformation:" << endl;
	// for (int i = 0; i < 4; ++i)
	// {
	// 	cout << i << ": " << c.data[i] << endl;
	// }

	TransformMatrix out = a * b;
	cout << "a * b: " << endl;
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			cout << out.data[row][col] << "  ";
		}
		cout << endl;
	}

	out = b * a;
	cout << "b * a: " << endl;
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			cout << out.data[row][col] << "  ";
		}
		cout << endl;
	}

	// HomCoordinates newC = out * c;
	// cout << "After transformation:" << endl;
	// for (int i = 0; i < 4; ++i)
	// {
	// 	cout << i << ": " << newC.data[i] << endl;
	// }
}



int main()
{

	// MatrixTest();
	Point3D vertices[8];
	vertices[0] = {1, 1, 1};
	vertices[1] = {-1, 1, 1};
	vertices[2] = {-1, -1, 1};
	vertices[3] = {1, -1, 1};
	vertices[4] = {1, 1, -1};
	vertices[5] = {-1, 1, -1};
	vertices[6] = {-1, -1, -1};
	vertices[7] = {1, -1, -1};	

	Triangle triangles[12];
	triangles[0] = {0,1,2,RED};
	triangles[1] = {0,2,3,RED};
	triangles[2] = {4,0,3,GREEN};
	triangles[3] = {4,3,7,GREEN};
	triangles[4] = {5,4,7,INDIGO};
	triangles[5] = {5,7,6,INDIGO};
	triangles[6] = {1,5,6,YELLOW};
	triangles[7] = {1,6,2,YELLOW};
	triangles[8] = {4,5,1,PURPLE};
	triangles[9] = {4,1,0,PURPLE};
	triangles[10] = {2,6,7,BLUE};
	triangles[11] = {2,7,3,BLUE};

	Model cube = {vertices, 8, triangles, 12};

	Transform tx = Transform(1, 1, 1, 3.14/4, 0, 0, -5, 0, 8);
	Transform ty = Transform(1, 1, 1, 0, 3.14/4, 0, -2, 0, 8);
	Transform tz = Transform(1, 1, 1, 0, 0, 3.14/4, 1, 0, 8);
	Transform t2 = Transform(1, 1, 1, 0, 0, 0, 4, 0, 8);

	// X Y Z Default

	Transform camera = Transform(1, 1, 1, 0, -3.14/6, 0, -4, 0, 0);
	


	ModelInstance scene[4] = { {&cube, t2}, {&cube, tx}, {&cube, ty}, {&cube, tz}};
	
	
	Transform base = Transform(1.5, 0.3, 1.5, 0, 0, 0, 4, -3, 8);
	
	ModelInstance crystal[] = { {&cube, t2}, {&cube, base} };
			
	// Initialize window
	GraphicsManager test;
	GraphicsManager* manager = &test;
	test.OpenWindow(1000, 1000);
	test.SetViewportDistance(6);
	// triforce(&test);


	// square(&test);

	//cube(manager);
	// render_scene(manager);
	// RenderInstance(manager, &cube1, camera);
	// RenderInstance(manager, &cube2, camera);
	Scene* scene1 = test.CreateScene();
	for (int i = 0; i < 4; ++i)
	{
		// RenderInstance(manager, &(scene[i]), camera);
		scene1->AddModelInstance(scene[i]);


	}

	test.rotate_cube = &(crystal[0]);
	test.starting_y = 0;
	
	// triforce(&test);

	scene1->RenderScene();
	test.RefreshScreen();

	test.StayOpenCameraControls();
	// test.StayOpenBlocking();
	test.CloseWindow();

	return 0;
}

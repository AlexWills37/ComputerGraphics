

#include "../lib/graphics.h"
#include <iostream>
#include "../lib/graphics_utility.h"
#include <time.h>
#include <functional>


using namespace std;

void test()
{
	std::vector<int> testy;
	
	std::cout << "vector size: " << testy.size() << std::endl;
	for (int i = 0; i < testy.size(); ++i)
	{
		std::cout << "\t" << i << ": " << testy[i] << std::endl;
	}

	std::cout << "Adding elements..." << std::endl;
	testy.push_back(41);
	testy.push_back(6);
	testy.push_back(56);
	testy.push_back(2);
	testy.push_back(77);
	testy.push_back(25);
	testy.push_back(7);
	testy.push_back(23);
	std::cout << "vector size: " << testy.size() << std::endl;
	for (int i = 0; i < testy.size(); ++i)
	{
		std::cout << "\t" << i << ": " << testy[i] << std::endl;
	}

	std::cout << "iterating magic: " << std::endl;
	std::vector<int>::iterator iterator = testy.begin();
	int initial_size = testy.size();
	int index = 0;
	while (iterator != testy.end() && index < initial_size) {
		std::cout << *iterator << ":";
		if (*iterator.base() == 41){
			testy.erase(iterator);
			std::cout << "~ ";
			// testy.push_back(423);
			// testy.push_back(423);
			// testy.push_back(2);
			iterator--;
		}
		else {
			std::cout << "/ ";
		iterator++;
		}
		index++;


	}
	std::cout << std::endl;

	std::cout << "vector size: " << testy.size() << std::endl;
	for (int i = 0; i < testy.size(); ++i)
	{
		std::cout << "\t" << i << ": " << testy[i] << std::endl;
	}

	std::cout << "cLEARING!" << std::endl;
	testy.clear();
	std::cout << "vector size: " << testy.size() << std::endl;
	for (int i = 0; i < testy.size(); ++i)
	{
		std::cout << "\t" << i << ": " << testy[i] << std::endl;
	}
}


int main()
{

	// test();


	// MatrixTest();
	std::vector<Point3D> vertices = std::vector<Point3D>(8);
	vertices[0] = {1, 1, 1};
	vertices[1] = {-1, 1, 1};
	vertices[2] = {-1, -1, 1};
	vertices[3] = {1, -1, 1};
	vertices[4] = {1, 1, -1};
	vertices[5] = {-1, 1, -1};
	vertices[6] = {-1, -1, -1};
	vertices[7] = {1, -1, -1};	

	Color ca = {171, 244, 255};
	Color cb = {202, 121, 237};
	Color cc = {90, 166, 232};

	std::vector<Triangle> triangles = std::vector<Triangle>(12);
	// triangles[0] = {0,1,2,RED};
	// triangles[1] = {0,2,3,RED};
	// triangles[2] = {4,0,3,GREEN};
	// triangles[3] = {4,3,7,GREEN};
	// triangles[4] = {5,4,7,INDIGO};
	// triangles[5] = {5,7,6,INDIGO};
	// triangles[6] = {1,5,6,YELLOW};
	// triangles[7] = {1,6,2,YELLOW};
	// triangles[8] = {4,5,1,PURPLE};
	// triangles[9] = {4,1,0,PURPLE};
	// triangles[10] = {2,6,7,BLUE};
	// triangles[11] = {2,7,3,BLUE};
	triangles[0] = {0,1,2,ca};
	triangles[1] = {0,2,3,ca};

	triangles[2] = {4,0,3,cb};
	triangles[3] = {4,3,7,cb};

	triangles[4] = {5,4,7,ca};
	triangles[5] = {5,7,6,ca};

	triangles[6] = {1,5,6,cb};
	triangles[7] = {1,6,2,cb};

	triangles[8] = {4,5,1,cc};
	triangles[9] = {4,1,0,cc};
	
	triangles[10] = {2,6,7,cc};
	triangles[11] = {2,7,3,cc};


	Model cube = {vertices, triangles};


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
	test.OpenWindow(700, 700);
	test.GetMainCamera()->SetViewportDistance(0.2);

	// triforce(&test);

	Transform * cam = test.GetMainCamera()->GetTransform();
	// cam->translation[0] = 13;
	// square(&test);

	//cube(manager);
	// render_scene(manager);
	// RenderInstance(manager, &cube1, camera);
	// RenderInstance(manager, &cube2, camera);
	Scene* scene1 = test.CreateScene();
	ModelInstance * working;
	Transform* transform;
	// for (int i = 0; i < 4; ++i)
	// {
	// 	// RenderInstance(manager, &(scene[i]), camera);
	// 	scene1->AddModelInstance(scene[i]);
	// 	working = new ModelInstance(scene[i]);
	// 	working->GetTransform()->translation[2] = -8;
	// 	scene1->AddModelInstance(*working);
	// 	working = new ModelInstance(scene[i]);
	// 	transform = working->GetTransform();
	// 	transform->translation[2] = transform->translation[0];
	// 	transform->translation[0] = 8;
	// 	transform->rotation[1] += 3.14 / 2;
	// 	scene1->AddModelInstance(*working);

	// 	working = new ModelInstance(scene[i]);
	// 	transform = working->GetTransform();
	// 	transform->translation[2] = transform->translation[0];
	// 	transform->translation[0] = -8;
	// 	transform->rotation[1] += 3.14 / 4;
	// 	scene1->AddModelInstance(*working);
	// }

	test.rotate_cube = &(crystal[0]);
	test.starting_y = 0;
	scene1->AddModelInstance(crystal[0]);
	scene1->AddModelInstance(crystal[1]);
	
	// triforce(&test);

	scene1->RenderScene();
	test.RefreshScreen();

	test.StayOpenCameraControls();
	// test.StayOpenBlocking();
	test.CloseWindow();

	return 0;
}

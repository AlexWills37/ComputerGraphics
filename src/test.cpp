

#include "../lib/graphics.h"
#include <iostream>
using namespace std;

int main()
{
	
	Color green = NewColor(51, 204, 51);

	GraphicsManager test;
	test.OpenWindow(1000, 1000);

	test.ChangeBrushColor(51, 204, 51);

	test.DrawLine(0, 0, 1000, 1000);
	test.DrawLine(300, 200, 800, 600);
	test.ChangeBrushColor(255, 255, 255);
	test.DrawLine(0, 500, 1000, 500);
	
	test.RefreshScreen();

	test.StayOpenBlocking();
	test.CloseWindow();

	return 0;
}

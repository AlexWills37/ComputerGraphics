/* graphics_utility.cpp
 *
 * Utility functions for graphics.
 *
 * @author Alex Wills
 * @date February 16, 2023
 */

#include "../lib/graphics_utility.h"
#include "../lib/graphics_math.h"
#include <cstdlib>
#include <ctime>



/*
 * Swaps the values of two points
 */
void SwapPoints(Point2D& p0, Point2D& p1)
{
	int tempx = p0.x;
	int tempy = p0.y;
	p0.x = p1.x;
	p0.y = p1.y;
	p1.x = tempx;
	p1.y = tempy;
}



Color operator*(Color color, float scalar)
{
	int r = int(color.red * scalar);
	int g = int(color.green * scalar);
	int b = int(color.blue * scalar);

	r = clamp(r, 0, 255);
	g = clamp(g, 0, 255);
	b = clamp(b, 0, 255);

	return Color{r, g, b};
}

Color operator+(Color c1, Color c2)
{
	int r = c1.red + c2.red;
	int g = c1.green + c2.green;
	int b = c1.blue + c2.blue;

	r = clamp(r, 0, 255);
	g = clamp(g, 0, 255);
	b = clamp(b, 0, 255);

	return Color{r, g, b};
}

Color operator-(Color c1, Color c2)
{
	int r = c1.red - c2.red;
	int g = c1.green - c2.green;
	int b = c1.blue - c2.blue;

	r = clamp(r, 0, 255);
	g = clamp(g, 0, 255);
	b = clamp(b, 0, 255);

	return Color{r, g, b};
}



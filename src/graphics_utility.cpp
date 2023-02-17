/* graphics_utility.cpp
 *
 * Utility functions for graphics.
 *
 * @author Alex Wills
 * @date February 16, 2023
 */

#include "../lib/graphics_utility.h"
#include <cstdlib>
#include <ctime>


/*
 * Swaps the values of two points
 */
void SwapPoints(Point& p0, Point& p1)
{
	int tempx = p0.x;
	int tempy = p0.y;
	p0.x = p1.x;
	p0.y = p1.y;
	p1.x = tempx;
	p1.y = tempy;
}

/*
 * Generates a random point.
 * NOTE: Currently this does not work.
 */
Point RandomPoint()
{
	int randx, randy;
	randx = rand() % 1000;

	randy = rand() % 1000;
	Point point;
	point.x = randx;
	point.y = randy;
	return point;
}

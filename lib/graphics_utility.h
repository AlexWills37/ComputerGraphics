/* graphics_utility.h
 *
 * Utility functions and structures for computer graphics
 *
 * @author Alex Wills
 * @date February 16, 2023
 */
#ifndef _GRAPHICS_UTILITY_H
#define _GRAPHICS_UTILITY_H

/*
 * Point structure with an x and y value.
 */
struct Point {
	int x, y;
};

struct Color {
	int red, green, blue;
};

void SwapPoints(Point& p0, Point& p1);

Point RandomPoint();

#endif

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
struct Point2D {
	int x, y;
};

struct Point3D {
	float x, y, z;
};


/*
 * Color structure to store RGB values.
 */
struct Color {
	int red, green, blue;
};

Color operator*(Color color, float scalar);


/*
 * Swaps the values of 2 points.
 *
 * @param p0, p1 - the two points to swap
 */
void SwapPoints(Point2D& p0, Point2D& p1);


#endif

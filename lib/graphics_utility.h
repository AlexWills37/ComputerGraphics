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

struct Point3D {
	int x, y, z;
};

/*
 * Simple function to make a point.
 *
 * @param x, y - the x and y coordinates of the point to make
 *
 * @return a Point struct with the specified x and y coordinates
 */
Point MakePoint(int x, int y);

/*
 * Color structure to store RGB values.
 */
struct Color {
	int red, green, blue;
};

/*
 * Simple function to create a color.
 *
 * @param red, green, blue - the RGB values of the color to make
 *
 * @return a Color struct with the RGB values specified
 */
Color MakeColor(int red, int green, int blue);

/*
 * Swaps the values of 2 points.
 *
 * @param p0, p1 - the two points to swap
 */
void SwapPoints(Point& p0, Point& p1);

/*
 * Generate a random point.
 * 
 * @return a point with a random x and y value.
 */
Point RandomPoint();

#endif

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

/*
 * Color structure to store RGB values.
 */
struct Color {
	int red, green, blue;
};

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

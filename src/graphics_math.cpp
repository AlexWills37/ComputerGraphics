/* graphics_math.cpp
 * 
 * Definitions for the math functions used in the grpahics math header.
 * 
 * @author Alex Wills
 * @date February 15, 2023
 */

#include <iostream>
#include <cstdlib>

/*
 * Interpolates a line between a point (i0, d0), and (i1, d1).
 *
 * NOTE: This function does not perform any checks.
 * The destination array must be large enough, or memory will be overwritten
 * out of bounds.
 *
 * @param i0, d0 - the first point of a line segment
 * @param i1, d1 - the second point in a line segment
 * @param destination - a pointer to an array that will hold
 *   the results of this interpolation. There should be 
 *   enough spaces to hold an integer for every integer between
 *   i0 and i1 inclusive.
 */
void Interpolate(int i0, float d0, int i1, float d1, float* destination)
{
	

	// Output list of dependent values in the range [i0, i1] inclusive
	//static int dependent_values[i1 - i0 + 1];
	float* dependent_values = destination;
	float working_dependent = d0;	// This variable will be copied into the values array
	float slope = float(d1 - d0) / float(i1 - i0);	// Slope of the line

	// If i0 > i1, we are starting with the right side of the line and moving left
	// Therefore, the slope should be negated.
	if (i0 > i1)
	{
		slope *= -1.0;
	}

	// Add the dependent value and increment it by the slope
	for (int index = 0; index < abs(i1 - i0) + 1; ++index)
	{
		*(dependent_values + index) = working_dependent;
		working_dependent += slope;
	}
}

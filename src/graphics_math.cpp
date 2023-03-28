/* graphics_math.cpp
 * 
 * Definitions for the math functions used in the grpahics math header.
 * 
 * @author Alex Wills
 * @date February 15, 2023
 */

#include <iostream>
#include <cstdlib>
#include "../lib/graphics.h"
#include <cmath>
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

/*
 * Clamp a value between two other values.
 *
 * @param value - the value to clamp
 * @param low - the lower boundary for the value
 * @param high - the upper boundary for the value
 * 
 * @return either low, value, or high, depending on if the value
 * 	is above high or below low.
 */
float clamp(float value, float low, float high)
{
	if (value < low)
	{
		return low;
	}
	else if (value > high)
	{
		return high;
	}
	else
	{
		return value;
	}
}

/*
 * Multiply two 4x4 transformation matrices
 * NOT a commutative operation.
 * 
 * @param m1 - the first matrix to multiply
 * @param m2 - the second matrix to multiply
 * 
 * @return the result of matrix multiplication
 */
TransformMatrix operator*(TransformMatrix m1, TransformMatrix m2)
{
	TransformMatrix output;
	float sum = 0;	// Sum for dot product
	// To multiply matrices, the output element is the dot product of the row from m1 and the column from m2
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			sum = 0;
			// Get the dot product of the row from m1 and the column from m2
			// We know there will be 4 elements
			for (int i = 0; i < 4; ++i)
			{
				sum += m1.data[row][i] * m2.data[i][col];
			}
			output.data[row][col] = sum;
		}
	}

	return output;
}


/*
 * Applies the effects of a transformation to a set of coordinates.
 * 
 * Applies a transformation matrix (4x4) on homogenous coordinates (4x1) with matrix multiplication.
 */
HomCoordinates operator*(TransformMatrix transform, HomCoordinates point)
{
	HomCoordinates output;
	float sum = 0;	// Sum for dot product
	for (int row = 0; row < 4; ++row)
	{
		sum = 0;
		for (int i = 0; i < 4; ++i)
		{
			sum += transform.data[row][i] * point.data[i];
		}
		output.data[row] = sum;
	}
	return output;
}
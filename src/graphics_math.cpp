/* graphics_math.cpp
 * 
 * Definitions for the math functions used in the grpahics math header.
 * 
 * @author Alex Wills
 * @date February 15, 2023
 */

#include <iostream>
#include <cstdlib>
#include "../lib/graphics_math.h"
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
TransformationMatrix operator*(TransformationMatrix m1, TransformationMatrix m2)
{
	TransformationMatrix output;
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

Transform::operator TransformationMatrix() const
{
	std::cout << "Blarbling the transform: Rotation:" << std::endl;
	std::cout << "x: " << this->rotation[0] << " y: " << this->rotation[1] << " z: " << this->rotation[2] << std::endl; 
	// First initialize all matrices
	TransformationMatrix xRot, yRot, zRot, scale, translation;
	
	// FIRST: Set all array values to 0
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			xRot.data[row][col] = 0;
			yRot.data[row][col] = 0;
			zRot.data[row][col] = 0;
			scale.data[row][col] = 0;
			translation.data[row][col] = 0;
		}
	}

	// X rotation:
	xRot.data[0][0] = 1;
	xRot.data[1][1] = std::cos(this->rotation[0]);
	xRot.data[1][2] = std::sin(this->rotation[0]);
	xRot.data[2][1] = - std::sin(this->rotation[0]);
	xRot.data[2][2] = std::cos(this->rotation[0]);
	xRot.data[3][3] = 1;

	// Y rotation:
	yRot.data[1][1] = 1;
	yRot.data[0][0] = std::cos(this->rotation[1]);
	yRot.data[0][2] = - std::sin(this->rotation[1]);
	yRot.data[2][0] = std::sin(this->rotation[1]);
	yRot.data[2][2] = std::cos(this->rotation[1]);
	yRot.data[3][3] = 1;

	// Z rotation:
	zRot.data[2][2] = 1;
	zRot.data[0][0] = std::cos(this->rotation[2]);
	zRot.data[1][0] = - std::sin(this->rotation[2]);
	zRot.data[0][1] = std::sin(this->rotation[2]);
	zRot.data[1][1] = std::cos(this->rotation[2]);
	zRot.data[3][3] = 1;

	// Combine all rotation
	TransformationMatrix rotation = (zRot * yRot) * xRot;
	// Put a 1 in the bottom right corner
	rotation.data[3][3] = 1;

	// Scale matrix: scale along diagonal
	for (int i = 0; i < 3; ++i)
	{
		scale.data[i][i] = this->scale[i];
	}
	// 1 in the bottom right corner
	scale.data[3][3] = 1;

	// Translation matrix: identity matrix with translation in the 4th column
	for (int i = 0; i < 3; ++i)
	{
		translation.data[i][i] = 1;
		translation.data[i][3] = this->translation[i];
	}
	translation.data[3][3] = 1;

	// Return the final transformation, doing translation last (we are premultiplying)
	return translation * (rotation * scale);

}

/*
 * Applies a transformation matrix (4x4) on homogenous coordinates (4x1) with matrix multiplication
 */
HomCoordinates operator*(TransformationMatrix transform, HomCoordinates point)
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

TransformationMatrix invert(TransformationMatrix original)
{
	float det = 0;

}
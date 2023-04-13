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
#include <array>
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

HomCoordinates operator+(HomCoordinates p0, HomCoordinates p1)
{
	HomCoordinates output;
	output.data[0] = p0.data[0] + p1.data[0];
	output.data[1] = p0.data[1] + p1.data[1];
	output.data[2] = p0.data[2] + p1.data[2];

	return output;
}

HomCoordinates operator-(HomCoordinates p0, HomCoordinates p1)
{
	HomCoordinates output;
	output.data[0] = p0.data[0] - p1.data[0];
	output.data[1] = p0.data[1] - p1.data[1];
	output.data[2] = p0.data[2] - p1.data[2];

	return output;
}

HomCoordinates operator*(float f, HomCoordinates p1)
{
	HomCoordinates output;
	output.data[0] = f * p1.data[0];
	output.data[1] = f * p1.data[1];
	output.data[2] = f * p1.data[2];

	return output;
}

HomCoordinates operator*(HomCoordinates p0, float f)
{
	return f * p0;
}

TransformMatrix BuildRotationMatrix(float x, float y, float z)
{
	// First initialize all matrices
	TransformMatrix xRot, yRot, zRot;
	
	// FIRST: Set all array values to 0
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			xRot.data[row][col] = 0;
			yRot.data[row][col] = 0;
			zRot.data[row][col] = 0;
		}
	}

	// X rotation:
	xRot.data[0][0] = 1;
	xRot.data[1][1] = std::cos(x);
	xRot.data[1][2] = std::sin(x);
	xRot.data[2][1] = - std::sin(x);
	xRot.data[2][2] = std::cos(x);
	xRot.data[3][3] = 1;

	// Y rotation:
	yRot.data[1][1] = 1;
	yRot.data[0][0] = std::cos(y);
	yRot.data[0][2] = - std::sin(y);
	yRot.data[2][0] = std::sin(y);
	yRot.data[2][2] = std::cos(y);
	yRot.data[3][3] = 1;

	// Z rotation:
	zRot.data[2][2] = 1;
	zRot.data[0][0] = std::cos(z);
	zRot.data[1][0] = - std::sin(z);
	zRot.data[0][1] = std::sin(z);
	zRot.data[1][1] = std::cos(z);
	zRot.data[3][3] = 1;

	// Combine all rotation
	TransformMatrix rotation = (zRot * yRot) * xRot;
	// Put a 1 in the bottom right corner
	rotation.data[3][3] = 1;

	return rotation;
}

void NormalizeVector(std::array<float, 3>& vec)
{
	// Get magnitude
	float magnitude = sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));
	if (magnitude != 0) {
		vec[0] /= magnitude;
		vec[1] /= magnitude;
		vec[2] /= magnitude;
	}
}


Plane::Plane(float x, float y, float z, float d)
{
	std::array<float, 3> normal = {x, y, z};
	NormalizeVector(normal);
	this->normal = normal;
	this->constant = d;
}

Plane::Plane(const Plane & to_copy)
{
	this->normal = to_copy.normal;
	this->constant = to_copy.constant;
}

float Plane::SignedDistance(HomCoordinates point)
{
	return (
		point.data[0] * normal[0] +
		point.data[1] * normal[1] +
		point.data[2] * normal[2] +
		constant
	);
}

HomCoordinates Plane::Intersection(HomCoordinates pA, HomCoordinates pB)
{
	HomCoordinates diff = pA - pB;
	float t = (
		(-this->constant - (normal[0] * pA.data[0] + normal[1] * pA.data[1] + normal[2] * pA.data[2])) /
		(normal[0] * diff.data[0] + normal[1] * diff.data[1] + normal[2] * diff.data[2])
	);

	return pA + (t * diff);

}
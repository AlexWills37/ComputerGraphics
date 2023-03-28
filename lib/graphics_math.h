/* graphics_math.h
 *
 * This file contains mathematical functions to be used by the graphics library.
 *
 * @author Alex Wills
 * @date February 15 2023
 */
#ifndef _GRAHPICS_MATH_H
#define _GRAPHICS_MATH_H


/*
 * Interpolate a line between two variables.
 * For every integer between i0 and i1 (independent variables),
 * this funciton returns the coresponding dependent variable
 * based on the line created by two points.
 *
 * @param i0 - independent variable at point 0
 * @param d0 - dependent variable at point 0
 * @param i1 - independent vairable at point 1
 * @param d1 - dependent variable at point 1
 * @param destination - a pointer to the beginning of an integer array to store 
 *   the results of this function
 */
void Interpolate(int i0, float d0, int i1, float d2, float* destination);

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
float clamp(float value, float low, float high);

/*
 * 4x4 matrix to represent a transform.
 * 
 * To apply a transform to a set of coordinates, multiply the matrix
 * with a 4x1 matrix representing the coordinates. This is handled in
 * the HomCoordinates struct.
 * 
 * Always multiply with the TransformMatrix on the left side, and the HomCoordinates
 * on the right.
 */
struct TransformMatrix {
    float data[4][4];
};

/*
 * 4x1 matrix (vector) to represent homogenous coordinates.
 * This is most often a 3D point ( the fourth element != 0 ),
 * but can also be a vector ( the fourth element == 0 ).
 */
struct HomCoordinates {
    float data[4];
};


/*
 * Multiplies two 4x4 matrices (TransformMatrix objects).
 * NOT commutative. A * B != B * A
 */
TransformMatrix operator*(TransformMatrix, TransformMatrix);

/*
 * Applies a transformation on a set of coordinates.
 * This is done by multiplying a 4x4 TransformMatrix with a 4x1 Homogenous Coordinates matrix.
 */
HomCoordinates operator*(TransformMatrix, HomCoordinates);


#endif

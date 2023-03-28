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

struct TransformationMatrix {
    float data[4][4];
};

class Transform {
    public:
	float scale[3];
	float rotation[3];
	float translation[3];

    // Constructors
    Transform(float sx, float sy, float sz, float rx, float ry, float rz, float tx, float ty, float tz)
    {
        scale[0] = sx;
        scale[1] = sy;
        scale[2] = sz;
        rotation[0] = rx;
        rotation[1] = ry;
        rotation[2] = rz;
        translation[0] = tx;
        translation[1] = ty;
        translation[2] = tz;
    }

    Transform() : Transform(1, 1, 1, 0, 0, 0, 0, 0, 0)
    {}

    operator TransformationMatrix() const;
};




struct HomCoordinates {
    float data[4];
};

TransformationMatrix GetTransformMatrix(Transform);

TransformationMatrix operator*(TransformationMatrix, TransformationMatrix);

HomCoordinates operator*(TransformationMatrix, HomCoordinates);

TransformationMatrix invert(TransformationMatrix);

#endif

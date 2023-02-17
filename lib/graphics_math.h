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



#endif

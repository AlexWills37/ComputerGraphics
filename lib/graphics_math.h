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
 *
 * @return - a list of dependent variable values corresponding to the values
 *  betewen the independent variables.
 */
int*  Interpolate(int i0, int d0, int i1, int d2, int* destination);



#endif

/* graphics_line.cpp
 * Defines functions for creating a line
 * 
 * @author Alex Wills
 * @date February 16, 2023
 */

#include <cstdlib>

#include "../lib/graphics_math.h"
#include "../lib/graphics.h"

/*
 * Draws a line on the graphics manager from point 0 to point 1.
 *
 * @param x0, y0 - the first point of the line
 * @param x1, y1 - the second point of the line
 */
void GraphicsManager::DrawLine(int x0, int y0, int x1, int y1)
{
    int deltax = x1 - x0;
	int deltay = y1 - y0;
	
	if (abs(deltax) > abs(deltay))
	{
		// Line is mostly horizontal
		// Ensure x0 < x1
		if (x0 > x1)
		{
			int tempx = x0;
			int tempy = y0;
			x0 = x1;
			y0 = y1;
			x1 = tempx;
			y1 = tempy;
		}

		// Make x the dependent variable and interpolate
		int num_values = abs(x1 - x0) + 1;
		float y_values[num_values];
		float* y_values_ptr = &(y_values[0]);
		Interpolate(x0, y0, x1, y1, y_values_ptr);

		// Draw the line
		for (int i = 0; i < num_values; ++i)
		{
		    this->PutPixel(x0 + i, int(y_values[i]));
		}
	}
	else
	{
		// Line is mostly vertical
		// Ensure y0 < y1
		if (y0 > y1)
		{
			int tempx = x0;
			int tempy = y0;
			x0 = x1;
			y0 = y1;
			x1 = tempx;
			y1 = tempy;
		}

		// Make y the dependent variable and interpolate
		int num_values = abs(y1 - y0) + 1;
		float x_values[num_values];
		float* x_values_ptr = &(x_values[0]);
		Interpolate(y0, x0, y1, x1, x_values_ptr);

		// Draw the line
		for (int i = 0; i < num_values; ++i)
		{
			this->PutPixel(int(x_values[i]), y0 + i);
		}
	}
}

/* graphics_line.cpp
 * Defines functions for creating a line
 * 
 * @author Alex Wills
 * @date February 16, 2023
 */

#include <cstdlib>

#include "../lib/graphics.h"

/*
 * Draws a line on the graphics manager from point 0 to point 1.
 *
 * @param p0 - the point where the line begins
 * @param p1 - the point where the line ends
 */
void GraphicsManager::DrawLine(Point2D p0, Point2D p1)
{
    int deltax = p1.x - p0.x;
	int deltay = p1.y - p0.y;
	
	if (abs(deltax) > abs(deltay))
	{
		// Line is mostly horizontal
		// Ensure x0 < x1
		if (p0.x > p1.x)
		{
			SwapPoints(p0, p1);
		}

		// Make x the dependent variable and interpolate
		int num_values = p1.x - p0.x + 1;
		// TODO: Fix this DUCTTAPE code
		// Manual check to make sure that the line is for real
		if (num_values > 0 && num_values < 10000)
		{
			float y_values[num_values];
			float* y_values_ptr = &(y_values[0]);
			Interpolate(p0.x, p0.y, p1.x, p1.y, y_values_ptr);

			// Draw the line
			for (int i = 0; i < num_values; ++i)
			{
				this->PutPixel(p0.x + i, int(y_values[i]));
			}

		} else 
		{
			// std::cout << "What! Horizontal Line with " << num_values << " points" << std::endl;
			// std::cout << "\tp1 y: " << p1.y << " - p0.y: " << p0.y << std::endl;

		}

	}
	else
	{
		// Line is mostly vertical
		// Ensure y0 < y1
		if (p0.y > p1.y)
		{
			SwapPoints(p0, p1);
		}

		// Make y the dependent variable and interpolate
		int num_values = p1.y - p0.y + 1;
		// TODO: Fix this DUCTTAPE code
		// Manual check to make sure that the line is for real
		if (num_values > 0 && num_values < 10000)
		{

			float x_values[num_values];
			float* x_values_ptr = &(x_values[0]);
			Interpolate(p0.y, p0.x, p1.y, p1.x, x_values_ptr);

			// Draw the line
			for (int i = 0; i < num_values; ++i)
			{
				this->PutPixel(int(x_values[i]), p0.y + i);
			}
		}
		else 
		{
			// std::cout << "What! Vertical Line with " << num_values << " points" << std::endl;
			// std::cout << "\tp1 y: " << p1.y << " - p0.y: " << p0.y << std::endl;
		}
	}
}

/*
 * Draws a line on the graphics manager from point 0 to point 1.
 *
 * @param p0 - the point where the line begins
 * @param p1 - the point where the line ends
 * @param color - the color of the line to draw
 */
void GraphicsManager::DrawLine(Point2D p0, Point2D p1, Color color)
{
	this->ChangeBrushColor(color);
	this->DrawLine(p0, p1);
}
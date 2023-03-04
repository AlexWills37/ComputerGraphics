/* graphics_triangle.cpp
 * 
 * This class defines the functions to be used in drawing triangles.
 *
 * @author Alex Wills
 * @date Februrary 16 2023
 */


#include "../lib/graphics.h"
#include "../lib/graphics_math.h"
#include "../lib/graphics_utility.h"
#include <cstdlib>
#include <iostream>

/*
 * Draws the outline of a triangle.
 * 
 * @param p0, p1, p2 - the three points of the triangle
 */
void GraphicsManager::DrawWireTriangle(Point2D p0, Point2D p1, Point2D p2)
{
	this->DrawLine(p0, p1);
	this->DrawLine(p1, p2);
	this->DrawLine(p2, p0);

}

/*
 * Draws the outline of a triangle.
 * 
 * @param p0, p1, p2 - the three points of the triangle
 * @param color - the color of the triangle
 */
void GraphicsManager::DrawWireTriangle(Point2D p0, Point2D p1, Point2D p2, Color color)
{
	this->ChangeBrushColor(color);
	this->DrawWireTriangle(p0, p1, p2);
}

/*
 * Draws a triangle with the inside filled.
 * The color of the triangle is determined by the curent brush color by default.
 *
 * @param p0 - the first point of the triangle
 * @param p1 - the second point of the triangle
 * @param p2 - the third point of the triangle
 */
void GraphicsManager::DrawFillTriangle(Point2D p0, Point2D p1, Point2D p2)
{
	// Sort points so that p0 has the lowest y value and p2 has the highest
	if (p1.y < p0.y)
	{
		SwapPoints(p1, p0);
	}
	if (p2.y < p0.y)
	{
		SwapPoints(p2, p0);
	}
	if (p2.y < p1.y)
	{
		SwapPoints(p2, p1);
	}
	
	// We will fill in the triangle with horizontal lines.
	// To do this, we need a list of x values for the left side of the lines,
	// and a list of x values for the right side of the lines.
	// One of these sides will be made by the longest edge of the triangle.

	// Long edge is from point 0 to point 2 (the highest and lowest points)
	float long_side_xs[ p2.y - p0.y + 1 ];
	Interpolate(p0.y, p0.x, p2.y, p2.x, &(long_side_xs[0]));

	// Two short edges make the other list of x values.
	// This list should match the long_side_xs in length
	float combined_side_xs[ p2.y - p0.y + 1];

	// Starting from the bottom to match the other list, we compute p0 -> p1.
	Interpolate(p0.y, p0.x, p1.y, p1.x, &(combined_side_xs[0]));

	// We will then add the next segment from p1 -> p2.
	// Note that one point, namely p1, was computed in the previous statement, so we will overwrite it
	int p1_index = p1.y - p0.y;	
	Interpolate(p1.y, p1.x, p2.y, p2.x, &(combined_side_xs[p1_index]));

	// Determine which list is the left side and which is right.
	float* left_list;
	float* right_list;
	if (long_side_xs[p1_index] < combined_side_xs[p1_index])
	{
		left_list = &(long_side_xs[0]);
		right_list = &(combined_side_xs[0]);
	}
	else
	{	
		right_list = &(long_side_xs[0]);
		left_list = &(combined_side_xs[0]);
	}

	// Draw horizontal lines
	for(int y = p0.y; y <= p2.y; ++y)
	{
		for(int x = left_list[y - p0.y]; x <= right_list[y - p0.y]; ++x)
		{
			this->PutPixel(int(x), int(y));
		}
	}
}

/*
 * Draws a triangle with the inside filled.
 *
 * @param p0 - the first point of the triangle
 * @param p1 - the second point of the triangle
 * @param p2 - the third point of the triangle
 * @param color - the color to fill the triangle with
 */
void GraphicsManager::DrawFillTriangle(Point2D p0, Point2D p1, Point2D p2, Color color)
{
	this->ChangeBrushColor(color);
	this->DrawFillTriangle(p0, p1, p2);
}



/*
 * Draws a triangle with a shaded gradient in the middle,
 * determined by the strength of a specified color at each of the three corners.
 *
 * @param p0 - the first point of the triangle
 * @param p1 - the second point of the triangle
 * @param p2 - the third point of the triangle
 * @param color - the color of the gradient
 * @param h0, h1, h2 - the intensity of the color at each point of the triangle (between 0 and 1).
 *   0 specifies black, 1 specifies the color parameter, and the values in between specify a blend.
 */ 
void GraphicsManager::DrawGradientTriangle(Point2D p0, Point2D p1, Point2D p2, Color color, float h0, float h1, float h2)
{

	// Clamp h values to avoid unexpected behavior
	h0 = clamp(h0, 0, 1);
	h1 = clamp(h1, 0, 1);
	h2 = clamp(h2, 0, 1);

	float temph;
	// Sort points so that p0 has the lowest y value and p2 has the highest
	if (p1.y < p0.y)
	{
		SwapPoints(p1, p0);
		temph = h1;
		h1 = h0;
		h0 = temph;
	}
	if (p2.y < p0.y)
	{
		SwapPoints(p2, p0);
		temph = h2;
		h2 = h0;
		h0 = temph;
	}
	if (p2.y < p1.y)
	{
		SwapPoints(p2, p1);
		temph = h2;
		h2 = h1;
		h1 = temph;
	}
	
	// We will fill in the triangle with horizontal lines.
	// To do this, we need a list of x values for the left side of the lines,
	// and a list of x values for the right side of the lines.
	// One of these sides will be made by the longest edge of the triangle.

	// Long edge is from point 0 to point 2 (the highest and lowest points)
	float long_side_xs[ p2.y - p0.y + 1 ];
	Interpolate(p0.y, p0.x, p2.y, p2.x, &(long_side_xs[0]));
	
	// Also now we want to get h values for the edges to determine the color of each pixel
	float long_side_hs[ p2.y - p0.y + 1];
	Interpolate(p0.y, h0, p2.y, h2, &(long_side_hs[0]));

	// Two short edges make the other list of x values.
	// This list should match the long_side_xs in length
	float combined_side_xs[ p2.y - p0.y + 1];
	float combined_side_hs[ p2.y - p0.y + 1];

	// Starting from the bottom to match the other list, we compute p0 -> p1.
	Interpolate(p0.y, p0.x, p1.y, p1.x, &(combined_side_xs[0]));
	Interpolate(p0.y, h0, p1.y, h1, &(combined_side_hs[0]));
	
	// We will then add the next segment from p1 -> p2.
	// Note that one point, namely p1, was computed in the previous statement, so we will overwrite it
	int p1_index = p1.y - p0.y;	
	Interpolate(p1.y, p1.x, p2.y, p2.x, &(combined_side_xs[p1_index]));
	Interpolate(p1.y, h1, p2.y, h2, &(combined_side_hs[p1_index]));	

	// Determine which list is the left side and which is right.
	float* left_x_list;
	float* left_h_list;
	float* right_x_list;
	float* right_h_list;
	if (long_side_xs[p1_index] < combined_side_xs[p1_index])
	{
		left_x_list = &(long_side_xs[0]);
		right_x_list = &(combined_side_xs[0]);
		left_h_list = &(long_side_hs[0]);
		right_h_list = &(combined_side_hs[0]);
	}
	else
	{	
		right_x_list = &(long_side_xs[0]);
		left_x_list = &(combined_side_xs[0]);
		left_h_list = &(combined_side_hs[0]);
		right_h_list = &(long_side_hs[0]);
	}

	// We now have a list of x values and h values that correspond to 
	// every y value on the left and right sides of the triangle.

	// Draw horizontal lines
	int y_index;
	int segment_width;
	Color pixel_color;
	for(int y = p0.y; y <= p2.y; ++y)
	{
		y_index = y - p0.y;
		// For each line, interpolate the h values for the color gradient
		segment_width = int(right_x_list[y_index]) - int(left_x_list[y_index]) + 1;
		float h_segment[ segment_width ];
		Interpolate(int(left_x_list[y_index]), left_h_list[y_index],
			int(right_x_list[y_index]), right_h_list[y_index], &(h_segment[0]));

		// Draw the line, changing the color each time
		for(int x = left_x_list[y_index]; x <= right_x_list[y_index]; ++x)
		{
			// Get the index of the segment
			// Unclamped RGB values
			// red = color.red * h_segment[x - int(left_x_list[y_index])];
			// green = color.green * h_segment[x - int(left_x_list[y_index])];
			// blue = color.blue * h_segment[x - int(left_x_list[y_index])];
			pixel_color = color * h_segment[x - int(left_x_list[y_index])];

			this->PutPixel(int(x), int(y), pixel_color);
		}
	}
}

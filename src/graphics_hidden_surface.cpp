/* grpahics_hidden_surface.cpp
 *
 * Definitions for the Hidden Surface Removal chatper. Includes drawing triangles with a depth buffer and
 * backface culling.
 * 
 * @author Alex Wills
 * @date April 27, 2023
 */

#include "../lib/graphics.h"

/*
 * Draws a triangle to the screen, respecting the depth buffer and only drawing pixels that are closer
 * to the camera than existing pixels.
 */
void GraphicsManager::DrawDepthTriangle(Point2D p0, Point2D p1, Point2D p2, Color color, 
		float h0, float h1, float h2)
{
    // NOTE: In this function, the h-values correspond to 1/z for the points, representing their
    // depth.

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

    float z_val;

	for(int y = p0.y; y <= p2.y; ++y)
	{
		y_index = y - p0.y;
		// For each line, interpolate the h values for the depth
		segment_width = int(right_x_list[y_index]) - int(left_x_list[y_index]) + 1;
		float h_segment[ segment_width ];

        // Interpolate the x values on this line with the h (depth) values
		Interpolate(int(left_x_list[y_index]), left_h_list[y_index],
			int(right_x_list[y_index]), right_h_list[y_index], &(h_segment[0]));

		// Draw the line, checking the depth of each pixel
		for(int x = left_x_list[y_index]; x <= right_x_list[y_index]; ++x)
		{
			z_val = h_segment[x - int(left_x_list[y_index])];
            if (z_val > this->depth_buffer(x, y))   // Higher 1/z value means lower z, closer to camera than existing pixel
            {
                this->PutPixel(x, y, color);
                this->depth_buffer(x, y) = z_val;
            }
		}
	}

}
/* graphics_polygon.cpp
 * 
 * File for handline polygon draws beyond a triangle.
 * 
 * This file is currently deprecated because it goes beyond the scope
 * of this course and is not necessary (not to mention it does not always
 * work).
 *
 * @author Alex Wills
 * @date February 19, 2023
 */

#include "../lib/graphics.h"

enum Direction { left, right };

void GraphicsManager::DrawFillQuad(Point2D p0, Point2D p1, Point2D p2, Point2D p3, Color color)
{
	
	this->ChangeBrushColor(color.red, color.green, color.blue);	

	// Sort Point2Ds by height
	if (p0.y > p3.y)
	{
		SwapPoints(p0, p3);
	}
	if (p1.y > p3.y)
	{
		SwapPoints(p1, p3);
	}
	if (p2.y > p3.y)
	{
		SwapPoints(p2, p3);
	} // p3 is now the highest Point2D

	if (p0.y > p2.y)
	{
		SwapPoints(p0, p2);
	}
	if (p1.y > p2.y)
	{
		SwapPoints(p0, p2);
	} // p2 and p3 are sorted

	if (p0.y > p1.y)
	{
		SwapPoints(p0, p1);
	}
	// All Point2Ds are now sorted

	// Next we must determine what Point2Ds are opposite of each other.
	// My claim is that we can determine this by looking at the 
	// relative x positions of each Point2D in order.
	// THIS IS WRONG
	/*    |  Case 1  |  Case 2  |  Case 3  | Case 4  |
	 * p3 |     .    |    .     |  .       |.        |
	 * p2 |    .     |       .  |    .     | .       |
	 * p1 |       .  |   .      |        . |   .     |
	 * p0 |     .    |     .    |  .       |       . |
	 */
	
	// In case 1 and 2, p0 and p3 are opposites, and p1 and p2 are opposites.
	// In case 1, the Point2Ds move Left, Right, Left. In case 2, the Point2Ds move Right, Left, Right.
	// (Movement determined by looking at a Point2D's x value related to the x value of the Point2D above it)
	// Seemingly, in all other cases, p0 and p2 are opposites, and p1 and p3 are opposites.
	
	Direction first_direction, second_direction, third_direction;
	if (p3.x > p2.x)
	{
		first_direction = left;
	} else
	{
		first_direction = right;
	}

	if (p2.x > p1.x)
	{
		second_direction = left;
	} else 
	{
		second_direction = right;
	}

	if (p1.x > p0.x)
	{
		third_direction = left;
	} else 
	{
		third_direction = right;
	}

	// Now we can check for the special case, where p0 and p3 are opposites, and p1 and p2 are opposites
	if (first_direction != second_direction && first_direction == third_direction)
	{
		// In this situation, p0 and p3 are opposites. p1 and p2 are opposites.
		/* p0     . 
		 * p1 .
		 * p2      .
		 * p3   .
		 */
		this->DrawFillTriangle(p0, p1, p2);
		this->DrawFillTriangle(p3, p1, p2);
	} else 
	{
		// In this situation, p0 and p2 are opposites, and p1 and p3 are opposites.
		/* p0    .
		 * p1       .
		 * p2       .
		 * p3   .
		 */
		 this->DrawFillTriangle(p0, p1, p2);
		 this->DrawFillTriangle(p0, p3, p2);
	}
}

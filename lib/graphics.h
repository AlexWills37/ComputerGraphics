/* graphics.h
 * 
 * Header file for the general graphics library created in the
 * Computer Graphics Tutorial with Professor Lepinski.
 *
 * USAGE:
 * 1) Include this file in a program
 * 2) Instantiate a GraphicsManager object
 * 3) Call OpenWindow(int width, int height)
 * 4) > insert all of your wonderful graphics program here <
 * 5) Call CloseWindow() to finish
 *
 * @author Alex Wills
 * @date February 14, 2023
 */
#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <iostream>
#include "SDL2/SDL.h"
#include "graphics_utility.h"


/*
 * Graphics Manager class to handle graphics calls.
 * This header is an interface between user-friendly graphics methods
 * and complicated backend logic.
 */
class GraphicsManager
{
public:
	GraphicsManager()
	{
		std::cout << "~ Creating Graphics Manager" << std::endl;
	}

private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event_handler;

	int viewport_width;
	int viewport_height;

public:
	/*
	 * Opens and initializes a graphics window.
	 *
	 * @param width - the width of the window in pixels
	 * @param height - the height of the window in pixels
	 */
	void OpenWindow(int width, int height);
	
	/*
	 * Closes the window and deactivates SDL.
	 */
	void CloseWindow();
	
	/*
	 * Keeps the window open by blocking the program.
	 */
	void StayOpenBlocking();
	
	/*
	 * Places a pixel on the screen at (x, y).
	 * 
	 * @param x - the x coordinate (0 is the leftmost column)
	 * @param y - the y coordinate (0 is the topmost row)
	 */
	void PutPixel(int x, int y);
	
	/*
	 * Changes the color to draw pixels with.
	 * 
	 * @param r - the red value of the color [0, 255]
	 * @param g - the green value of the color [0, 255]
	 * @param b - the blue value of the color [0, 255]
	 * @param a (optional) - the alpha value of the color [0, 255] (default 255 = opaque)
	 */
	void ChangeBrushColor(int r, int g, int b, int a = SDL_ALPHA_OPAQUE);
	
	/*
	 * Updates the screen to reflect any changes made by placing pixels.
	 */
	void RefreshScreen();
	
	/*
	 * Places a line on the screen between two points.
	 *
	 * @param x1 - the x coordinate of point 1
	 * @param y1 - the y coordinate of point 1
	 * @param x2 - the x coordinate of point 2
	 * @param y2 - the y coordinate of point 2
	 */
	void DrawLine(int x1, int y1, int x2, int y2);
	
	/*
	 * Draws the outline of a triangle between three points (the center will not be filled).
	 *
	 * @param point0x, point0y - the x and y values of point 0 of the triangle
	 * @param point1x, point1y - the x and y values of point 1 of the triangle
	 * @param point2x, point2y - the x and y values of point 2 of the triangle
	 */
	void DrawWireTriangle(int point0x, int point0y, int point1x, int point1y, int point2x, int point2y);
	
	/*
	 * Draws a triangle between three points with the center filled in.
	 * Uses the color currently selected by the brush.
	 *
	 * @param p0 - the first point in the triangle
	 * @param p1 - the second point in the triangle
	 * @param p2 - the third point in the triangle
	 */
	void DrawFillTriangle(Point p0, Point p1, Point p2);
	
	/*
	 * Draws a triangle between three points and fills in the center with a gradient.
	 *
	 * @param p0, p1, p2 - the three points of the triangle
	 * @param color - the color to define the gradient
	 * @param h0, h1, h2 - the intensity of each point [0, 1]. 0 represents black on the gradient,
	 *   and 1 represents the chosen color on the gradient.
	 */
	void DrawGradientTriangle(Point p0, Point p1, Point p2, Color color, float h1, float h2, float h3);
	
	void DrawFillQuad(Point p0, Point p1, Point p2, Point p3, Color color);

};




#endif // _GRPAHICS_H //

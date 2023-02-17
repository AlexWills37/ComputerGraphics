/* graphics.h
 * 
 * Header file for the general graphics library created in the
 * Computer Graphics Tutorial with Professor Lepinski.
 *
 * @author Alex Wills
 * @date February 14, 2023
 */
#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <iostream>
#include "SDL2/SDL.h"
#include "graphics_utility.h"


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
public:
	/*
	 * Open and initialize a graphics window
	 */
	void OpenWindow(uint16_t width, uint16_t height);
	

	void CloseWindow();
	void StayOpenBlocking();
	void PutPixel(int x, int y);
	void ChangeBrushColor(int r, int g, int b, int a = SDL_ALPHA_OPAQUE);
	void RefreshScreen();
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawWireTriangle(int point0x, int point0y, int point1x, int point1y, int point2x, int point2y);
	void DrawFillTriangle(Point p0, Point p1, Point p2);
	void DrawGradientTriangle(Point p0, Point p1, Point p2, Color c, float h1, float h2, float h3);
};




#endif // _GRPAHICS_H //

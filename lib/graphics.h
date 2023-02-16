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

struct Color {
	uint8_t red, green, blue, alpha;
};

Color NewColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);

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
};




#endif // _GRPAHICS_H //

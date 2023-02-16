/* graphics.cpp
 * ~~~~~~~~~~~~
 * A custom grpahics library for Lepinski's Computer Graphics tutorial.
 * This code contains the primitive functions that will be used for everything.
 */

#include <cstdlib>
#include <iostream>
#include <SDL2/SDL.h>
#include "../lib/graphics.h"
#include "../lib/graphics_math.h"

Color NewColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	Color color;
	color.red = red;
	color.green = green;
	color.blue = blue;
	color.alpha = alpha;
	return color;
}

/*
 * Create a graphics window and initialize SDL
 *
 * @param width: the width of the window, in pixels
 * @param height: the height of the window, in pixels
 */
void GraphicsManager::OpenWindow(uint16_t width, uint16_t height)
{
	SDL_Init(SDL_INIT_VIDEO);
	//SDL_CreateWindowAndRenderer(width, height, 0, &(this->window), &(this->renderer) );

	window = SDL_CreateWindow("Graphics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	if(renderer == NULL)
	{
		std::cout << "!!ERROR: " << SDL_GetError() << std::endl;
	}


}

/* 
 * Close the window and shut down SDL
 */
void GraphicsManager::CloseWindow()
{
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

/*
 * Keeps the window open until the user closes it. This is a blocking funciton.
 */
void GraphicsManager::StayOpenBlocking()
{
	
	bool running = true;
	while(running)
	{
		// Go through the event queue
		while ( SDL_PollEvent(&event_handler) )
		{
			// Handle the event
			switch (event_handler.type)
			{	
				// Quit event
				case SDL_QUIT:
					running = false;
					std::cout << "~ User has closed the window." << std::endl;
					break;
				default:
					break;
			}
		}
	}
}

/*
 * Place a pixel on the screen at a given location.
 */
void GraphicsManager::PutPixel(int x, int y)
{
	SDL_RenderDrawPoint(renderer, x, y);			
}

/*
 * Change the color that the renderer will draw with
 */
void GraphicsManager::ChangeBrushColor(int red, int green, int blue, int alpha)
{
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
}

/*
 * Update the renderer
 */
void GraphicsManager::RefreshScreen()
{
	SDL_RenderPresent(renderer);
}

/*
 * Draw a line between two points.
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
		int num_values = x1 - x0 + 1;
		int y_values[num_values];
		int* y_values_ptr = &(y_values[0]);
		Interpolate(x0, y0, x1, y1, y_values_ptr);

		// Draw the line
		for (int i = 0; i < num_values; ++i)
		{
			this->PutPixel(x0 + i, y_values[i]);
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
		int num_values = y1 - y0 + 1;
		int x_values[num_values];
		int* x_values_ptr = &(x_values[0]);
		Interpolate(y0, x0, y1, x1, x_values_ptr);

		// Draw the line
		for (int i = 0; i < num_values; ++i)
		{
			this->PutPixel(x_values[i], y0 + i);
		}
	}

}

int tmain()
{
	// Testing function	
	GraphicsManager test = GraphicsManager();
	test.OpenWindow(1000, 1000);
	
//	int* y_values = Interpolate(0, 0, 9, 40);
//	for(int i = 0; i < 10; ++i)
//	{
//		std::cout << "p + i = " << y_values + i << ". " << "(" << i << ", " << *(y_values + i) << ")\n";	
//	}

	test.ChangeBrushColor(255, 255, 255);
	
	test.DrawLine(0, 0, 500, 500);

	test.DrawLine(300, 300, 1000, 200);
	test.DrawLine(200, 0, 200, 1000);
	test.DrawLine(0, 700, 1000, 700);
	
	test.RefreshScreen();

	test.StayOpenBlocking();
	test.CloseWindow();
	return 0;
}

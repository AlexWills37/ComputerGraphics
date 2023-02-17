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



/*
 * Create a graphics window and initialize SDL
 *
 * @param width: the width of the window, in pixels
 * @param height: the height of the window, in pixels
 */
void GraphicsManager::OpenWindow(int width, int height)
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
//	std::cout << "Placing pixel at (" << x << ", " << y << ")" << std::endl;
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

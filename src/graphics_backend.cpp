/* graphics_backend.cpp
 *
 * This file defines the backend for the graphics manager system.
 * Whatever library this program uses, this file should define the 
 * functions to open a window on the user's screen and place a pixel at 
 * a given point.
 * Currently uses SDL2.
 * 
 * @author Alex Wills
 * @date March 3, 2023
 */

#include <cstdlib>
#include <iostream>
#include <SDL2/SDL.h>
#include "../lib/graphics.h"

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

	// Also set viewport dimensions
	this->viewport_width = width / 50;
	this->viewport_height = height / 50;
	this->viewport_distance = 5;	// This default viewport distance is arbitrary

    this->canvas_width = width;
    this->canvas_height = height;
    
    // Calculate minimum x and y coordinates for faster calculations
    this->max_screen_x = width / 2;
    this->max_screen_y = height / 2;

    std::cout << "~ Screen dimensions: starting at (" << this->max_screen_x << ", " << this->max_screen_y
        << ") and ending at (" << (this->max_screen_x - this->canvas_width) << ", "
        << (this->max_screen_y - this->canvas_height) << ")" << std::endl;

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
 *
 * @param x, y - coordinates of the point to place the pixel ((0, 0) is the center of the screen)
 */
void GraphicsManager::PutPixel(int x, int y)
{
//	std::cout << "Placing pixel at (" << x << ", " << y << ")" << std::endl;
	
    // Check input and print out warning if pixel is out of screen
    if ( x < -(this->max_screen_x) || x > this->max_screen_x
		|| y < -(this->max_screen_y) || y > this->max_screen_y)
    {
        // std::cout << "!!WARNING: Attempting to place a pixel outside of the screen: (" << x
        //     << ", " << y << ")" << std::endl; 
    }
	else 
	{
	    SDL_RenderDrawPoint(renderer, this->max_screen_x + x, this->max_screen_y - y);
	}
	// Pixel (0, 0) (top left) == World space (-(width / 2), (width / 2)) 
	// Pixel (width - 1, height - 1) == World space ( (width / 2) - 1, 1 -(width / 2))

}


/*
 * Change the color that the renderer will draw with
 * 
 * @param red, green, blue - the RGB values of the color to draw with
 * @param alpha (default 255) - the opacity of the color to draw with
 */
void GraphicsManager::ChangeBrushColor(int red, int green, int blue, int alpha)
{
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
}

/*
 * Update the renderer on the user's end
 */
void GraphicsManager::RefreshScreen()
{
	SDL_RenderPresent(renderer);
}

/*
 * Clear the user's screen
 */
void GraphicsManager::ClearScreen()
{
	SDL_RenderClear(renderer);
}


/*
 * Keeps the user's window open and allows the user to move the camera in the scene.
 * Use WASD to move on the X/Z plane, E and Q to move up/down on the Y axis,
 * and the arrow keys or IJKL to rotate up/down and left/right.
 */
void GraphicsManager::StayOpenCameraControls()
{
	

	bool running = true;
	SDL_Keysym key;
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
				case SDL_KEYDOWN:
					key = event_handler.key.keysym;

					//std::cout << "~ User has pressed key: " << key.sym << std::endl;
					switch (key.sym)
					{
						// Move left
						case SDLK_a:
							this->main_camera.MovePosition(-0.1, 0, 0);
							break;
						// Move back
						case SDLK_s:
							this->main_camera.MovePosition(0, 0, -0.1);
							break;
						// Move right
						case SDLK_d:
							this->main_camera.MovePosition(0.1, 0, 0);
							break;
						// Move forward
						case SDLK_w:
							this->main_camera.MovePosition(0, 0, 0.1);
							break;
						// Move down
						case SDLK_q:
							this->main_camera.MovePosition(0, -0.1, 0);
							break;

						// Move up
						case SDLK_e:
							this->main_camera.MovePosition(0, 0.1, 0);
							break;

						// Rotate up
						case SDLK_i:
							// Fall through
						case SDLK_UP:
							this->main_camera.Rotate(3.14/60, 0, 0);
							break;

						// Rotate down
						case SDLK_k:
							// Fall through
						case SDLK_DOWN:
							this->main_camera.Rotate(-3.14/60, 0, 0);
							break;
						// Rotate counter-clockwise
						case SDLK_j:
							// Fall through
						case SDLK_LEFT:
							this->main_camera.Rotate(0, 3.14/60, 0);
							break;
						
						// Rotate clockwise
						case SDLK_l:
							// Fall through
						case SDLK_RIGHT:
							this->main_camera.Rotate(0, -3.14/60, 0);
						default:
							// Do nothing if key not recognized
							break;
					}

					

					break;
				default:
					break;
			}
		}

		// Clear screen
		this->ChangeBrushColor(BLACK);
		this->ClearScreen();
		// Render new scene
		this->current_scene.RenderScene();
		this->RefreshScreen();

	}
}


/*************************************************************/
/*    Graphics functions that do not depend on the backend   */
/*************************************************************/

// Put pixel overloads
void GraphicsManager::PutPixel(int x, int y, Color color)
{
    this->ChangeBrushColor(color.red, color.green, color.blue);
    this->PutPixel(x, y);
}

void GraphicsManager::PutPixel(Point2D point)
{
    this->PutPixel(point.x, point.y);
}

void GraphicsManager::PutPixel(Point2D point, Color color)
{
    this->PutPixel(point.x, point.y, color);
}

// Change Brush Color overload
void GraphicsManager::ChangeBrushColor(Color color)
{
	this->ChangeBrushColor(color.red, color.green, color.blue);
}

void GraphicsManager::ClearScreen(Color color)
{
	this->ChangeBrushColor(color);
	this->ClearScreen();
}
/* PutPixel test for computer graphics
 *
 *
 * @author Alex Wills
 * @date February 8 2023
 */

#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int main()
{

	
	// Pointer to hold the window
	SDL_Window* window = nullptr;

	// Pointer to the canvas
	SDL_Surface* canvas = nullptr;

	// Activate SDL in video mode
	SDL_Init( SDL_INIT_VIDEO );

	// Create the window
	window = SDL_CreateWindow( "SDL Tutorial (STOLEN CODE)", SDL_WINDOWPOS_CENTERED, 0, 2000, 1000, SDL_WINDOW_SHOWN);
	
	canvas = SDL_GetWindowSurface( window );

	// Storage for event object to decipher
	SDL_Event event;

	// Try drawing image
	SDL_Surface* image = SDL_LoadBMP("img.bmp");
	if(image == NULL){
		cout << "ERROR: Couldn't load image :( \n";
	} else {
		cout << "Image loaded :) \n";
	}
	
	// Put image on canvas
	SDL_BlitSurface(image, NULL, canvas, NULL);

	// PIXEL THINGS
	//SDL_PixelFormat* format;
	//SDL_Color* color;
	//Uint8 index;

	//format = canvas->format;
	//cout << "Format: " << format << "\n";
	//cout << "Bits per pixel: " << format->BitsPerPixel << "\n";
	//
	//SDL_LockSurface(canvas);
	//index = *(Uint8*)canvas->pixels;
	//cout << "Index: " << index << "\n";
	//color = &format->palette->colors[index];
	//cout << "Color: " << color << "\n";

	//SDL_UnlockSurface(canvas);


	cout << "Width: " << canvas->w << " Height: " << canvas->h << "\n";
	cout << "Must lock surface? " << SDL_MUSTLOCK(canvas) << "\n";
	Uint8* pixel = (Uint8*)canvas->pixels;

	SDL_PixelFormat* format = canvas->format;
	cout << "Format address: " << &format << "\n";
	cout << "Format value: " << format << "\n";
	cout << "Pixel depth: " << Uint8((format)->BitsPerPixel) << "\n";
	

	SDL_LockSurface(canvas);
	SDL_memset(canvas->pixels, 0, 300);
	SDL_UnlockSurface(canvas);
	


	// Update screen
	SDL_UpdateWindowSurface(window);

	//SDL_FRect rectangle = SDL_FRect(0, 0, 40, 70);


	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}

	// Close window
	SDL_DestroyWindow( window );
	window = nullptr;
	SDL_Quit();
	
	return 0;
}

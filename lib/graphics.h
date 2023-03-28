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
#include <functional>
#include <SDL2/SDL.h>
#include "graphics_math.h"
#include "graphics_utility.h"
#include "graphics_scene.h"

// Color constants
static constexpr Color RED = {255, 0, 0};
static constexpr Color ORANGE = {255, 153, 51};
static constexpr Color YELLOW = {255, 255, 51};
static constexpr Color GREEN = {51, 255, 51};
static constexpr Color BLUE = {0, 255, 255};
static constexpr Color INDIGO = {0, 0, 255};
static constexpr Color PURPLE = {127, 0, 255};
static constexpr Color BLACK = {0, 0, 0};
static constexpr Color WHITE = {255, 255, 255};


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
	// Objects from SDL for handling graphics
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event_handler;

	float viewport_width, viewport_height, viewport_distance;	// Dimensions of the viewport
	int canvas_width, canvas_height;		// Dimsnesions of the canvas (screen/window)
	int max_screen_x, max_screen_y;			// Locations of the top right corner, when the center of the screen is 0,0

	Camera main_camera;		// Where the graphics will be viewed from
	Scene current_scene;	// The current scene


// Functions to be implemented in graphics_backend.cpp
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
	 * Keeps the window open and allows the user to move the camera in the scene
	 * with WASD to move on the X/Z plane, E and Q to move up/down on the Y axis,
	 * and the arrow keys or IJKL to rotate up/down and left/right.
	 */
	void StayOpenCameraControls();
	
	/*
	 * Places a pixel on the screen at (x, y).
	 * 
	 * @param x - the x coordinate (0 is the center)
	 * @param y - the y coordinate (0 is the center)
	 */
	void PutPixel(int x, int y);
	void PutPixel(int x, int y, Color color);
	void PutPixel(Point2D point);
	void PutPixel(Point2D point, Color color);
	
	/*
	 * Changes the color to draw pixels with.
	 * 
	 * @param r - the red value of the color [0, 255]
	 * @param g - the green value of the color [0, 255]
	 * @param b - the blue value of the color [0, 255]
	 * @param a (optional) - the alpha value of the color [0, 255] (default 255 = opaque)
	 */
	void ChangeBrushColor(int r, int g, int b, int a = SDL_ALPHA_OPAQUE);
	void ChangeBrushColor(Color color);
	
	/*
	 * Updates the screen to reflect any changes made by placing pixels.
	 */
	void RefreshScreen();

	/*
	 * Clear the screen.
	 */
	void ClearScreen();
	/*
	 * Clear the screen with a specified color.
	 */
	void ClearScreen(Color color);
	
// 2D Drawing functions
public:
	/*
	 * Places a line on the screen between two points.
	 *
	 * @param point0 - the first point for the line
	 * @param point1 - the second point for the line
	 */
	void DrawLine(Point2D point0, Point2D point1);
	void DrawLine(Point2D point0, Point2D point1, Color color);
	
	/*
	 * Draws the outline of a triangle between three points (the center will not be filled).
	 *
	 * @param point0 - the x and y values of point 0 of the triangle
	 * @param point1 - the x and y values of point 1 of the triangle
	 * @param point2 - the x and y values of point 2 of the triangle
	 */
	void DrawWireTriangle(Point2D point0, Point2D point1, Point2D point2);
	void DrawWireTriangle(Point2D point0, Point2D point1, Point2D point2, Color color);
	
	/*
	 * Draws a triangle between three points with the center filled in.
	 * Uses the color currently selected by the brush.
	 *
	 * @param p0 - the first point in the triangle
	 * @param p1 - the second point in the triangle
	 * @param p2 - the third point in the triangle
	 */
	void DrawFillTriangle(Point2D p0, Point2D p1, Point2D p2);
	void DrawFillTriangle(Point2D p0, Point2D p1, Point2D p2, Color color);
	
	/*
	 * Draws a triangle between three points and fills in the center with a gradient.
	 *
	 * @param p0, p1, p2 - the three points of the triangle
	 * @param color - the color to define the gradient
	 * @param h0, h1, h2 - the intensity of each point [0, 1]. 0 represents black on the gradient,
	 *   and 1 represents the chosen color on the gradient.
	 */
	void DrawGradientTriangle(Point2D p0, Point2D p1, Point2D p2, Color color, float h1, float h2, float h3);

// 3D Drawing
public:
	
	/*
	 * Sets the distance between the viewport and the camera.
	 *
	 * @param distance - the distance (in world units) from the camera that the viewport is.
	 */
	void SetViewportDistance(float distance);
	
	/*
	 * Converts a point from 3D space to canvas space, based on this graphic
	 * manager's viewport. Assumes the camera is at (0, 0) facing towards the positive Z direction.
	 *
	 * @param vertex - the 3D point to render
	 * @return the 3D point's location on the 2D canvas, as seen through the viewport
	 */
	Point2D ProjectVertex(Point3D vertex);
	Point2D ProjectVertex(HomCoordinates vertex);


	// void RenderScene(Scene scene);

// Camera and scene
public:
	/*
	 * Returns a pointer to the camera being used by this GraphicsManager.
	 */
	Camera* GetMainCamera();

	/*
	 * Builds an empty scene with the current main Camera.
	 * 
	 * @return - a pointer to the new Scene, that you can add models to and render
	 */
	Scene* CreateScene();

	/*
	 * Returns a pointer to the Scene that is currently in use.
	 */
	Scene* GetCurrentScene();


};




#endif // _GRPAHICS_H //

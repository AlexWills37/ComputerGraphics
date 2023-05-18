# ComputerGraphics
## Read the documentation [here](documentation/GraphicsManager.md) (documentation still under construction)

# Project Overview
This repository contains my implementations for [Gabriel Gambetta's computer graphics textbook](https://gabrielgambetta.com/computer-graphics-from-scratch/), which is about building up a 
library for 3D graphics on a conceptual level.

The implementation uses SDL2 in C++ for opening a window and placing pixels
of a certain color on a certain coordinate in the window.
Other than using SDL2 to handle user input, window management, and pixel placement,
no external libraries (other than standard C++ libraries) are used.

This implementation takes an object-oriented approach and draws some inspiration
from game engines like Unity.

Currently, a rasterization system exists to render a scene with 3D models
from any camera point. 

Future work includes improving the rasterizer:
- shading
- textures

and implementing raytracing algorithms.

## Video of the rasterizer rendering a spinning cube
In this video, I am also moving the camera around the cube.

https://github.com/AlexWills37/ComputerGraphics/assets/77563588/75b4eb35-36c1-40c5-92be-ff9311000056



# Running the code
### Prerequisites
To compile the code, you must have a C++ compiler, the makefile tools, and SDL2, the graphics library used to place pixels on the screen.

Currently, this project is designed for Linux. It was built using Windows Subsystem for Linux and Ubuntu.

### Compiling
run `make` in this directory to build the project.

run `make clean` in this directory to remove the existing build.

run `./main.out` to run the program!
> Note: the `.out` prefix is designed for Linux systems

### Controls
This program operates on a small game-loop, where it handles some limited user input and redraws the scene every frame, with a maximum framerate of 120 fps.

The average framerate will be printed to the console every time it collects a sample of frames equal to the frame rate (currently every 120 frames).

`W A S D` - moves the camera up, left, down, and right, respectively.

`Escape` - quits the program.

`Left/Right arrows` - rotates the camera left and right.

`Up/Down arrows` - rotates the camera up and down (does not work when combined with left/right rotation).


# Chapter Breakdown
## Introduction
```
graphics.h
graphics_backend.cpp
```
PutPixel function and behind-the-scenes window management.
## Lines
```
graphics.h
graphics_math.h
graphics_utility.h
graphics_line.cpp
graphics_math.cpp
graphics_utility.cpp
```
Lines, points, and colors. Extending the backend to support drawing lines.

## Filled Triangles
```
graphics_triangle.cpp
```
Triangles!

## Shaded Triangles
```
graphics_triangle.cpp
```
More triangles!!

## Perspective Projection
```
graphics_projection.cpp
```
Using viewport math to project 3D points onto the screen, assuming the camera is
at (0, 0, 0) facing the positive Z direction. At this point, some refactoring
occurred to improve how the backend works (using Point structs instead of 
separate coordinates).

## Describing and Rendering a Scene
So much refactoring!!
```
graphics_scene.h
graphics_math.h
graphics_camera.cpp
graphics_scene.cpp
graphics_transform.cpp
```
Here, we introduced Models and ModelInstances, an encapsulated Camera class,
and a Scene class for rendering multiple ModelInstances. We also started to
use some fancy math with matrices, making a Transform class and structs to 
hold matrices and perform matrix math. These structs were later refactored
into classes for improved functionality and greater control.

## Clipping
Haha! Nothing can stop me now!
```
graphics_scene.cpp
graphics_models.cpp
```
This chapter was slightly incompatible with the existing code, so I had to do much refactoring. This included moving projection code into the Camera class, in
addition to reworking the render pipeline with a new RenderableModelInstance
class. This was needed, since clipping adds and removes triangles for rendering,
but we do not want to lose any information about the actual model in world
space (especially if we are rendering multiple frames, as is the case in
a game engine).

## Hidden Surface Removal

```
graphics_hsr.h
graphics_hidden_surface.cpp
```
The graphics manager now has a DepthBuffer object for keeping track of what depth the pixels are at,
so that objects closer to the camera are drawn on top of objects farther away.

`graphics_scene.cpp`, when rendering the scene, now calls `RenderDepthTriangle()` from `graphics_hidden_surface.cpp` to make use of this depth buffer.

Additionally, the backfacing faces of objects are culled here:
```
RenderScene()           [in graphics_scene.cpp]
-> RenderInstance()     [in graphics_scene.cpp]
   -> CullBackFaces()   [in graphics_hidden_surface.cpp]
```


## Shading

## Textures

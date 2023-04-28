# ComputerGraphics
## Read the documentation [here](documentation/GraphicsManager.md) 


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



## Shading

## Textures
/* graphics_scene.cpp
 *
 * Definitions for the Scene functions outlined in grpahics_scene.h
 * 
 * @author Alex Wills
 * @date March 28, 2023
 */

#include "../lib/graphics.h"
#include <iostream>


void Scene::AddModelInstance(ModelInstance & to_add)
{
    this->modelInstances.push_back(&to_add);
}

void Scene::RenderScene()
{

	// Render all of the models
	for (auto current_model : this->modelInstances)
	{
		this->RenderInstance(*current_model);
	}


}

void Scene::RenderInstance(ModelInstance to_render)
{
    // Get base model to transform
    Model * model = to_render.model;

	HomCoordinates transformed_points[ model->numVertices ];	// List of points before projection to the screen
    // Get list of 2D points to render eventually
	Point2D projected_points[ model->numVertices ];

    // We will be working with homoogenous coordinates for transforming our points
	HomCoordinates newPoint;

	TransformMatrix world_space_transform = TransformMatrix(to_render.transform);	// This converts from model space to world space
	// TODO: MOVE THIS SHIT BACK ONE FUNCTION
	TransformMatrix camera_transform = this->main_camera->GetWorldToCameraMatrix();	// This converts from world space to camera space




	// Convert all of the points to Camera space
	for (int i = 0; i < model->numVertices; ++i)
	{
		// Get the model-space point as homogenous coordinates
		newPoint.data[0] = model->vertices[i].x;
		newPoint.data[1] = model->vertices[i].y;
		newPoint.data[2] = model->vertices[i].z;
		newPoint.data[3] = 1;
		
		// Apply matrix to convert to world space
		newPoint = world_space_transform * newPoint;
		// Apply camera transform
		newPoint = camera_transform * newPoint;
	
		transformed_points[i] = newPoint;	// Passed by value
	}

	// Project all points
	for (int i = 0; i < model->numVertices; ++i)
	{
		newPoint = transformed_points[i];
		// Apply camera space -> screen space projection
		projected_points[i] = this->graphics_manager->ProjectVertex(newPoint);
	}


	// Render all triangles
	for (int i = 0; i < model->numTriangles; ++i)
	{
		this->RenderTriangle(model->triangles[i], projected_points);
	} 
}

void Scene::RenderTriangle(Triangle triangle, Point2D projected_vertices[])
{
    this->graphics_manager->DrawWireTriangle(    
		projected_vertices[triangle.p0], projected_vertices[triangle.p1], projected_vertices[triangle.p2],
		triangle.color);
}


Scene* GraphicsManager::CreateScene()
{
	this->current_scene = Scene(&(this->main_camera), this);
	return &(this->current_scene);
}
Scene* GraphicsManager::GetCurrentScene()
{
	return &(this->current_scene);
}
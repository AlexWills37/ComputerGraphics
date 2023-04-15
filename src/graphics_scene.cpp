/* graphics_scene.cpp
 *
 * Definitions for the Scene functions outlined in grpahics_scene.h
 * 
 * @author Alex Wills
 * @date March 28, 2023
 */

#include "../lib/graphics.h"
#include <unistd.h>	// For calling sleep() during debugging
#include <iostream>	// For print statements for debugging

/*
 * Destructor. Attempts to delete the ModelInstances at every pointer.
 */
Scene::~Scene()
{
	for (ModelInstance * pointer : this->model_instances)
	{
		if (pointer != nullptr)
			delete pointer;
	}
}

/*
 * Copy constructor. Explicitly copies every model instance as a pointer to a COPY
 * of the ModelInstance (as opposed to a copy of the pointer, pointing to the same instances).
 */
Scene::Scene(const Scene& to_copy)
{
	// Explicitly copy the isntance array with new copies of every model instance
	this->model_instances.resize(to_copy.model_instances.size());
	for (int i = 0; i < to_copy.model_instances.size(); ++i)
	{
		this->model_instances[i] = new ModelInstance(*(to_copy.model_instances[i]));
	}

	// Copy other values
	this->main_camera = to_copy.main_camera;
	this->graphics_manager = to_copy.graphics_manager;
}


void Scene::AddModelInstance(ModelInstance & to_add)
{
    this->model_instances.push_back(&to_add);
}

void Scene::RenderScene()
{
	// Get planes for clipping
	std::array<Plane * , 5> planes;
	for (int i = 0; i < 5; ++i)
	{
		planes[i] = this->main_camera->GetClippingPlane(i);
	}

	// Get camera transform (this will not change during the render)
	TransformMatrix world_to_cameraspace = this->main_camera->GetWorldToCameraMatrix();

	// Render one model instance at a time
	RenderableModelInstance * clipped_instance;
	for (ModelInstance * instance : this->model_instances)
	{
		// Create a copy of the instance for clipping
		clipped_instance = new RenderableModelInstance(instance);
		// Put instance in camera space. We will generate the bounding sphere before checking with each plane,
		// 		since clipping against a plane may change the points in the model, changing the bounding sphere.
		clipped_instance->GenerateWorldspacePoints();
		clipped_instance->ApplyTransform(world_to_cameraspace);
		
		// Clip the instance
		Scene::ClipInstance(*clipped_instance, planes);

		// Render the instance if it is not clipped
		if (!clipped_instance->GetIsRejected())
		{
			RenderInstance(clipped_instance);
		}
	}
}

void Scene::ClipInstance(RenderableModelInstance & instance, std::array<Plane*, 5> planes)
{
	// Clip against all of the planes, stopping if the instance is rejected by any plane
	for (int i = 0; i < planes.size() && !instance.GetIsRejected(); ++i)
	{
		Scene::ClipInstanceAgainstPlane(instance, planes[i]);
		if (instance.GetIsRejected()){
			std::cout << "\tBounding sphere rejected by plane: " << i << std::endl;
		}
	}

}

void Scene::ClipInstanceAgainstPlane(RenderableModelInstance & instance, Plane* plane)
{
	// Check the bounding sphere to see if all, somme, or none of the points are in bounds
	Triangle last = instance.triangles[instance.triangles.size() - 1];
	instance.GenerateBoundingSphere();
	float distance = plane->SignedDistance(instance.GetBoundingSphereCenter());	// Sphere center's distance from the plane
	float sphere_radius = instance.GetBoundingSphereRadius();
	if (distance > sphere_radius)	// Distance > radius, so every point is definitely on the correct side of the plane
	{
		std::cout << "\t\tInstance is completely in bounds of this plane!" << std::endl;
		// This object is in the bounds entirely!! The instance will keep its current triangles.
		// instance.triangles = instance.model->triangles;
	} else if (distance < -sphere_radius)	// Distance < -radius, so every point is definitely on the wrong side of the plane
	{
		std::cout << "\t\tInstance is completely out of bounds of this plane." << std::endl;
		// This object is entirely out of bounds...
		instance.Reject();
	} else {	// Distance is in between the radius, so the sphere intersects the plane, and some points may be on different sides
		std::cout << "\t\tInstance is partially in bounds with this plane..." << std::endl;
		// This object is partially in bounds. WE MUST CLIP THE TRIANGLES NOW!!!
		instance.ClipTrianglesAgainstPlane(plane);
	}
}

void Scene::RenderInstance(RenderableModelInstance * to_render)
{

	// The points in the model instance should at this point be in camera space,
	// having the local transform and the camera transform applied, along with any clipping.

	// Project all points from the model instance
	std::vector<HomCoordinates>* points = to_render->GetPoints();
	Point2D projected_points[ points->size() ];	// 1 to 1 mapping between 3D points in camera space and 2D points on screen

	for (int i = 0; i < points->size(); ++i)
	{
		// Apply camera space -> screen space projection
		projected_points[i] = this->main_camera->ProjectVertex((*points)[i]);
	}

	// Render all triangles
	std::vector<Triangle>* triangles = to_render->GetTriangles();
	for (int i = 0; i < triangles->size(); ++i)
	{
		this->RenderTriangle((*triangles)[i], projected_points);
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
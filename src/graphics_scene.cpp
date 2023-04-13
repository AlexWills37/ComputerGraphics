/* graphics_scene.cpp
 *
 * Definitions for the Scene functions outlined in grpahics_scene.h
 * 
 * @author Alex Wills
 * @date March 28, 2023
 */

#include "../lib/graphics.h"
#include <unistd.h>
#include <iostream>

Scene::~Scene()
{
	if (!is_original_scene){
		for (ModelInstance * pointer : this->modelInstances)
		{
			if (pointer != nullptr)
				delete pointer;
		}
	}
}



void Scene::AddModelInstance(ModelInstance & to_add)
{
    this->modelInstances.push_back(&to_add);
}

void Scene::RenderScene()
{
	// Get planes for clipping
	// std::vector<Plane> planes;
	// Plane one = Plane(0, 0, 1, - this->main_camera->GetViewportDistance());
	// planes.push_back(one);


	// Plane left = Plane((1 / std::sqrt(2)), 0, (1 / std::sqrt(2)), 0);
	// Plane right = Plane((-1 / std::sqrt(2)), 0, (1 / std::sqrt(2)), 0);
	// Plane top = Plane( 0, (-1 / std::sqrt(2)), (1 / std::sqrt(2)), 0);
	// Plane bottom = Plane( 0, (1 / std::sqrt(2)), (1 / std::sqrt(2)), 0);

	// planes.push_back(left);
	// planes.push_back(right);
	// planes.push_back(top);
	// planes.push_back(bottom);

	std::array<Plane * , 5> planes;
	// std::cout << "Rendering Scene with " << modelInstances.size() << " model instances" << std::endl;
	// std::cout << "Clipping planes: " << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		planes[i] = this->main_camera->GetClippingPlane(i);
		// std::cout << "\t";
		// planes[i]->Print();
	}



	// Get camera transform (this will not change during the render)
	TransformMatrix world_to_cameraspace = this->main_camera->GetWorldToCameraMatrix();
	RenderableModelInstance * clipped_instance;
	for (ModelInstance * instance : this->modelInstances)
	{
		// std::cout << "Rendering instance... Generating points..." << std::endl;
		// Create a copy of the instance for clipping
		clipped_instance = new RenderableModelInstance(instance);
		// Put instance in camera space
		clipped_instance->GenerateWorldspacePoints();
		clipped_instance->ApplyTransform(world_to_cameraspace);
		std::vector<HomCoordinates> ps = *(clipped_instance->GetPoints());
		// std::cout << ps.size() << " points generated for the base model" << std::endl;
		// std::cout << "Points in camera space: " << std::endl;
		// for (HomCoordinates c : ps)
		// {
		// 	std::cout << "\t(" << c.data[0] << ", " << c.data[1] << ", " <<c.data[2] << ")" << std::endl;
		// }

		// Clip the instance
		// std::cout << "Clipping instance..." << std::endl;
		Scene::ClipInstance(*clipped_instance, planes);
		// std::cout << "Instance was " << (clipped_instance->GetIsRejected() ? "" : "not") << " rejected" << std::endl;
		// std::cout << "Instance now has " << clipped_instance->GetPoints()->size() << " points" << std::endl;
		// Render the instance if it is not clipped
		if (!clipped_instance->GetIsRejected())
		{
			RenderInstance(clipped_instance);
		}

		// sleep(5);
	}



}

void Scene::ClipInstance(RenderableModelInstance & instance, std::array<Plane*, 5> planes)
{
	// Clip against all of the planes, stopping if the instance should be rejected
	instance.GenerateBoundingSphere();
	// std::cout << "Clipping against " << planes.size() << " planes" << std::endl;
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
	float distance = plane->SignedDistance(instance.GetBoundingSphereCenter());
	float sphere_radius = instance.GetBoundingSphereRadius();
	if (distance > sphere_radius)
	{
		// This object is in the bounds entirely!! The instance will keep its normal triangles <3
		// std::cout << "\t\tInstance is completely in bounds of this plane!" << std::endl;
		instance.triangles = instance.model->triangles;
	} else if (distance < -sphere_radius)
	{
		// std::cout << "\t\tInstance is completely out of bounds of this plane." << std::endl;
		// This object is entirely out of bounds...
		instance.Reject();
	} else {
		// This object is partially in bounds. WE MUST CLIP THE TRIANGLES NOW!!!
		// std::cout << "\t\tInstance is partially in bounds with this plane..." << std::endl;
		instance.ClipTrianglesAgainstPlane(plane);
	}

}



void Scene::RenderInstance(RenderableModelInstance * to_render)
{

	// std::cout << "Rendering instance..." << std::endl;
	// The points in the model instance should at this point be in camera space,
	// having the local transform and the camera transform applied


	// Project all points from the model instance
	HomCoordinates newPoint;
	std::vector<HomCoordinates> points = *(to_render->GetPoints());
	// std::cout << "\t" << points.size() << " points to project..." << std::endl;
	Point2D projected_points[ points.size() ];
	for (int i = 0; i < points.size(); ++i)
	{
		// Apply camera space -> screen space projection
		projected_points[i] = this->main_camera->ProjectVertex(points[i]);
		// std::cout << "\t\tProjection: (" << projected_points[i].x << ", " << projected_points[i].y << ")" << std::endl;
	}


	// Render all triangles
	std::vector<Triangle> triangles = to_render->triangles;
	// std::cout << "\tRendering " << triangles.size() << " triangles..." << std::endl;
	for (int i = 0; i < triangles.size(); ++i)
	{
		this->RenderTriangle(triangles[i], projected_points);
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
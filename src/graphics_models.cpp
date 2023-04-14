
#include "../lib/graphics.h"


/*
 * Based on this instance's model and transform, set the list of worldspace points
 */
void RenderableModelInstance::GenerateWorldspacePoints()
{
    int num_points = this->model->vertices.size();

    this->points.clear();
    this->points.resize(num_points);

    HomCoordinates newPoint;
    TransformMatrix world_space_transform = this->transform;

    for (int i = 0; i < num_points; ++i)
    {
        // Get the model-space point as homogenous coordinates
		newPoint[0] = model->vertices[i].x;
		newPoint[1] = model->vertices[i].y;
		newPoint[2] = model->vertices[i].z;
		newPoint[3] = 1;
		
		// Apply matrix to convert to world space
		newPoint = world_space_transform * newPoint;

        this->points[i] = newPoint;  // Passed by value
    }

}

void RenderableModelInstance::ApplyTransform(TransformMatrix transform)
{
    for (int i = 0; i < points.size(); ++i)
    {
        this->points[i] = transform * this->points[i];
    }
}

/*
 * Generates a bounding sphere that includes all points in this model instance.
 */
void RenderableModelInstance::GenerateBoundingSphere()
{
    // First, find a rough center of the sphere by averaging all of the points
    HomCoordinates center;
    if (points.size() > 0)
    {
        for (HomCoordinates point : this->points)
        {
            center[0] += point[0];
            center[1] += point[1];
            center[2] += point[2];
        }
        center[0] /= this->points.size();
        center[1] /= this->points.size();
        center[2] /= this->points.size();
    }

    // Now find the largest distance between the center and a point (the radius)
    float radius = 0;
    float distance;
    for (HomCoordinates point : this->points)
    {
        distance = std::sqrt(
            std::pow(point[0] - center[0], 2) +
            std::pow(point[1] - center[1], 2) +
            std::pow(point[2] - center[2], 2)
        );

        if (distance > radius)
            radius = distance;        
    }

    this->bounding_sphere_center = center;
    this->bounding_sphere_radius = radius;

}


void RenderableModelInstance::ClipTrianglesAgainstPlane(Plane* plane)
{
    // I am using buffer lists to add triangles to our lists for 2 reasons:
        // 1: calling push_back multiple times (adding 2 triangles) seems to mess up the iterator (at least when tested with ints)
        // 2: we do not need to clip the triangles that are added to the list
    new_tris.clear();   // List of new triangles to add to this render instance
    new_points.clear(); // List of new points to add to this render instance, for the new triangles
    new_point_start_index = this->points.size(); // Index to start building new triangles

    bool clipped = false;

    // Iterate through the triangle list
    for (std::vector<Triangle>::iterator iterator = this->triangles.begin(); iterator != this->triangles.end(); iterator++)
    {
        Triangle to_clip = *iterator;

        clipped = this->ClipTriangle(to_clip, plane);
        if (clipped)
        {
            this->triangles.erase(iterator);
            iterator--;
            std::cout << "Triangle clipped!" << std::endl;
        }
    }

    // Add new triangles and points
    
    for (HomCoordinates point : new_points)
    {
        // std::cout << "\t" << point.data[0] << ", " << point.data[1] << ", " << point.data[2] << std::endl;
        this->points.push_back(point);
        
    }

    int num_tris = new_tris.size();
    if (num_tris > 0)
    {
        std::cout << "\tadding " << num_tris << " triangles to the model" << std::endl;
    }
    for (Triangle triangle : new_tris)
    {
        this->triangles.push_back(triangle);
    }

}

/*
 * Clips a triangle based on a plane. If the triangle gets clipped, the new triangles (if any) will
 * be added to the new_tris and new_points buffer lists, and the function will return true
 */
bool RenderableModelInstance::ClipTriangle(Triangle to_clip, Plane * plane)
{

    
    bool clipped = true;
	HomCoordinates p0, p1, p2;
	p0 = this->points[to_clip.p0];
	p1 = this->points[to_clip.p1];
	p2 = this->points[to_clip.p2];



	float dist[3];
	dist[0] = plane->SignedDistance(p0);
	dist[1] = plane->SignedDistance(p1);
	dist[2] = plane->SignedDistance(p2);

	// Sort the distances
	float temp;
    HomCoordinates temp_coord;
    int temp_idx;
    if (dist[0] > dist[1])
	{
		temp = dist[0];
		dist[0] = dist[1];
		dist[1] = temp;
        temp_coord = p0;
        p0 = p1;
        p1 = temp_coord;
        temp_idx = to_clip.p0;
        to_clip.p0 = to_clip.p1;
        to_clip.p1 = temp_idx;
	}
	if (dist[0] > dist[2])
	{
		temp = dist[0];
		dist[0] = dist[2];
		dist[2] = temp;
        temp_coord = p0;
        p0 = p2;
        p2 = temp_coord;
        temp_idx = to_clip.p0;
        to_clip.p0 = to_clip.p2;
        to_clip.p2 = temp_idx;
	}
	if (dist[1] > dist[2])
	{
		temp = dist[1];
		dist[1] = dist[2];
		dist[2] = temp;
        temp_coord = p1;
        p1 = p2;
        p2 = temp_coord;
        temp_idx = to_clip.p1;
        to_clip.p1 = to_clip.p2;
        to_clip.p2 = temp_idx;
	}


	if (dist[0] > 0)	// Smallest distance is positive
	{
		// Triangle is entirely in bounds!! yay <3
		// The triangle will be part of the list!
        clipped = false;

	} else if (dist[2] < 0)	// Largest distance is negative
	{
		// Triangle is entirely out of bounds... not yay </3
        // Triangle will be erased in ClipTrianglesAgainstPlane function
	} else if (dist[1] < 0)	// Only 1 value is positive (the largest)
	{

        /*let A be the vertex with a positive distance
        compute B' = Intersection(AB, plane)
        compute C' = Intersection(AC, plane)
        return [Triangle(A, B', C')]*/

        HomCoordinates * pA = &p2;
        HomCoordinates pB_prime = plane->Intersection(p2, p1);
        HomCoordinates pC_prime = plane->Intersection(p2, p0);
        this->new_points.push_back(pB_prime);
        this->new_points.push_back(pC_prime);

        // Create triangle with same color, with 2 new points (B and C), and p2 from the original triangle (A)
        Triangle new_tri = {this->new_point_start_index, this->new_point_start_index + 1, to_clip.p2, to_clip.color};
        this->new_point_start_index += 2;
        this->new_tris.push_back(new_tri);

	} else { // Only 1 value is negative (the smallest)
        /*
        let C be the vertex with a negative distance
        compute A' = Intersection(AC, plane)
        compute B' = Intersection(BC, plane)
        return [Triangle(A, B, A'), Triangle(A', B, B')]
        */
       HomCoordinates * pC = &p0;
       HomCoordinates * pB = &p1;
       HomCoordinates * pA = &p2;
       HomCoordinates pA_prime = plane->Intersection(*pA, *pC);
       HomCoordinates pB_prime = plane->Intersection(*pB, *pC);
       this->new_points.push_back(pA_prime);
       this->new_points.push_back(pB_prime);

       Triangle new_tri0 = {to_clip.p2, to_clip.p1, this->new_point_start_index, to_clip.color};
       Triangle new_tri1 = {this->new_point_start_index, to_clip.p1, this->new_point_start_index + 1, to_clip.color};
       this->new_tris.push_back(new_tri0);
       this->new_tris.push_back(new_tri1);
       this->new_point_start_index += 2;


	}
    
    return clipped;

}
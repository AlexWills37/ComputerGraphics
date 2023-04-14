/* grpahics_projection.cpp
 *
 * Implementes functions needed for rasterization 
 * of a 3D scene to a 2D canvas.
 * 
 * @author Alex Wills
 * @date March 4, 2023
 */

#include "../lib/graphics.h"


/*
 * Convert a point on the viewport to a point on the canvas.
 * This is a helper function for doing conversions.
 */
Point2D ViewportToCanvas(float x, float y, float viewport_width, float viewport_height,
                            int canvas_width, int canvas_height)
{
    int canvas_x = int(x * canvas_width / viewport_width);
    int canvas_y = int(y * canvas_height / viewport_height);

    return Point2D{canvas_x, canvas_y};
}

/*
 * Converts a point from 3D space to canvas space, based on this graphic
 * manager's viewport.
 *
 * @param vertex - the 3D point to render
 * @return the 3D point's location on the 2D canvas, as seen through the viewport
 */
Point2D Camera::ProjectVertex(Point3D vertex)
{
    float viewport_x = vertex.x * this->viewport_distance / vertex.z;
    float viewport_y = vertex.y * this->viewport_distance / vertex.z;

    // std::cout << "Viewport: " << viewport_width << " x " << viewport_height << ": " << viewport_distance << std::endl;
    // std::cout << "Canvas: " << canvas_width << " x " << canvas_height << std::endl;
    
    return ViewportToCanvas(viewport_x, viewport_y, this->viewport_width, this->viewport_height,
                            this->canvas_width, this->canvas_height);
}


/*
 * Converts a point from 3D space to canvas space, based on this graphic
 * manager's viewport.
 */
Point2D Camera::ProjectVertex(HomCoordinates vertex)
{
    Point3D p_vertex = {vertex[0], vertex[1], vertex[2]};
    return this->ProjectVertex(p_vertex);
}


#ifndef _GRAPHICS_SCENE_H
#define _GRAPHICS_SCENE_H
#include "graphics.h"

class Camera
{
    private: // Private member variables
        Transform camera_transform;

    public: // Constructor
        Camera()
        {
            this->camera_transform = Transform();   // The scale will always be 1
        }

        Camera(float x, float y, float z, float rx, float ry, float rz) : Camera()
        {
            this->camera_transform.translation[0] = x;
            this->camera_transform.translation[1] = y;
            this->camera_transform.translation[2] = z;
            this->camera_transform.rotation[0] = rx;
            this->camera_transform.rotation[1] = ry;
            this->camera_transform.rotation[2] = rz;
        }

    public: // Public methods
        void SetPosition(float x, float y, float z);
        void MovePosition(float deltaX, float deltaY, float deltaZ);
        void SetRotation(float x, float y, float z);

        TransformationMatrix GetWorldToCameraMatrix();
};




struct Triangle {
	int p0, p1, p2;
	Color color;
};

struct Model {
    struct Point3D * vertices;
    int numVertices;
    struct Triangle * triangles;
    int numTriangles;
};

struct ModelInstance {
    struct Model * model;
    Transform transform;
};


#endif
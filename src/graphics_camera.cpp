/* graphics_camera.cpp
 * 
 * Defines the functions involving the Camera object
 * 
 * @author Alex Wills
 * @date March 28, 2023
 */

#include "../lib/graphics.h"


void Camera::SetPosition(float x, float y, float z)
{
    this->camera_transform.translation[0] = -x;
    this->camera_transform.translation[1] = -y;
    this->camera_transform.translation[2] = -z;
}

void Camera::MovePosition(float deltaX, float deltaY, float deltaZ)
{
    this->camera_transform.translation[0] -= deltaX;
    this->camera_transform.translation[1] -= deltaY;
    this->camera_transform.translation[2] -= deltaZ;
}


void Camera::SetRotation(float x, float y, float z)
{
    this->camera_transform.rotation[0] = -x;
    this->camera_transform.rotation[1] = -y;
    this->camera_transform.rotation[2] = -z;
}

void Camera::Rotate(float deltaX, float deltaY, float deltaZ)
{
    this->camera_transform.rotation[0] -= deltaX;
    this->camera_transform.rotation[1] -= deltaY;
    this->camera_transform.rotation[2] -= deltaZ;
}

/*
 * Get the transformation matrix to convert a point in World Space
 * to its place in Camera Space (where the point would be if the world were
 * moved so that the camera is at (0, 0) facing the positive z direction)
 * NOTE: CURRENTLY BROKEN: THE INVERTED MATRIX IS NOT THE CORRECT OPTION.
 * WITH THE CAMERA, YOU SHOULD APPLY THE INVERTED TRANSLATION, THEN THE INVERTED ROTATION.
 */
TransformMatrix Camera::GetWorldToCameraMatrix()
{
    return TransformMatrix(this->camera_transform);
}

Camera* GraphicsManager::GetMainCamera()
{
    return &(this->main_camera);
}
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
    this->camera_transform.translation[0] = x;
    this->camera_transform.translation[1] = y;
    this->camera_transform.translation[2] = z;
}

void Camera::MovePosition(float deltaX, float deltaY, float deltaZ)
{
    this->camera_transform.translation[0] += deltaX;
    this->camera_transform.translation[1] += deltaY;
    this->camera_transform.translation[2] += deltaZ;
}


void Camera::SetRotation(float x, float y, float z)
{
    this->camera_transform.rotation[0] = x;
    this->camera_transform.rotation[1] = y;
    this->camera_transform.rotation[2] = z;
}

void Camera::Rotate(float deltaX, float deltaY, float deltaZ)
{
    this->camera_transform.rotation[0] += deltaX;
    this->camera_transform.rotation[1] += deltaY;
    this->camera_transform.rotation[2] += deltaZ;
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
    TransformMatrix inverse_rotation = BuildRotationMatrix(-this->camera_transform.rotation[0], -this->camera_transform.rotation[1], -this->camera_transform.rotation[2]);
    
    // TransformMatrix inverse_rotation = BuildRotationMatrix(-this->pitch, 0, 0) * BuildRotationMatrix(0, -this->yaw, 0);

    TransformMatrix inverse_translation;
    float value;    // Value for building matrix
    // Build inverse translation
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            value = 0;
            if (row == col)
                value = 1;
            else if (col == 3)
                value = -this->camera_transform.translation[row];
            
            inverse_translation.data[row][col] = value;
        }
    }


    // return TransformMatrix(this->camera_transform);
    return inverse_rotation * inverse_translation;
}

Camera* GraphicsManager::GetMainCamera()
{
    return &(this->main_camera);
}

void Camera::RotateVertically(float rotation)
{
    // Assume that the X axis is parallel to the XZ plane (in the roll/pitch/yaw model, the roll is 0)
    // Rotate the X axis so that it is in line with the global X axis
    // (reverse the Y rotation to 0)
    TransformMatrix invertY = BuildRotationMatrix(0, -this->camera_transform.rotation[1], 0);
    TransformMatrix revertY = BuildRotationMatrix(0, this->camera_transform.rotation[1], 0);



    // Use a mini-rotation matrix to rotate exclusively on the X axis
    // (apply the x rotation)
    TransformMatrix applyX = BuildRotationMatrix(rotation, 0, 0);


    // Undo the rotation that lined the global X axis up
}

void Camera::GenerateClippingPlanes()
{
    // Create clipping planes
    // std::cout << "Generating clipping planes for the camera." << std::endl;


    float delta_height = (this->viewport_height) / 2.0;
    float delta_width = (this->viewport_width) / 2.0;

    this->clipping_planes[0] = Plane(0, 0, 1, -this->viewport_distance);    // Front plane
    this->clipping_planes[1] = Plane(0, -this->viewport_distance, delta_height, 0);   // Top
    this->clipping_planes[3] = Plane(0, this->viewport_distance, delta_height, 0);   // Bottom
    this->clipping_planes[2] = Plane(this->viewport_distance, 0, delta_width, 0);  // Left
    this->clipping_planes[4] = Plane(-this->viewport_distance, 0, delta_width, 0);  // Right
}
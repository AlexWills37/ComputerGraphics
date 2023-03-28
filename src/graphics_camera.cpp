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

TransformationMatrix Camera::GetWorldToCameraMatrix()
{
    return TransformationMatrix(this->camera_transform);
}

Camera* GraphicsManager::GetMainCamera()
{
    return &(this->main_camera);
}
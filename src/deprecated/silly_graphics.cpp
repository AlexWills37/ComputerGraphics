
#include "../lib/graphics.h"
#include <iostream>
#include <cmath>

void GraphicsManager::RotateCube(ModelInstance* cube)
{
    Transform * cube_transform = &(cube->transform);

    cube_transform->rotation[0] += 3.14/400;
    cube_transform->rotation[1] += 3.14/400;

    float offset = std::sin(cube_transform->rotation[0]);
    cube_transform->translation[1] = this->starting_y + offset;



}
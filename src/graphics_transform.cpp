/* graphics_transform.cpp
 * 
 * Defines the functions used by Transform objects and the TransformMatrix.
 * 
 * @author Alex Wills
 * @date March 28, 2023
 */
#include "../lib/graphics.h"


//TODO: Store rotation MATRIX, not just euler angles
Transform::operator TransformMatrix() const
{

	// First initialize all matrices
	TransformMatrix scale, rotation, translation;
	
	// Get rotation matrix
	rotation = BuildRotationMatrix(this->rotation[0], this->rotation[1], this->rotation[2]);
	
	// Set all array values to 0
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			scale.data[row][col] = 0;
			translation.data[row][col] = 0;
		}
	}



	// Scale matrix: scale along diagonal
	for (int i = 0; i < 3; ++i)
	{
		scale.data[i][i] = this->scale[i];
	}
	// 1 in the bottom right corner
	scale.data[3][3] = 1;

	// Translation matrix: identity matrix with translation in the 4th column
	for (int i = 0; i < 3; ++i)
	{
		translation.data[i][i] = 1;
		translation.data[i][3] = this->translation[i];
	}
	translation.data[3][3] = 1;

	// Return the final transformation, doing translation last (we are premultiplying)
	return translation * (rotation * scale);

}

void Transform::MoveLocally(float deltaX, float deltaY, float deltaZ)
{
	// Get movement as a homogenous vector
	HomCoordinates movement;
	movement.data[0] = deltaX;
	movement.data[1] = deltaY;
	movement.data[2] = deltaZ;
	movement.data[3] = 0;

	// Get rotation matrix from this transform's rotations
	TransformMatrix rotation = BuildRotationMatrix(this->rotation[0], this->rotation[1], this->rotation[2]);
	movement = rotation * movement;

	// Move transform globally now
	this->translation[0] += movement.data[0];
	this->translation[1] += movement.data[1];
	this->translation[2] += movement.data[2];
}

void Transform::RotateAboutAxis(HomCoordinates axis_vector, float rotation)
{
	// Start by normalizing the axis
}

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

	// First initialize matrices
	// Default constructor initializes values to 0
	TransformMatrix scale = TransformMatrix();
	TransformMatrix translation = TransformMatrix();
	
	// Get rotation matrix
	TransformMatrix rotation = TransformMatrix::BuildRotationMatrix(this->rotation[0], this->rotation[1], this->rotation[2]);
	

	// Scale matrix: scale along diagonal
	for (int i = 0; i < 3; ++i)
	{
		scale(i, i) = this->scale[i];
	}
	// 1 in the bottom right corner
	scale(3, 3) = 1;

	// Translation matrix: identity matrix with translation in the 4th column
	for (int i = 0; i < 3; ++i)
	{
		translation(i, i) = 1;
		translation(i, 3) = this->translation[i];
	}
	translation(3, 3) = 1;

	// Return the final transformation, doing translation last (we are premultiplying)
	return translation * (rotation * scale);

}

void Transform::MoveLocally(float deltaX, float deltaY, float deltaZ)
{
	// Get movement as a homogenous vector
	HomCoordinates movement;
	movement[0] = deltaX;
	movement[1] = deltaY;
	movement[2] = deltaZ;
	movement[3] = 0;

	// Get and apply rotation matrix from this transform's rotations
	TransformMatrix rotation = TransformMatrix::BuildRotationMatrix(this->rotation[0], this->rotation[1], this->rotation[2]);
	movement = rotation * movement;

	// Move transform globally now
	this->translation[0] += movement[0];
	this->translation[1] += movement[1];
	this->translation[2] += movement[2];
}

// void Transform::RotateAboutAxis(HomCoordinates axis_vector, float rotation)
// {
// 	// Start by normalizing the axis
// }

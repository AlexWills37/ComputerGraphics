/* graphics_transform.cpp
 * 
 * Defines the functions used by Transform objects and the TransformMatrix.
 * 
 * @author Alex Wills
 * @date March 28, 2023
 */
#include "../lib/graphics.h"

Transform::operator TransformMatrix() const
{

	// First initialize all matrices
	TransformMatrix xRot, yRot, zRot, scale, translation;
	
	// FIRST: Set all array values to 0
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			xRot.data[row][col] = 0;
			yRot.data[row][col] = 0;
			zRot.data[row][col] = 0;
			scale.data[row][col] = 0;
			translation.data[row][col] = 0;
		}
	}

	// X rotation:
	xRot.data[0][0] = 1;
	xRot.data[1][1] = std::cos(this->rotation[0]);
	xRot.data[1][2] = std::sin(this->rotation[0]);
	xRot.data[2][1] = - std::sin(this->rotation[0]);
	xRot.data[2][2] = std::cos(this->rotation[0]);
	xRot.data[3][3] = 1;

	// Y rotation:
	yRot.data[1][1] = 1;
	yRot.data[0][0] = std::cos(this->rotation[1]);
	yRot.data[0][2] = - std::sin(this->rotation[1]);
	yRot.data[2][0] = std::sin(this->rotation[1]);
	yRot.data[2][2] = std::cos(this->rotation[1]);
	yRot.data[3][3] = 1;

	// Z rotation:
	zRot.data[2][2] = 1;
	zRot.data[0][0] = std::cos(this->rotation[2]);
	zRot.data[1][0] = - std::sin(this->rotation[2]);
	zRot.data[0][1] = std::sin(this->rotation[2]);
	zRot.data[1][1] = std::cos(this->rotation[2]);
	zRot.data[3][3] = 1;

	// Combine all rotation
	TransformMatrix rotation = (zRot * yRot) * xRot;
	// Put a 1 in the bottom right corner
	rotation.data[3][3] = 1;

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
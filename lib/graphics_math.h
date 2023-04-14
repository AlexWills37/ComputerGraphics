/* graphics_math.h
 *
 * This file contains mathematical functions to be used by the graphics library.
 *
 * @author Alex Wills
 * @date February 15 2023
 */
#ifndef _GRAHPICS_MATH_H
#define _GRAPHICS_MATH_H

#include <array>
#include <iostream>

/*
 * Interpolate a line between two variables.
 * For every integer between i0 and i1 (independent variables),
 * this funciton returns the coresponding dependent variable
 * based on the line created by two points.
 *
 * @param i0 - independent variable at point 0
 * @param d0 - dependent variable at point 0
 * @param i1 - independent vairable at point 1
 * @param d1 - dependent variable at point 1
 * @param destination - a pointer to the beginning of an integer array to store 
 *   the results of this function
 */
void Interpolate(int i0, float d0, int i1, float d2, float* destination);

/*
 * Clamp a value between two other values.
 *
 * @param value - the value to clamp
 * @param low - the lower boundary for the value
 * @param high - the upper boundary for the value
 * 
 * @return either low, value, or high, depending on if the value
 * 	is above high or below low.
 */
float clamp(float value, float low, float high);

/*
 * 4x4 matrix to represent a transform.
 * 
 * To apply a transform to a set of coordinates, multiply the matrix
 * with a 4x1 matrix representing the coordinates. This is handled in
 * the HomCoordinates struct.
 * 
 * Always multiply with the TransformMatrix on the left side, and the HomCoordinates
 * on the right.
 */
class TransformMatrix {
    
    // Member variables
    private:
        float data[4][4];
    
    // Constructors
    public: 
        /*
         * Default Constructor. Initializes a matrix will 0 in every element.
         */
        TransformMatrix()
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    this->data[i][j] = 0;
                }
            }
        }

    // Operators
    public: 
        /*
        * Multiplies two 4x4 matrices (TransformMatrix objects).
        * NOT commutative. A * B != B * A
        */
        TransformMatrix operator*(const TransformMatrix&);

        float& operator()(int row, int column)
        {
            return this->data[row][column];
        }

        float operator()(int row, int column) const
        {
            return this->data[row][column];
        }


};

/*
 * 4x1 matrix (vector) to represent homogenous coordinates.
 * This is most often a 3D point ( the fourth element != 0 ),
 * but can also be a vector ( the fourth element == 0 ).
 */
class HomCoordinates {
    // Member variables
    private:
        float data[4];

    // Constructors
    public:
        /*
         * Default constructor. Initializes a 0 vector (0, 0, 0, 0)
         */
        HomCoordinates()
        {
            for (int i = 0; i < 4; ++i)
            {
                this->data[i] = 0;
            }
        }

    // Operators
    public:
        
        // Index operators to access the data in this object
        float& operator[](int index)
        {
            return this->data[index];
        }

        float operator[](int index) const
        {
            return this->data[index];
        }

        // Arithmetic operators
        HomCoordinates operator+(const HomCoordinates&, const HomCoordinates&);
        HomCoordinates operator-(const HomCoordinates&, const HomCoordinates&);
        HomCoordinates operator*(float, const HomCoordinates&);
        HomCoordinates operator*(const HomCoordinates&, float);

};

TransformMatrix BuildRotationMatrix(float x, float y, float z);




/*
 * Applies a transformation on a set of coordinates.
 * This is done by multiplying a 4x4 TransformMatrix with a 4x1 Homogenous Coordinates matrix.
 */
HomCoordinates operator*(const TransformMatrix&, const HomCoordinates&);

// HomCoordinates operator+(HomCoordinates, HomCoordinates);
// HomCoordinates operator-(HomCoordinates, HomCoordinates);
// HomCoordinates operator*(float, HomCoordinates);
// HomCoordinates operator*(HomCoordinates, float);

// float& HomCoordinates::operator[](int index);

void NormalizeVector(std::array<float, 3> & vec);

class Plane {
    friend class RenderableModelInstance;
    public:
        Plane(float x, float y, float z, float d);
        Plane(): Plane(0, 0, 0, 0)
        {}
        ~Plane()
        {}

        Plane(const Plane & to_copy);

        void Print()
        {
            std::cout << "Plane info" << std::endl;
            std::cout << "\tN = " << normal[0] << ", " << normal[1] << ", " << normal[2] << std::endl;
            std::cout << "\tConstant: " << constant << std::endl;
        }

    private:
        std::array<float, 3> normal;
        float constant;

    public:
        float SignedDistance(HomCoordinates point);

        HomCoordinates Intersection(HomCoordinates pA, HomCoordinates pB);
};


#endif

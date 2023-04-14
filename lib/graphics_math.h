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
 * Normalizes a vector's values so that the magnitude is equal to 1.
 * 
 * @param vec - reference to a vector defined as an std::array of 3 floats
 */
void NormalizeVector(std::array<float, 3> & vec);

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

        /*
         * Copy constructor. Explictly copies array data.
         */
        TransformMatrix(const TransformMatrix& to_copy)
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    this->data[i][j] = to_copy(i, j);
                }
            }
        }

        // Default destructor
        //~TransformMatrix()

    // Operators
    public: 
        /*
        * Multiplies two 4x4 matrices (TransformMatrix objects).
        * NOT commutative. A * B != B * A
        */
        TransformMatrix operator*(const TransformMatrix&) const;

        /*
         * Indexes into the matrix at (row, column).
         */
        float& operator()(int row, int column)
        {
            if (row < 0 || row >= 4 || column < 0 || column >= 4)
            {
                std::cout << "ERROR: Indexing out of bounds (" << row << ", " << column << "). Returning Matrix(0, 0)." << std::endl;
                return this->data[0][0];
            }
            return this->data[row][column];
        }

        /*
         * Indexes into the matrix at (row, column).
         */
        float operator()(int row, int column) const
        {
            if (row < 0 || row >= 4 || column < 0 || column >= 4)
            {
                std::cout << "ERROR: Indexing out of bounds (" << row << ", " << column << "). Returning 0." << std::endl;
                return 0;
            }
            return this->data[row][column];
        }

    // Static functions
    public:
        static TransformMatrix BuildRotationMatrix(float x, float y, float z);


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

        /*
         * Copy constructor. Expliclty copies array data.
         */
        HomCoordinates(const HomCoordinates& to_copy)
        {
            for (int i = 0; i < 4; ++i)
            {
                this->data[i] = to_copy[i];
            }
        }

        // Default destructor
        //~HomCoordinates()

    // Operators
    public:
        
        // Index operators to access the data in this object
        /*
         * Index into the coordinates.
         * 0 = x
         * 1 = y
         * 2 = z
         * 3 = w
         */
        float& operator[](int index)
        {
            return this->data[index];
        }
        /*
         * Index into the coordinates.
         * 0 = x
         * 1 = y
         * 2 = z
         * 3 = w
         */
        float operator[](int index) const
        {
            return this->data[index];
        }

        // Arithmetic operators
        HomCoordinates operator+(const HomCoordinates&) const;
        HomCoordinates operator-(const HomCoordinates&) const;

        // Scalar multiplication
        HomCoordinates operator*(float) const;

};
// Operators in the other direction
HomCoordinates operator*(float, const HomCoordinates&);
/*
 * Applies a transformation on a set of coordinates.
 * This is done by multiplying a 4x4 TransformMatrix with a 4x1 Homogenous Coordinates matrix.
 */
HomCoordinates operator*(const TransformMatrix&, const HomCoordinates&);




class Plane {
    friend class RenderableModelInstance;
    // Member variables
    private:
        std::array<float, 3> normal;
        float constant;

    // Constructors
    public:
        /*
         * Creates a plane based on a normal vector and a constant.
         */
        Plane(float x, float y, float z, float d);

        /*
         * Default constructor. Creates a plane from a 0 vector with no constant.
         */
        Plane(): Plane(0, 0, 0, 0)
        {}

        /*
         * Copy constructor. Explictly copies array values.
         */
        Plane(const Plane& to_copy)
        {
            for (int i = 0; i < 3; ++i)
            {
                this->normal[i] = to_copy.normal[i];
            }
	        this->constant = to_copy.constant;
        }

        // Default destructor
        // ~Plane()


        
    // Methods
    public:
        /*
         * Returns the signed distance between a point and this plane
         */
        float SignedDistance(const HomCoordinates& point);

        /*
         * Returns the point on the line formed by 2 other points that intersects this plane.
         *
         * @param pA, pB - points that form a line intersecting this plane
         */
        HomCoordinates Intersection(const HomCoordinates& pA, const HomCoordinates& pB);

        /*
         * Prints information about this plane to the console.
         */
        void Print()
        {
            std::cout << "Plane info" << std::endl;
            std::cout << "\tN = " << normal[0] << ", " << normal[1] << ", " << normal[2] << std::endl;
            std::cout << "\tConstant: " << constant << std::endl;
        }
};


#endif

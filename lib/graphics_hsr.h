/* graphics_hsr.h
 *
 * Functions for the Hidden Surface Removal (hsr) chapter.
 * Includes modifications for depth buffering and backface culling.
 */
#ifndef _GRAPHICS_HSR_H
#define _GRAPHICS_HSR_H
#include "./graphics.h"
#include <vector>

/*
 * 2D array to store information about the depth of the pixels currently on screen.
 */
class DepthBuffer {
    
    // Member variables
    private:
        static constexpr float cleared_value = 0;   // The value representing a space that has no pixels

        std::vector<float> buffer;
        int width, height;
        int max_x, max_y;

    // Constructors
    public:
        /*
         * Default constructor. Initializes a depth buffer with 1 empty element.
         */
        DepthBuffer()
        {
            this->width = 0;
            this->height = 0;
            this->max_x = 0;
            this->max_y = 0;
            this->buffer.resize(1);
            this->buffer[0] = DepthBuffer::cleared_value;
        }

        /*
         * Initializes a depth buffer with a width and height.
         * 
         * @param width - the width of the canvas
         * @param height - the height of the canvas
         */
        DepthBuffer(int width, int height)
        {
            this->width = width;
            this->height = height;
            this->max_x = width / 2;
            this->max_y = height / 2;

            // Initialize empty buffer (as a 1D vector)
            buffer.resize((width + 1) * (height + 1));
            for (int i = 0; i < buffer.size(); ++i)
            {
                buffer[i] = DepthBuffer::cleared_value;
            }

            std::cout << "~ Creating Depth Buffer with top left [0][0] at (" << -this->max_x << ", " << -this->max_y << ")" << 
                "\n\tand top right [" << this->width - 1 << "][" << this->height-1 << "] at (" << this->width - this->max_x << ", " << this->height - this->max_y << ")" << std::endl;
        }

    // Overloaded operators
    public:
        /*
        * Indexes into the depth buffer at row, column (read/write access).
        */
        float& operator()(int x, int y)
        {
            // Translate (x, y) to array location 
            int row = y + this->max_y;
            int column = x + this->max_x;

            // Ensure index is in-bounds
            


            if (row > this->width || row < 0 || column > this->height || column < 0)
            {
                std::cout << "ERROR: Indexing out of bounds (" << x << ", " << y << "). Returning DepthBuffer(0, 0)." << std::endl;
                return buffer[0];
            }

            return buffer[(row * this->width) + column];
        }

        /*
        * Indexes into the depth buffer at row, column (read only access).
        */
        float operator()(int row, int column) const
        {
            // Ensure index is in-bounds
            if (row >= this->width || row < 0 || column >= this->height || column < 0)
            {
                std::cout << "ERROR: Indexing out of bounds (" << row << ", " << column << "). Returning DepthBuffer(0, 0)." << std::endl;
                return buffer[0];
            }

            return buffer[(row * this->width) + column];
        }

    // Methods
    public:
        /*
         * Clears the buffer by setting all values to the const cleared_value.
         */
        void ClearBuffer()
        {
            for (int i = 0; i < this->buffer.size(); ++i)
            {
                this->buffer[i] = DepthBuffer::cleared_value;
            }
        }


};


#endif
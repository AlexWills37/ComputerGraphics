# graphics_math.h
Funcitons provided in the graphics_math library

## void Interpolate(int i0, int d0, int i1, int d1, int* destination)
    Populates an array specified by `destination` with dependent variable values. This function creates a line from (i0, d0) to (i1, d1) and commputes the values of d for each integer between i0 and i1 (inclusive).

    `destination` is a pointer to the beginning of an array that has enough memory to store every integer between i0 and i1.

    NOTE: This function does NOT check if enough memory is allocated for the destination array.
/* graphics_math.cpp
 * 
 * Definitions for the math functions used in the grpahics math header.
 * 
 * @author Alex Wills
 * @date February 15, 2023
 */

int* Interpolate(int i0, int d0, int i1, int d1, int* destination)
{
	

	// Output list of dependent values in the range [i0, i1] inclusive
	//static int dependent_values[i1 - i0 + 1];
	int* dependent_values = destination;
	float working_dependent = d0;	// This variable will be copied into the values array
	float slope = float(d1 - d0) / float(i1 - i0);	// Slope of the line

	// Add the dependent value and increment it by the slope
	for (int index = 0; index < i1 - i0 + 1; ++index)
	{
		*(dependent_values + index) = int(working_dependent);
		working_dependent += slope;
	}

	// Return the dependent values
	return dependent_values;
}

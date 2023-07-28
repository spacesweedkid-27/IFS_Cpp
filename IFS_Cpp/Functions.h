#pragma once
#include <xstring>

/*
	Class representing a function of an IFS.
	Therefore an object contains the parameters a,b,c,d,e,f and a Color for the function
	as well as the probability of the function being used in an IFS.
	The values x,y are calculated with: (x,y) -> (a*x + b*y + e, c*x + d*y + f).
	The Color part is not yet fully implemented so it is useless for now.
 */
class function
{
public:
	// Calculates the next x when this function is picked
	double calculate_x(double x, double y) const;
	// Calculates the next y when this function is picked
	double calculate_y(double x, double y) const;
	// TODO: Used for debugging, unused and has to be removed
	// Returns a string representation of a double array
	static std::string arr_to_str(double* arr, int length);
	// Constructor that initializes all values.
	function(double a, double b, double c, double d, double e, double f, int color[3], double prop);
	// Default constructor
	function();

	double prob;
private:
	// Color represented as a 3d tuple.
	// TODO: Implement as normal integer like in Java.Color.getRGB()
	int color[3]{};

	// The following are the parameters for the calculation of x and y

	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
};

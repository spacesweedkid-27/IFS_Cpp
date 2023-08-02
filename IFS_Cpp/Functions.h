#pragma once

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
	// Constructor that initializes all values.
	function(double a, double b, double c, double d, double e, double f, double prop);
	// Default constructor
	function();

	double prob;
private:
	// The following are the parameters for the calculation of x and y

	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
};

#include "Functions.h"

// Returns the next x described in Functions.h
double function::calculate_x(double x, double y) const
{
	// Don't mix up parameters
	return a * x + b * y + e;
}
// Returns the next y described in Functions.h
double function::calculate_y(double x, double y) const
{
	// Don't mix up parameters
	return c * x + d * y + f;
}
// Implements the constructor described in Functions.h
function::function(double a, double b, double c, double d, double e, double f, double prob)
{
	// Just set all attributes of the object.
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->e = e;
	this->f = f;
	this->prob = prob;
}
// Set to default
function::function() = default;


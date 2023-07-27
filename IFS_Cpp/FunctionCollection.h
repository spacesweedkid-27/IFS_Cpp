#pragma once
#include "Functions.h"

/*
	This class serves to simulate integrated functional systems.
	A object contains a collection of functions and can calculate the next position from a given point.
	The next function is picked randomly according to the probabilities of the functions in the collection array.
*/
class function_collection
{
public:
	// Calculates the x-value of the next position
	double calculate_x(double x, double y) const;
	// Calculates the y-value of the next position
	double calculate_y(double x, double y) const;
	// Inits an IFS with the input being an array of functions and the according length.
	function_collection(function collection[], const int length);
	// Returns a random index according to the probabilities in the collection.
	static int pick_index(function functions[], const int length);
	// Picks a random next index with "pick_index"
	void pick_own_index();
private:
	// Should point to the first element of a function array
	function *collection;
	// length of the array "collection"
	int length;
	// index of the function inside "collection" that will be used in the next iteration
	int current_random_picked_index;
};


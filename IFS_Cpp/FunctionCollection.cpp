#include "FunctionCollection.h"
#include <iostream>
#include <sstream>

// Implements calculate x as described
double function_collection::calculate_x(double x, double y) const
{
	// Use calculate_x of the function that should be used.
	return this->collection[current_random_picked_index].calculate_x(x, y);
}
// Implements calculate y as described
double function_collection::calculate_y(double x, double y) const
{
	// Use calculate_y of the function that should be used.
	return this->collection[current_random_picked_index].calculate_y(x, y);
}
// Sets collection and length as described
function_collection::function_collection(function *collection, const int length)
{
	// Start with Seed 0, if you want you can make this a parameter TODO: Make this a parameter or implement with std::cin
	std::srand(0);

	// Set the collection and length
	this->collection = collection;
	this->length = length;

	// Get the first random index;
	this->pick_own_index();
}

/*
	Probably the most complicated thing here.
	Sums up all probabilities so that there is an array sum that contains these sums.
	In this array there are gaps exactly as big as the probabilities of the functions.
	Now we pick a random double between 0 and 1 and check for every interval that is defined
	by the current neighbors in the sums array if the random number is inside this interval.
	If it is we know that the current index of the sums array contains the left of the interval,
	and the next element is the right of the interval. Therefore we return the current index.
 */

int function_collection::pick_index(function functions[], const int length)
{
	// Init the sums array as an array containing length + 1 zeroes.
	auto* sums = new double[length + 1] {0};

	// The first element of sum we remain 0,
	// but every following element will be the last element plus the probability.
	// Remember: We start at i = 1 so we have to access functions[i - 1] to access the right function.
	for (int i = 1; i < length + 1; ++i)
	{
		sums[i] = sums[i - 1] + functions[i - 1].prob;
	}

	// pick a random number between 0 and 1.
	// The call of std::rand() is not thread safe, so when this will be implemented with multiple threads,
	// TODO: implement with more threads or with CUDA
	// ... then it could perform not as wished.
	const double random = static_cast<double>(std::rand()) / RAND_MAX;

	// Now check for every direct interval in sums if random is in it.
	// If so, return the index of this interval.
	for (int i = 0; i < length + 1; i++) {
		if (sums[i] < random && random < sums[i + 1]) {
			return i;
		}
	}

	// Return 0 in case of bit error or if I forgot something.
	return 0;
}

void function_collection::pick_own_index()
{
	// Pretty simple, just pick_index of this and refresh the current one.
	this->current_random_picked_index = pick_index(this->collection, this->length);
	//std::cout << this->current_random_picked_index << std::endl;
}


// Not used, but useful for debugging.
// TODO: Marked for deletion
std::string function::arr_to_str(double* arr, int length)
{
	std::string temp = "[";

	for (int i = 0; i < length; ++i)
	{
		// Init string-buffer
		std::ostringstream strs;
		// Insert the double
		strs << arr[i];
		// Convert to string and append
		temp.append(strs.str() + ", ");
	}
	// Remove the last ", "
	temp.resize(temp.size() - 2);
	temp.append("]");

	return temp;
}

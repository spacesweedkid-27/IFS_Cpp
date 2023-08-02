#include "FunctionCollection.h"
#include <iostream>

// Random value between 0 and 1
#define RANDM (static_cast<double>(std::rand()) / RAND_MAX)

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
// START_SEED is the seed that the IFS will start at.
function_collection::function_collection(function *collection, const int length, const unsigned int START_SEED)
{
	// Set the start seed
	this->SEED = START_SEED;
	// Use the seed
	std::srand(SEED);

	// Set the collection and length
	this->collection = collection;
	this->length = length;

	// Make the IDE shut up 1
	this->current_random_picked_index = 0;

	// Get the first random index;
	this->pick_own_index();
}
// Initializes a fully random IFS with length length
// START_SEED is the seed that the IFS will start at while GEN_SEED is the seed used to generate the random IFS.
// TODO: Maybe add (un)-fairness to lower normal distribution between probs.
function_collection::function_collection(const int length,const unsigned int START_SEED, const unsigned int GEN_SEED)
{
	// Set the generative seed
	this->SEED = GEN_SEED;
	// Use the seed
	std::srand(SEED);

	// I fucking hate that my IDE wants to force me to use auto

	// The first thing we have to do is generate random probabilities for all functions.
	// I just realized the way I did this in my Java approach was way too complicated.
	// Just take length integers that have a random value between 0 and 1000 for example
	// and then compare these values to the total sum. Et voit la, you have your probs.
	auto* relative_prob = new int[length] {0};
	auto* total_prob = new double[length] {0};
	unsigned long sum = 0;

	for (int i = 0; i < length; ++i)
	{
		// Pick values between 0 and 0x7fff
		int random = std::rand();
		relative_prob[i] = random;
		sum += random;
	}

	// Now calculate the total_probabilities. After this the sum of total_prob should be 1
	for (int i = 0; i < length; ++i)
	{
		total_prob[i] = static_cast<double>(relative_prob[i]) / sum;
	}
	// We don't need it anymore so take it, Mr (or Ms?) garbage collector.
	delete[] relative_prob;

	// Now we only have to init the random parameters for the functions.
	auto* as = new double[length] {0};
	auto* bs = new double[length] {0};
	auto* cs = new double[length] {0};
	auto* ds = new double[length] {0};
	auto* es = new double[length] {0};
	auto* fs = new double[length] {0};

	for (int i = 0; i < length; ++i)
	{
		as[i] = 2 * RANDM - 1;
		bs[i] = 2 * RANDM - 1;
		cs[i] = 2 * RANDM - 1;
		ds[i] = 2 * RANDM - 1;
		es[i] = 2 * RANDM - 1;
		fs[i] = 2 * RANDM - 1;
	}

	// Now we have everything we need

	this->length = length;
	this->collection = new function[length];


	for (int i = 0; i < length; ++i)
	{
		this->collection[i] = function(
			as[i],
			bs[i],
			cs[i],
			ds[i],
			es[i],
			fs[i],
			total_prob[i]
		);
	}

	// Collect garbage
	delete[] total_prob;
	delete[] as;
	delete[] bs;
	delete[] cs;
	delete[] ds;
	delete[] es;
	delete[] fs;

	// Make the IDE shut up 2
	this->current_random_picked_index = 0;

	// Set the start seed for calculation
	this->SEED = START_SEED;
	// Use the seed
	std::srand(SEED);

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
	const double random = RANDM;

	// Now check for every direct interval in sums if random is in it.
	// If so, return the index of this interval.
	for (int i = 0; i < length + 1; i++) {
		if (sums[i] < random && random < sums[i + 1]) {
			delete[] sums;
			return i;
		}
	}

	delete[] sums;

	// Return 0 in case of bit error or if I forgot something.
	return 0;
}

void function_collection::pick_own_index()
{
	// Pretty simple, just pick_index of this and refresh the current one.
	this->current_random_picked_index = pick_index(this->collection, this->length);
	//std::cout << this->current_random_picked_index << std::endl;
}

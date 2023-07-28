#include "FunctionCollection.h"
#include <iostream>
#include <fstream>
#include <string>

// SKIP TO LINE 34 for the main

// Useless for now
// Contains some colors in the rgb format. TODO: Change these colors to their Java.Color.getRGB() counterpart aswell.
namespace colors
{
	int RED[3] = { 255, 0, 0 };
	int GREEN[3] = { 0, 255, 0 };
	int BLUE[3] = { 0, 0, 255 };
	int BLACK[3] = { 0, 0, 0 };
	int GREY[3] = { 128, 128, 128 };
}
// Contains some examples for IFS's
namespace examples
{
	// Example function collection from http://informatics4kids.de
	// Try this one yourself, if you don't want to but still want the output, I have linked the output of this program (plot.csv).
	function default__[4] = {
		function(0, 0, 0, 0.16, 0, 0, &(colors::RED[0]), 0.01),
		function(0.85, 0.04, -0.04, 0.85, 0, 1.6, &(colors::BLUE[0]), 0.85),
		function(0.2, -0.26, 0.23, 0.11, 0, 1.6, &(colors::GREEN[0]), 0.07),
		function(-0.15, 0.28, 0.26, 0.24, 0, 0.44, &(colors::GREEN[0]), 0.07)
	};
	// As function_collection object
	function_collection default_ = function_collection(default__, 4, 0);
}

// I am ok with typing std:: always but examples:: is to long for me.
using namespace examples;

constexpr char usage[] = "Usage: IFS.Cpp.exe <random / default> [?random: <GEN_SEED> <length>] <START_SEED>\n(START_SEED is used for the actual calculation of the data while GEN_SEED is used by the random function_collection to determine it's values)";

int main(int argc,char *argv[])
{
	// From here to line 54 we just check what the user wants.
	// TODO Implement Seeds from command line.

	// The argc should be in [1, 2] (we don't count the path to the executable)
	if (argc != 3 && argc != 5)
	{
		std::cout << usage << std::endl;
		return -1;
	}

	if (std::string(argv[1])._Equal("random") && argc == 5)
	{
		auto gen_seed_str = std::string(argv[2]);
		auto gen_seed = std::stoul(gen_seed_str, nullptr, 0);

		auto length_str = std::string(argv[3]);
		auto length = std::stoul(length_str, nullptr, 0);

		auto start_seed_str = std::string(argv[4]);
		auto start_seed = std::stoul(start_seed_str, nullptr, 0);

		auto test = function_collection(length, start_seed, gen_seed);

		//default_.~function_collection();

		default_ = test;
		std::cout << "starting random pattern with generative seed: " << gen_seed << " and start seed: " << start_seed << std::endl;


	} else if (!std::string(argv[1])._Equal("default") || argc != 3)
	{
		std::cout << usage << std::endl;
		return 1;
	}
	else
	{
		auto start_seed_str = std::string(argv[2]);
		auto start_seed = std::stoul(start_seed_str, nullptr, 0);
		// Set the start seed
		default_.SEED = start_seed;
	}
	

	// Write into this file which we'll name "plot.csv"
	std::ofstream plot;
	// open the file, if it already exists, overwrite it.
	plot.open("plot.csv", std::ios::out | std::ios::trunc);

	// This number works well for 2*4k resolution when plotting it.
	constexpr unsigned long long MAX = 20000000;

	// Initialize the positions
	double pos_x = 0.0;
	double pos_y = 0.0;

	// and the last positions.
	// we have to use them to avoid collision between calculate_x and calculate_y (x gets overwritten).
	double x = pos_x;
	double y = pos_y;

	// Stop after MAX iterations
	for (unsigned long long runner = 0; runner < MAX; runner++)
	{
		// Calculate the next positions
		pos_x = default_.calculate_x(x, y);
		pos_y = default_.calculate_y(x, y);

		// Pick the next function.
		default_.pick_own_index();

		// old -> new
		x = pos_x;
		y = pos_y;

		// add to plot.csv
		plot << pos_x << "\t" << pos_y << std::endl;

		// Makes it a bit slower, but you get a progress so that you can pick better MAX values.
		if (runner % (MAX / 100) == 0)
		{
			std::cout << "Progress: " << runner / static_cast<long double>(MAX) << std::endl;
		}
	}
	// close the file
	plot.close();
}

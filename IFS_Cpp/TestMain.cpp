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


// --help
constexpr char usage[] = "IFS.Cpp.exe <random / default> [?random: <GEN_SEED> <length>] <START_SEED> <append?> [?!append <plot directly?> [?plot directly <x-fit> <y-fit> <scale>]]\n(START_SEED is used for the actual calculation of the data while GEN_SEED is used by the random function_collection to determine it's values)";

// This number works well for 2*4k resolution when plotting it.
constexpr unsigned long long MAX = 20000000;

unsigned int bin(const char in[])
{
	return std::stoi(in, nullptr, 2);
}


int main(int argc, char *argv[])
{
	//std::cout << "test" << std::endl;
	
	// USAGE SHOULD BE:
	// IFS.Cpp.exe <random / default> [?random: <GEN_SEED> <length>] <START_SEED> <append?> [?!append <plot directly?> [?plot directly <x-fit> <y-fit> <scale>]]
	// So valid argc are 4 or 8 for default mode and 6 or 12 for random mode.
	// (if we want to plot directly, we have to give the parameters x-fit y-fit and scale)

	unsigned long start_seed;
	unsigned long gen_seed;
	unsigned long length;
	bool append;
	bool plot_directly = false;

	constexpr double RESOLUTION = 1;
	constexpr int HEIGHT = 1440 * RESOLUTION;
	constexpr int WIDTH = 2560 * RESOLUTION;


	int xFit = 0;
	int yFit = 0;
	int scale = 1;

	
	if (argc == 4 || argc == 5 || argc == 8)
	{
		// default mode
		if (argv[1] != std::string("default"))
		{
			std::cout << usage << std::endl;
			return 1;
		}

		start_seed = std::stoul(argv[2] , nullptr, 0);
		examples::default_.SEED = start_seed;

		append = (argv[3] == std::string("true"));

		if (!append)
		{
			plot_directly = (argv[4] == std::string("true"));
			if (plot_directly)
			{
				xFit = std::stoul(argv[5], nullptr, 0);
				yFit = std::stoul(argv[6], nullptr, 0);
				scale = std::stoul(argv[7], nullptr, 0);
			}
		}
	} else if (argc == 6 || argc == 7 || argc == 10)
	{
		// random mode
		if (argv[1] != std::string("random"))
		{
			std::cout << usage << std::endl;
			return 1;
		}

		gen_seed = std::stoul(argv[2], nullptr, 0);
		length = std::stoul(argv[3], nullptr, 0);
		start_seed = std::stoul(argv[4], nullptr, 0);
		append = (argv[5] == std::string("true"));

		if (!append)
		{
			plot_directly = (argv[6] == std::string("true"));
			if (plot_directly)
			{
				xFit = std::stoul(argv[7], nullptr, 0);
				yFit = std::stoul(argv[8], nullptr, 0);
				scale = std::stoul(argv[9], nullptr, 0);
			}
		}

		examples::default_ = function_collection(length, start_seed, gen_seed);

	} else
	{
		std::cout << usage << std::endl;
		return 1;
	}

	std::cout << "Starting calculation of " << argv[1] << " IFS plot." << std::endl;

	if (plot_directly)
	{
		// Initialize the positions
		double pos_x = 0.0;
		double pos_y = 0.0;

		// and the last positions.
		// we have to use them to avoid collision between calculate_x and calculate_y (x gets overwritten).
		double x = pos_x;
		double y = pos_y;

		int ix = 0;
		int iy = 0;

		auto (*image)[WIDTH] = new unsigned char[HEIGHT][WIDTH]{};

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				image[i][j] = static_cast<unsigned char>(bin("11111111"));
			}
		}

		// Stop after MAX iterations
		for (unsigned long long runner = 0; runner < MAX; runner++)
		{
			// Calculate the next positions
			pos_x = examples::default_.calculate_x(x, y);
			pos_y = examples::default_.calculate_y(x, y);

			// Pick the next function.
			examples::default_.pick_own_index();

			// old -> new
			x = pos_x;
			y = pos_y;

			ix = static_cast<int>(250 * RESOLUTION * (x * scale) + WIDTH / 2 + xFit * WIDTH);
			iy = static_cast<int>(250 * RESOLUTION * (y * scale) + HEIGHT / 2 + yFit * HEIGHT);

			if (0 <= ix && ix < WIDTH && 0 <= iy && iy < HEIGHT)
			{
				image[iy][ix] = static_cast<unsigned char>(0.99 * static_cast<unsigned short>(image[iy][ix]));
			}

			//std::cout << ix << " " << iy << std::endl;

			// Makes it a bit slower, but you get a progress so that you can pick better MAX values.
			if (runner % (MAX / 100) == 0)
			{
				std::cout << "Progress: " << static_cast<float>(runner) * 100 / static_cast<float>(MAX) << "%" << std::endl;
			}
		}

		std::cout << "Plotting directly..." << std::endl;

		std::ofstream plot1;
		plot1.open("plot.ppm", std::ios::out | std::ios::trunc);
		plot1 << "P2\n" << WIDTH << " " << HEIGHT << "\n255" << std::endl;
		
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {

				plot1 << static_cast<unsigned short>(image[i][j]) << " ";
				//plot1 << (image[i][j] & bin("111111110000000000000000") >> 16) << " ";
				//plot1 << (image[i][j] & bin("1111111100000000") >> 8) << " ";
				//plot1 << (image[i][j] & bin("11111111")) << " ";
			}
			plot1 << "\n";
		}

		// Delete the values to free RAM.
		delete[] image;

		plot1.close();

		std::cout << "Plot saved as \"plot.ppm\"" << std::endl;

	} else
	{
		std::cout << "Calculating plot data and saving to \"plot.csv\"" << std::endl;

		// Write into this file which we'll name "plot.csv"
		std::ofstream plot;
		// open the file, if it already exists, overwrite it.
		if (!append)
			plot.open("plot.csv", std::ios::out | std::ios::trunc);
		else
			plot.open("plot.csv", std::ios::out | std::ios::app);

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
			pos_x = examples::default_.calculate_x(x, y);
			pos_y = examples::default_.calculate_y(x, y);

			// Pick the next function.
			examples::default_.pick_own_index();

			// old -> new
			x = pos_x;
			y = pos_y;

			// add to plot.csv
			plot << pos_x << "\t" << pos_y << std::endl;

			// Makes it a bit slower, but you get a progress so that you can pick better MAX values.
			if (runner % (MAX / 100) == 0)
			{
				std::cout << "Progress: " << static_cast<float>(runner) * 100 / static_cast<float>(MAX) << "%" << std::endl;
			}
		}
		// close the file
		plot.close();
		std::cout << "Saved all plot data as \"plot.csv\"" << std::endl;
	}

	std::cout << "Finished." << std::endl;

	return 0;
	

}

/*
int old_main(int argc,char *argv[])
{
	// From here to line 54 we just check what the user wants.

	// The argc should be in [3, 5] (we don't count the path to the executable)

	bool append = false;

	if (argc != 5 && argc != 6)
	{
		std::cout << usage << std::endl;
		return -1;
	}

	if (std::string(argv[1])._Equal("random") && argc == 6)
	{
		auto gen_seed_str = std::string(argv[2]);
		auto gen_seed = std::stoul(gen_seed_str, nullptr, 0);

		auto length_str = std::string(argv[3]);
		auto length = std::stoul(length_str, nullptr, 0);

		auto start_seed_str = std::string(argv[4]);
		auto start_seed = std::stoul(start_seed_str, nullptr, 0);

		auto test = function_collection(length, start_seed, gen_seed);

		if (std::string(argv[5])._Equal("true"))
		{
			append = true;
		}

		default_ = test;
		std::cout << "starting random pattern with generative seed: " << gen_seed << " and start seed: " << start_seed << std::endl;


	} else if (!std::string(argv[1])._Equal("default"))
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

		if (std::string(argv[3])._Equal("true"))
		{
			append = true;
		}
	}
	

	// Write into this file which we'll name "plot.csv"
	std::ofstream plot;
	// open the file, if it already exists, overwrite it.
	if (!append)
		plot.open("plot.csv", std::ios::out | std::ios::trunc);
	else
		plot.open("plot.csv", std::ios::out | std::ios::app);

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
*/
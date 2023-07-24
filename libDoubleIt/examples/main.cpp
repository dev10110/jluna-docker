#include <iostream>
#include "double_it.hpp"

void print_array(std::string label, double * arr, std::size_t N)
{

	std::cout << label << ": ";
	for (size_t i=0; i<N; i++)
		std::cout << arr[i] << " ";

	std::cout << std::endl;
}


int main() {


	std::cout << "hello from cpp" << std::endl;

	// instantiate the library
	double_it::DoubleIt lib;

	// create the array we want to double, and the location of the output
	double in[5] = {1,2,3,4,5};
	double out[5];

	// print the arrays
	print_array("in", in, 5);
	print_array("out (before doubling)", out, 5);

	// now call the library
	lib.double_it(out, in, 5);

	// now print the doubled array
	print_array("out (after doubling)", out, 5);

	return 0;

}

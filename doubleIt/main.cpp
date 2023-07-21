#include <iostream>
#include <typeinfo>
#include <jluna.hpp>

#include <chrono>
#include <cstdlib>

using namespace jluna;
using namespace std::chrono;
using namespace std::placeholders;

// generic method to time a function call
template <typename F, typename ... Ts>
double timeit(F&& f, Ts&&...args)
{
	constexpr int TIMING_ITERS = 10000;

	auto start = std::chrono::high_resolution_clock::now();
	for (size_t ind = 0; ind < TIMING_ITERS ; ind++){
		std::forward<F>(f)(std::forward<Ts>(args)...);
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	double t = double(duration.count()) / double(TIMING_ITERS);

	return t;
}



void cpp_double_it(double* out, double* in, size_t N) {

	for (size_t i=0; i< N; i++){
		out[i] = 2 * in[i];
	}

	return;
}

void julia_double_it_wrapper(unsafe::Function* double_it_fn, double* out, double* in, size_t N) {

	auto* in_jl= unsafe::new_array_from_data(Float64_t, (void*) in, N);
	auto* out_jl= unsafe::new_array_from_data(Float64_t, (void*) out, N);

	jluna::safe_call(double_it_fn, out_jl, in_jl, box<std::size_t>(N));

	return;

}


bool check_is_equal(double * arr1, double* arr2, int N, double eps=1e-9){

	for (size_t i=0; i<N; i++){

		if (std::abs(arr1[i] - arr2[i]) > eps) {
			return false;
		}
	}
	return true;

}

void fill_rand(double* arr, size_t N){
	
	for (size_t i=0; i<N; ++i){
		arr[i] = (double) rand() / RAND_MAX;
	}

	return; 
}

int main(int argc, char* argv[])
{

	std::cout << "hello from cpp" << std::endl;

	initialize();
	Base["println"]("hello from julia");

	// get the module	
	std::string path = "/root/doubleIt/myModule.jl";
	Main.safe_eval_file(path);
	Module MyModule = Main.safe_eval("return MyModule");

	// create the binding to the julia function
	unsafe::Function * jl_double_it_impl = unsafe::get_function(MyModule, "double_it!"_sym);
	auto julia_double_it = std::bind(julia_double_it_wrapper, jl_double_it_impl, _1, _2, _3);


	// now define the test problem
	constexpr std::size_t N = 10000 ;


	// construct input array 
	double in[N];
	fill_rand(in, N);
	
	// allocate output memory (and fill it with random numbers)
	double cpp_out[N];
	fill_rand(cpp_out, N);

	cpp_double_it(cpp_out, in, N);

	// now run the julia version
	double jl_out[N];
	fill_rand(jl_out, N);
	julia_double_it(jl_out, in, N);

	// print the first few elements:
	for (size_t i=0; i<10; i++){
		std::cout << "[" << i << "] in: " << in[i] << " | C++: "<< cpp_out[i] << " |  Julia: " << jl_out[i] << std::endl;
	}

	// check equality
	bool res = check_is_equal(jl_out, cpp_out, N, 1e-9);
	if (res) {
		std::cout << "EQUALITY TEST PASSED" << std::endl;
	} else {
		std::cout << "!! EQUALITY TEST FAILED !!" << std::endl;
	}

	// run some timing
	double cpp_time = timeit(cpp_double_it, cpp_out, in, N);
	double julia_time = timeit(julia_double_it, jl_out, in, N);


	std::cout << "Time C++  : " << cpp_time << " us/loop" << std::endl;
	std::cout << "Time Julia: " << julia_time << " us/loop" << std::endl;


	return 0;
}

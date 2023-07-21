#include <iostream>
#include <typeinfo>
#include <jluna.hpp>

#include <chrono>

using namespace jluna;
using namespace std::chrono;


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
	std::cout << "Timing: " <<  t << " us / call" << std::endl;

	return t;
}

template <typename T>
void print_vec(std::vector<T> vec) {

	for (T v : vec) {
		std::cout << v << ", ";
	}
	std::cout << std::endl;

}


void cpp_double_it(double* out, double* in, size_t N) {

	for (size_t i=0; i< N; i++){
		out[i] = 2 * in[i];
	}

	return;
}


void julia_double_it_ip(unsafe::Function* double_it_fn, double* out, double* in, size_t N) {

	auto* in_jl= unsafe::new_array_from_data(Float64_t, (void*) in, N);
	auto* out_jl= unsafe::new_array_from_data(Float64_t, (void*) out, N);

	jluna::safe_call(double_it_fn, out_jl, in_jl, box<std::size_t>(N));

	return;

}

double* julia_double_it_raw( unsafe::Function* double_it_fn, double* in, size_t N){


	// create thin wrapper around the data
	// this should cause no reallocations
	auto* jl_array = unsafe::new_array_from_data(Float64_t, (void*) in, N);

	Array<Float64, 1> res = jluna::safe_call(double_it_fn, jl_array);

	double* out = reinterpret_cast<double*>(res.data());

	return out;

}


double* julia_double_it( double* in, size_t N){

	// first grab the function
	//std::string path = "/root/array/myModule.jl";
	//Main.safe_eval_file(path);
	Module MyModule = Main.safe_eval("return MyModule");
	auto unsafe_double_it = unsafe::get_function(MyModule, "double_it"_sym);

	return julia_double_it_raw(unsafe_double_it, in, N);
}



int main(int argc, char* argv[])
{

	std::cout << "hello from cpp" << std::endl;

	initialize();
	Base["println"]("hello from julia");

	// get the module	
	std::string path = "/root/array/myModule.jl";
	Main.safe_eval_file(path);
	Module MyModule = Main.safe_eval("return MyModule");
	auto double_it = MyModule["double_it"];

	double x = 5.0;
	double two_x = double_it(x);
	
	std::cout << two_x << std::endl;


	// test the arrays
	// first construct the cpp array
	size_t N = 1000;
	auto* arr = unsafe::new_array(Float64_t, 0);
	unsafe::sizehint(arr, N);
	
	for (size_t i = 1; i <= N; ++i){
		unsafe::push_back(arr, unsafe::unsafe_box<Float64>(1.0*i));
	}

	// call the julia function on the array
	auto res = double_it(arr);
	Array<Float64, 1> res2 = res;
	std::cout << typeid(res).name() << '\n';
	std::cout << typeid(res2).name() << '\n';




	/************************/
	/** try the unsafe way **/
	/************************/


	// // first grab the function
	// auto unsafe_double_it = unsafe::get_function(MyModule, "double_it"_sym);

	// // suppose we have some data
	// const double in_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	// double * out_array;

	// // create thin wrapper around the data
	// // this should cause no reallocations
	// auto* jl_array = unsafe::new_array_from_data(Float64_t, (void*) in_array, 10);
	// Array<Float64, 1> res4 = jluna::safe_call(unsafe_double_it, jl_array);

	// //Float64 * res5 = reinterpret_cast<Float64*>(res4.data());
	// out_array = reinterpret_cast<double*>(res4.data());

	// for (size_t i=0; i< 10; i++){
	// 	std::cout << out_array[i] << std::endl;
	// }


	constexpr size_t ARRAY_SIZE = 100000;
	const size_t TIMING_ITERS = 10000;

	// construct test data
	double in_array[ARRAY_SIZE];
	for (size_t i=0; i<ARRAY_SIZE; i++){
		in_array[i] = 1.0 * i;
	}


	// METHOD 1:
	double* out_array = julia_double_it(in_array, ARRAY_SIZE);
	for (size_t i=0; i< 10; i++){
	 	std::cout << out_array[i] << std::endl;
	}
	// timeit(julia_double_it, in_array, ARRAY_SIZE);


	// METHOD 2:
	auto unsafe_double_it = unsafe::get_function(MyModule, "double_it"_sym);
	double* out_array_2 = julia_double_it_raw(unsafe_double_it, in_array, ARRAY_SIZE);
	for (size_t i=0; i< 10; i++){
	 	std::cout << out_array_2[i] << std::endl;
	 }
	//timeit(julia_double_it_raw, unsafe_double_it, in_array, ARRAY_SIZE);
	

	//METHOD 3:
	double cpp_out[ARRAY_SIZE]; 
	cpp_double_it(cpp_out, in_array, ARRAY_SIZE);
	for (size_t i=0; i< 10; i++){
	 	std::cout << cpp_out[i] << std::endl;
	 }
	timeit( cpp_double_it, cpp_out, in_array, ARRAY_SIZE);




	/*************************************
	 *** Now try the in place methods ****
	 *************************************/

	unsafe::Function * unsafe_double_it_ip = unsafe::get_function(MyModule, "double_it_ip"_sym);
	double out_array2[ARRAY_SIZE];

	// // create thin wrapper around the data
	// auto* in_jl= unsafe::new_array_from_data(Float64_t, (void*) in_array, ARRAY_SIZE);
	// auto* out_jl= unsafe::new_array_from_data(Float64_t, (void*) out_array2, ARRAY_SIZE);

	// //unsafe::call(unsafe_double_it_ip, out_jl, in_jl, box<std::size_t>(ARRAY_SIZE));
	// jluna::safe_call(unsafe_double_it_ip, out_jl, in_jl, box<std::size_t>(ARRAY_SIZE));

	julia_double_it_ip(unsafe_double_it_ip, out_array2, in_array, ARRAY_SIZE);

	for (size_t i=0; i< 10; i++){
	 	std::cout << out_array2[i] << std::endl;
	 }


	{
		auto start = high_resolution_clock::now();
		for (size_t i=0; i < 10000;i++){
			julia_double_it_ip(unsafe_double_it_ip, out_array2, in_array, ARRAY_SIZE);
		}
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		auto t = (double)(duration.count()) / ((double)(10000));
		std::cout << " Time: "<< t <<  " us / loop" << std::endl;
	
	}


	// try to use bind to make life easier
	using namespace std::placeholders;
	auto julia_double_it_ip_bind = std::bind(julia_double_it_ip, unsafe_double_it_ip, _1, _2, _3);

	// now try running the command
	double out_3[ARRAY_SIZE];
	julia_double_it_ip_bind(out_3, in_array, ARRAY_SIZE);
	for (size_t i=0; i< 10; i++){
	 	std::cout << out_3[i] << std::endl;
	 }
	timeit(julia_double_it_ip_bind, out_3, in_array, ARRAY_SIZE);


	// TIMEIT
	{
		auto start = high_resolution_clock::now();
		for (size_t i=0; i < 10000;i++){
			unsafe::new_array_from_data(Float64_t, (void*) in_array, ARRAY_SIZE);
		}
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		auto t = (double)(duration.count()) / ((double)(10000));
		std::cout << " Time: "<< t <<  " us / loop" << std::endl;
	
	}

	return 0;
}

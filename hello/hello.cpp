#include <iostream>
#include <jluna.hpp>

#include <chrono>

using namespace jluna;

template <typename T>
void print_vec(std::vector<T> vec) {

	for (T v : vec) {
		std::cout << v << ", ";
	}
	std::cout << std::endl;

}



int main(int argc, char* argv[])
{

	std::cout << "hello from cpp" << std::endl;

	initialize();
	Base["println"]("hello from julia");

	auto array_proxy = Main.safe_eval("return [1, 2, 3, 4]");

  Int64 third = array_proxy[2];
  std::cout << third << std::endl;


  std::string path = "/root/hello/hello.jl";
  Main.safe_eval_file(path);


  auto fn_plus_pi = Main.safe_eval("return plus_pi");

  auto start = std::chrono::high_resolution_clock::now();
  double res = fn_plus_pi(1.0);
  auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "First time: " << diff.count() << " s\n";


  start = std::chrono::high_resolution_clock::now();
  for (size_t i=0; i<10000; i++){
    double r= fn_plus_pi(1.0);
  }
  end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "second time: " << diff.count()/10000 << " s\n";

  //std::cout << res << std::endl;
  

	// get the module	
  

	Module MyModule = Main.safe_eval("return MyModule");

	auto double_it = MyModule["double_it"];


	double x = 5.0;
	double two_x = double_it(x);
	
	std::cout << two_x << std::endl;


	std::vector<double> vec_x = {0, 1, 2, 3.0};
	for (size_t i=0; i< 10000; i++) {
		vec_x.push_back(i);
	}
	
		start = std::chrono::high_resolution_clock::now();
	std::vector<double> vec_two_x = double_it(vec_x);
	end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "first time: " << diff.count() << " s\n";
	
		start = std::chrono::high_resolution_clock::now();
	 vec_two_x = double_it(vec_x);
	end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "second time: " << diff.count() << " s\n";
	// std::cout << "vec_two_x: " ;
	// print_vec(vec_two_x);
	



// gc_pause; 
// 
// 	unsafe::Function* dit = unsafe::get_function(MyModule, "double_it"_sym);
// 	//unsafe::Array* raw_data = static_cast<unsafe::Array*>(vec_x);
// 
// 	// auto* res2 = unsafe::call(dit, raw_data);
// 	//
// 	//
// 	//
// 	Array<double, 10> vec = {0,1,2,3,4,5,6,7,8,9};
// 	unsafe::Array* raw_data = static_cast<unsafe::Array*>(vec);
// 
// 	unsafe::Value* out_data  = unsafe::call(dit, (unsafe::Value*) raw_data);
// gc_unpause; 
//



	std::cout << "\n\n\n\n\n\n";
	// allocate C-array
	constexpr size_t N = 100;
double c_array[N];
for (size_t i=0; i<N; i++){
	c_array[i] = 1.0 * i;
}
//= {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// create thin wrapper around it
auto* jl_array = unsafe::new_array_from_data(Float64_t, (void*) c_array, N);

// wrapper can now be used Julia-side
auto* dit = unsafe::get_function(MyModule, "double_it"_sym);
//auto* dit_ip = unsafe::get_function(MyModule, "double_it_ip!"_sym);



		start = std::chrono::high_resolution_clock::now();
	auto* res2 = jluna::safe_call(dit, jl_array);
	end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "first time: " << diff.count() << " s\n";
		
	start = std::chrono::high_resolution_clock::now();
	res2 = jluna::safe_call(dit, jl_array);
	end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "second time: " << diff.count() << " s\n";

	std::cout << "\n\n\n";

//double* res_d = reinterpret_cast<double*>(res2);

//std::cout << typeid(res2).name() << std::endl;

// Int32* array_data = reinterpret_cast<Int32*>(res2->data());

//auto* dit = unsafe::get_function(MyModule, "double_it"_sym);

{	
start = std::chrono::high_resolution_clock::now();
Array<Float64, N> res4 = unbox<Array<Float64, N>>(jluna::safe_call(dit, jl_array));
double* res5 = reinterpret_cast<double*>(res4.data());
	end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "first time: " << diff.count() << " s\n";
for (size_t i=0; i<10; i++){
	std::cout << double(res4[i] )<< ",";
	//std::cout << unsafe::get_index(res4,i)<< ",";
}
std::cout << std::endl;
}

{
		
	start = std::chrono::high_resolution_clock::now();
Array<Float64, N> res4 = unbox<Array<Float64, N>>(jluna::safe_call(dit, jl_array));
double* res5 = reinterpret_cast<double*>(res4.data());
	end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "second time: " << diff.count() << " s\n";
for (size_t i=0; i<10; i++){
	std::cout << double(res4[i] )<< ",";
	//std::cout << unsafe::get_index(res4,i)<< ",";
}
std::cout << std::endl;
}




Array<Float64, N> res4 = unbox<Array<Float64, N>>(jluna::safe_call(dit, jl_array));

double* res5 = reinterpret_cast<double*>(res4.data());

std::cout << res5[1] << std::endl;

double* res6 = res5;

double* res7 = static_cast<double*>(res5);

	return 0;
}

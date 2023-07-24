// here we define the implementation that hides the jluna dependencies


#include <jluna.hpp>
#include <cstdlib>


namespace double_it {

using namespace jluna;

class DoubleItImpl {

	public:
		DoubleItImpl();
		void double_it(double* out, double* in, std::size_t N);

	private:
		// this is the pointer to the actual julia function
		unsafe::Function * ptr_jl_double_it;

};

// constructor
DoubleItImpl::DoubleItImpl() {
	// initialize jluna here
	initialize();

	// get myModule
	std::string path = "/root/libDoubleIt/julia/myModule.jl";
	Main.safe_eval_file(path);
	Module myModule = Main.safe_eval("return MyModule");


	// get the julia function pointer
	ptr_jl_double_it = unsafe::get_function(myModule, "double_it!"_sym);

}

// define the double_it function
void DoubleItImpl::double_it(double* out, double* in, std::size_t N)
{

	// recast to jluna array types
	auto * in_jl = unsafe::new_array_from_data(Float64_t, (void*) in, N);
	auto * out_jl = unsafe::new_array_from_data(Float64_t, (void*) out, N);

	// call the julia double it function
	jluna::safe_call(ptr_jl_double_it, out_jl, in_jl, box<std::size_t>(N));

	return;

}


} // namespace double_it

#include "double_it.hpp"
#include "double_it_impl.hpp"


namespace double_it {


// in the constructor, allocate the implementation
DoubleIt::DoubleIt()
	: m_doubleItImpl ( new DoubleItImpl() )
{ }

// free the allocation
DoubleIt::~DoubleIt() { delete m_doubleItImpl; }

// call the implementation 
void DoubleIt::double_it(double* out, double* in, std::size_t N){
	return m_doubleItImpl -> double_it(out, in, N);
}


} // namespace double_it

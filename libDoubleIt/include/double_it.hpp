// this is the main interface file
// uses the pimpl paradigm  https://www.cppstories.com/2018/01/pimpl/

#include <cstdlib>

namespace double_it {

// forward declare the implementation class
class DoubleItImpl;


class DoubleIt 
{

	public:
		DoubleIt();
		~DoubleIt();
		void double_it(double* out, double* in, std::size_t N);

	private:
		DoubleItImpl * m_doubleItImpl;

};



} // namespace double_it

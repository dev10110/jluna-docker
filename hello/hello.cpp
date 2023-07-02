#include <iostream>
#include <jluna.hpp>

using namespace jluna;

int main(int argc, char* argv[])
{

	std::cout << "hello from cpp" << std::endl;

	initialize();
	Base["println"]("hello from julia");

	auto array_proxy = Main.safe_eval("return [1, 2, 3, 4]");

  Int64 third = array_proxy[2];
  std::cout << third << std::endl;
        
	return 0;
}

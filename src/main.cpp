#include <hackerchat/version.hpp>
#include <iostream>

int main()
{
	std::cout << "Hackerchat v" << hackerchat::get_version() << std::endl;
}

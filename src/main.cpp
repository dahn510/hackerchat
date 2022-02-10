#include <iostream>

#include <hackerchat/net.h>
#include <hackerchat/version.hpp>

enum class CustomMessageTypes: uint32_t
{
	From,
	Message
};
int main()
{
	hackerchat::net::message<CustomMessageTypes> msg;
	msg.header.id = CustomMessageTypes::From;

	int a = 1;
	bool b = true;
	float c = 3.123123;
	
	struct 
	{
		float x;
		float y;
	} d[5];

	msg << a << b << c << d;

	a = 99;
	b = false;
	c = 99.0;

	msg >> d >> c >> b >> a;

	std::cout << "msg >> "<<  d << " >> " <<  c << " >> " << b << " >> " << a << std::endl;

	return 0;
}

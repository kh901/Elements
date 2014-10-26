#include <iostream>
#include <exception>
#include "ServerController.h"

int main()
{
	try
	{
		ServerController servercontroller;
		servercontroller.run();
	}
	catch(std::exception e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
}

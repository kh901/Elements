#include <iostream>
#include <exception>
#include "UserController.h"

int main()
{
	try
	{
		UserController userCtrl;
		userCtrl.run();
	}
	catch(std::exception e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
}

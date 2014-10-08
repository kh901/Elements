#ifndef MENU_H
#define MENU_H

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

//INPUT STUFF
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
//END INPUT STUFF

class Menu
{
	public:
		Menu(std::ostringstream *, std::ostringstream *, int *);
		~Menu();
		
		void setOptions(const std::string, std::string[], const int, const int row = 0);
		void setDescriptions(std::string *);
		void setValues(int *);
		int doMenu();
	private:
		std::ostringstream * buffer;
		std::ostringstream * lastBuffer;
		int * bufferSize;
	
		std::string title;
		std::string * options;
		int optionNum;
		int displayRow;
		std::string * descriptions;
		int * values;

		int getCmd();
};

class MenuManager 
{
	public:
		MenuManager(const int);
		~MenuManager();

		
	private:
		int displayRow;
		std::ostringstream buffer;
		std::ostringstream lastBuffer;
		int bufferSize;
};

#endif

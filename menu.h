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
		static const int Back;
	
		Menu(std::ostringstream *, int *);
		~Menu();
		
		void setOptions(const std::string, std::string[], const int, const int row = 0);
		void setDescriptions(std::string *);
		void setValues(int *);
		void setPaged();
		void setScrolling();
		void setVisibleNum(const int);
		int doMenu();
		bool notExited(const int);
		void clear();
	private:
		// reference to buffer
		std::ostringstream * buffer;
		int * bufferSize;
	
		// menu data
		std::string title;
		std::string * options;
		int optionNum;
		int displayRow;
		std::string * descriptions;
		int * values;
		bool isPaged, isScroll;
		int visibleNum;
		int scrollIndex;

		int getCmd();
		void clearLastDisplay();
	protected:
		void displayOption(const int);
		void displayValue(const int);
		void displayDescription(const int);
		bool processInput(int &);
		void displayFixed(const int);
		void displayScroll(const int);
		void displayPaged(const int);
};

const int Menu::Back = -1;

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

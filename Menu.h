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

#include "common.h"

class Menu
{
	public:
		Menu();
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
		enum DisplayMode { Fixed, Scroll, Paged };
	
		// buffer shared by all menu objects
		static std::ostringstream buffer;
		static int bufferSize;
	
		// menu data
		std::string title;
		std::string * options;
		int optionNum;
		int displayRow;
		std::string * descriptions;
		int * values;
		DisplayMode displayMode; 
		int visibleNum;
		int scrollIndex;

		int getCmd();
		void clearLastDisplay();
	protected:
		void displayOption(const int);
		void displayValue(const int);
		void displayDescription(const int);
		bool processInput(int &);
		void display(const int, const int, const int);
		void displayFixed(const int);
		void displayScroll(const int);
		void displayPaged(const int);
		void displayScrollBar(const int);
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

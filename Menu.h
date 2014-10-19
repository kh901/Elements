#ifndef MENU_H
#define MENU_H

#include <cstdlib>
#include <cstring>
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

namespace text
{
	// Colours
	enum Colour {
		Colour_Black = 0,
		Colour_Red = 1,
		Colour_Green = 2,
		Colour_Yellow = 3,
		Colour_Blue = 4,
		Colour_Purple = 5,
		Colour_Cyan = 6,
		Colour_White = 7,
	};
	
	// Effects
	enum Effect {
		Effect_Bold = 1,
		Effect_Underline = 4,
		Effect_Intense = 9,
		Effect_None = 0
	};
	
	// Backgrounds
	enum Background {
		Bkg_Black = 0,
		Bkg_Red = 1,
		Bkg_Green = 2,
		Bkg_Yellow = 3,
		Bkg_Blue = 4,
		Bkg_Purple = 5,
		Bkg_Cyan = 6,
		Bkg_White = 7,
		Bkg_None = 8
	};
	
	std::string styleString(const std::string & str, Colour col = Colour_White,
		 Effect eft = Effect_None, Background bkg = Bkg_None);
};

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
		void setShowControls();
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
		bool showControls;
		
		/*
			Menu themes?
			
			Should we be able to set themes for their menus?
			Some pre set themes or even custom ones?
			Change how the title is displayed, what character is used for the bar,
			how long the bar width is, what character is used as the cursor,
			how the highlighted option is displayed (colour, effect, background, extra text),
			how unhighlighted options are displayed (colour, effect, background, extra text),
			how the last option is displayed (colour, effect, background, extra text),
			and more?
		*/

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
		void displayControls();
};

#endif

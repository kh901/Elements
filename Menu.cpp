#include "Menu.h"

std::ostringstream Menu::buffer;
int Menu::bufferSize;

Menu::Menu()
{
	options = NULL;
	descriptions = NULL;
	values = NULL;
	displayMode = Menu::Fixed;
	visibleNum = 1;
	scrollIndex = 0;
	showControls = false;
}
Menu::~Menu()
{
	options = NULL;
	descriptions = NULL;
	values = NULL;
}
void Menu::setOptions(const std::string theTitle, std::string theOptions[], const int num, const int row)
{
	title = theTitle;
	options = theOptions;
	optionNum = num;
	displayRow = row;
	visibleNum = num;
}
void Menu::setDescriptions(std::string * desc)
{
	descriptions = desc;
}
void Menu::setValues(int * val)
{
	values = val;
}
void Menu::setPaged()
{
	displayMode = Menu::Paged;
}
void Menu::setScrolling()
{
	displayMode = Menu::Scroll;
}
void Menu::setVisibleNum(const int num)
{
	visibleNum = num;
}
void Menu::setShowControls() 
{
	showControls = true;
}
void Menu::clearLastDisplay()
{
	std::string tmpbuf;
	if (Menu::bufferSize > 0)
	{
		//Reposition the cursor
		std::cout << "\x1B[" << displayRow << ";1f";

		// save the buffer to a local string to save multiple calls
		tmpbuf = Menu::buffer.str();

		// overwrites the last display by each character
		for(int i = 0; i < Menu::bufferSize; i++)
		{
			if(tmpbuf[i] == '\n')
			{
				std::cout << '\n';
			}
			else
			{
				std::cout << ' ';
			}
		}

		// ready the buffer for the next thing to display
		Menu::bufferSize = 0;		
		Menu::buffer.str("");
		Menu::buffer.clear();
	}
}
void Menu::displayValue(const int i)
{
	if (values)
	{
		Menu::buffer << std::setw(18 - options[i].length()) << values[i];
	}
}
void Menu::displayDescription(const int option)
{
	if (descriptions)
	{
		Menu::buffer << descriptions[option] << std::endl;
	}	
}
bool Menu::processInput(int &option)
{
	static const int ENTERKEY = 10, UPKEY = 65, DOWNKEY = 66;
	static const int LEFTKEY = 68, RIGHTKEY = 67;
	static const int BACKSPACE = 8, DELETE = 127;
	
	int key = getCmd();
	
	if(key == ENTERKEY)
	{
		return true;
	}
	else
	{
		switch(key)
		{
			case UPKEY:	//Up
				if(option > 0)
				{ 
					option--; 
					// if there is room to display the visible options
					if (displayMode == Menu::Scroll && option < (optionNum - visibleNum + 1))
					{
						scrollIndex = option;
					}
					if (displayMode == Menu::Paged && ((option+1) % visibleNum) == 0)
					{
						scrollIndex = (option / visibleNum) * visibleNum;		
					}
				}
				else
				{ 
					option = optionNum -1; 
					if (displayMode == Menu::Scroll)
					{
						scrollIndex = option - visibleNum + 1;
					}
					if (displayMode == Menu::Paged)
					{
						scrollIndex = (option / visibleNum) * visibleNum;
					}
				}
			break;

			case DOWNKEY:		//Down
				if(option < optionNum -1)
				{ 
					option++; 
					if (displayMode == Menu::Scroll && option < (optionNum - visibleNum + 1))
					{
						scrollIndex = option;
					}
					if (displayMode == Menu::Paged && ((option) % visibleNum) == 0)
					{
						scrollIndex = option;
					}
				}
				else
				{ 
					option = 0; 
					if (displayMode == Menu::Scroll || displayMode == Menu::Paged)
					{
						scrollIndex = option;
					}
				}
			break;

			case LEFTKEY:	//Reduce value
				if (values)
				{
					if (values[option] > 0)
					{ values[option]--; }
				}
			break;
			
			case RIGHTKEY:	//Increase value
				if (values)
				{
					if (values[option] < 10)		// Todo: change to actual max value 
					{ values[option]++; }
				}
			break;

			case BACKSPACE:     //Back/Cancel - Goes back up one menu level
            case DELETE:
            	option = -1;	
            	return true;
			break;
		}
	}
	return false;
}
void Menu::displayOption(const int i)
{
	Menu::buffer << options[i];
}
void Menu::display(const int start, const int end, const int option)
{
	for (int i = start; i < end; i++)
	{
		// current selection is highlighted
		if(option == i)
		{
			Menu::buffer << "> ";
			Menu::buffer << text::styleString(options[i], text::Colour_Green, text::Effect_Bold);
		}
		else
		{
			Menu::buffer << "  ";
			displayOption(i);
		}
		
		displayValue(i);
		Menu::buffer << std::endl;
	}
}
void Menu::displayFixed(const int option)
{
	display(0, optionNum, option);
}
void Menu::displayScroll(const int option)
{
	int end = scrollIndex + visibleNum;
	if (end > optionNum) { end = optionNum; }
	if (scrollIndex < 0) { scrollIndex = 0; }
	display(scrollIndex, end, option);
	displayScrollBar(option);	
}
void Menu::displayScrollBar(const int option)
{
	Menu::buffer << "  ";
	std::string scrollBar;
	int barWidth = 24;
	int barOptionPos = ((option+1) * barWidth)/optionNum;
    for(int i = 0; i < barOptionPos-2; ++i)
    {
        scrollBar += "-";
    }
    scrollBar += "||";
    int barOptionFill = ((optionNum-(option+1)) * barWidth)/optionNum;
    for(int j = 0; j < barOptionFill; ++j)
    {
        scrollBar += "-";
    }
    Menu::buffer << scrollBar << std::endl;
}
void Menu::displayPaged(const int option)
{
	int end = scrollIndex + visibleNum;
	if (end > optionNum) { end = optionNum; }
	if (scrollIndex < 0) { scrollIndex = 0; }
	display(scrollIndex, end, option);	
	Menu::buffer << "  Page " << (scrollIndex / visibleNum)+1 << " of " << Ceil(optionNum, visibleNum) << std::endl;
}
void Menu::displayControls()
{
	static char controlHint [] = "Controls: Up, Down - Cursor Movement\nEnter - Select Option\nBackspace - Return to previous menu";
	if (showControls)
	{
		Menu::buffer << controlHint << std::endl;
	}
}
int Menu::doMenu()
{
	static char BAR [] = "********************************";
	
	bool selected = false;
	int option = 0;
	std::string tmpbuf;
	scrollIndex = 0;
	
	while (!selected)
	{
		this->clearLastDisplay();
		//Don't redraw the grid, reposition cursor
		//ROW;COLUMN
		Menu::buffer << "\x1B[" << displayRow << ";1f";

		Menu::buffer << title << std::endl;
		Menu::buffer << BAR << std::endl;

		// display the content of the menu
		switch(displayMode)
		{
			case Menu::Fixed: 	displayFixed(option); 	break;
			case Menu::Scroll: 	displayScroll(option); 	break;
			case Menu::Paged: 	displayPaged(option);	break;
		}
		Menu::buffer << BAR << std::endl;	
		displayDescription(option);
		displayControls();
		
		// store the buffer in a local string to optimise
		tmpbuf = Menu::buffer.str();
		
		// Set the buffer size	
		Menu::bufferSize = tmpbuf.length();

		// Output the buffer to the screen
		std::cout << tmpbuf;

		// Get and process input
		selected = processInput(option);
	}
	return option;
}
int Menu::getCmd()
{
	//To keep terminal settings
  	struct termios oldt, newt;
	//Stores character as integer
  	int ch;
	//Get current settings
  	tcgetattr( STDIN_FILENO, &oldt );
  	newt = oldt;
	//Set new flags
  	newt.c_lflag &= ~( ICANON | ECHO );
	//Set new terminal settings
  	tcsetattr( STDIN_FILENO, TCSANOW, &newt );

	//Get content (arrow keys or enter)
  	ch = getchar();
  	if (ch == 27)	//If the character is a '^[', it's probably an arrow key
	{
		ch = getchar();
		ch = getchar();
	}	

	//Reset terminal settings to old ones
  	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

	//Return
  	return ch;
}
bool Menu::notExited(const int result)
{
	return (result != optionNum-1 && result != -1);
}
void Menu::clear()
{
	this->clearLastDisplay();
	//Reposition the cursor
	std::cout << "\x1B[" << displayRow << ";1f";
}

/*
	Menu test;
	test.setOptions("Normal Menu", opts, 3);
	switch(test.doMenu())
	{
		case 0:
			
		break;
		case 1:
			
		break;
		case 2:
			
		break;
	}

	Menu vtest;
	vtest.setOptions("Value Menu", opts, 3);
	vtest.setValues(val);
	cout << "Picked: " << vtest.doMenu() << endl;

	Menu dtest;
	dtest.setOptions("Describe Menu", opts, 3);
	dtest.setDescriptions(desc);
	cout << "Picked: " << dtest.doMenu() << endl;
	*/

/*
using namespace std;

int main ()
{
	ostringstream buf;
	int bufSize = 0;

	//string title = "Title";
	//string opts[3] = {"Option 1", "Option 2", "Option 3"};
	//string desc[3] = {"Describe 1", "Describe 2", "Describe 3"};
	//int val[3] = {1, 2, 3};

	//Clear and reset cursor
	cout << "\033[2J";
	cout << "\x1B[1;1f";

	
	
	string menuOptions[6] = {
		"MY ACCOUNT",
		"PAPER SUBMISSIONS",
		"NOTIFICATIONS", 
		"OPTIONS",
		"REVIEW PAPERS",
		"LOG OUT"
	};
	string menuDesc[6] = {
		"View information about your account.", 
		"Manage paper submissions.", 
		"Manage notifications.",
		"Configure options.",
		"Manage reviews for assigned papers.",
		"Log out of the system."
	};
	
	string notifyOpts [2] = {
		"VIEW NOTIFICATIONS",
		"BACK"
	};
	
	string * testNotifications = NULL;
	testNotifications = new string[20];
	ostringstream os;
	for (int i = 0; i < 20; ++i)
	{
		os.str("");
		os << "Test " << i+1;
		testNotifications[i] = os.str();
	}
	
	Menu viewNotifyMenu;
	viewNotifyMenu.setOptions("Notifications > View", testNotifications, 20);
	viewNotifyMenu.setScrolling();
	viewNotifyMenu.setVisibleNum(5);
	
	Menu notifyMenu;
	notifyMenu.setOptions("Main Menu > Notifications", notifyOpts, 2);
	notifyMenu.setScrolling();
	
	Menu mainMenu;
	mainMenu.setOptions("Main Menu", menuOptions, 6);
	mainMenu.setDescriptions(menuDesc);
	//mainMenu.setScrolling();
	mainMenu.setPaged();  
	mainMenu.setVisibleNum(4);
	int menuResult = 0;
	while (mainMenu.notExited(menuResult))
	{
		menuResult = mainMenu.doMenu();
		switch(menuResult)
		{
			case 0: break;
			case 1: break;
			case 2: 
				menuResult = 0;
				while(notifyMenu.notExited(menuResult))
				{
					menuResult = notifyMenu.doMenu();
					switch(menuResult)
					{
						case 0: 
							menuResult = 0;
							while(viewNotifyMenu.notExited(menuResult))
							{
								menuResult = viewNotifyMenu.doMenu();
								switch(menuResult)
								{
									case 19: case -1: break;
								}
							}
							menuResult = -2;
						break;
						case 1: case -1: break;
					}
				}
				menuResult = 0;
			break;
			case 3: break;
			case 4: break;
			case 5: case -1: break;
		}
	}
	
	delete [] testNotifications;
	testNotifications = NULL;
}
*/

std::string text::styleString(const std::string & str, text::Colour col, text::Effect eft, text::Background bkg)
{
	std::ostringstream os;
	os << "\033[";
	
	//os << '[';
	// apply effect
	if (eft != text::Effect_Intense)
	{
		os << eft << ';';
		os << '3';
	}
	else if (eft == text::Effect_Intense)
	{
		os << "0;" << eft;
	}
	// apply colour
	os << col << 'm';
	// apply background
	if (bkg != text::Bkg_None)
	{
		os << "\033[4" << bkg << "m";
	}
	// insert string
	os << str << "\033[0m";
	return os.str();
}
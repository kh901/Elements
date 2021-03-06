#include "Menu.h"

std::ostringstream Menu::buffer;
int Menu::bufferSize;

Menu::Menu()
{
	options = NULL;
	option = 0;
	descriptions = NULL;
	values = NULL;
	displayMode = Menu::Fixed;
	visibleNum = 1;
	scrollIndex = 0;
	showControls = disableBack = false;
	doesLastReturn = true;
	selectColour = text::Colour_Green;
	barWidth = MENU_BAR_DEFAULT_WIDTH;
	fillBar();
	minVal = MENU_VALUES_MIN;
	maxVal = MENU_VALUES_MAX;
	hasMarquee = false;
	inputTimeout = 350000;		// initial timeout is 0.35 seconds
	msgPos = barWidth;
	animFrame = 0;
}
Menu::~Menu()
{
	options = NULL;
	descriptions = NULL;
	values = NULL;
	unsetTimeout();
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
void Menu::setSelectColour(const text::Colour col)
{
	selectColour = col;
}
void Menu::setBarWidth(const int width)
{
	if (width >= 0)
	{
		barWidth = width;
		fillBar();
		msgPos = barWidth;
	}
}
void Menu::fillBar()
{
	BAR.clear();
	BAR.append(barWidth, '*');
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
		Menu::buffer << std::setw(MENU_VALUES_OFFSET - options[i].length()) << values[i];
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
	static const int ENTERKEY = 10, UPKEY = 65 + 128, DOWNKEY = 66 + 128;
	static const int LEFTKEY = 68 + 128, RIGHTKEY = 67 + 128;
	static const int BACKSPACE = 8, DELETE = 127;
	
	int key;
	if (hasMarquee)
	{
		key = getTimeoutCmd();
	}
	else
	{
	 	key = getCmd();
	}
	
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
					if (values[option] > minVal)
					{ values[option]--; }
				}
			break;
			
			case RIGHTKEY:	//Increase value
				if (values)
				{
					if (values[option] < maxVal)		// Todo: change to actual max value 
					{ values[option]++; }
				}
			break;

			case BACKSPACE:     //Back/Cancel - Goes back up one menu level
            case DELETE:
            	if (!disableBack)
            	{
            		option = -1;
            		return true;	
            	}
			break;
		}
	}
	return false;
}
void Menu::setValueBounds(const int aMin, const int aMax)
{
	minVal = aMin;
	maxVal = aMax;
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
			Menu::buffer << text::styleString(options[i], selectColour, text::Effect_Bold);
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
	int sBarWidth = barWidth - 4;
	int barOptionPos = ((option+1) * sBarWidth)/optionNum;
    for(int i = 0; i < barOptionPos-2; ++i)
    {
        scrollBar += "-";
    }
    scrollBar += "||";
    int barOptionFill = ((optionNum-(option+1)) * sBarWidth)/optionNum;
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
	if (showControls)
	{
		Menu::buffer << "Controls:\n Up, Down Arrow - Cursor Movement\nEnter - Select Option" << std::endl;
		if (values != NULL)
		{
			Menu::buffer << "Left, Right Arrow - Change values" << std::endl;
		}
		if (!disableBack)
		{
			Menu::buffer << "Backspace - Return to previous menu" << std::endl;
		}
	}
}
int Menu::doMenu()
{	
	bool selected = false;
	//int option = 0;
	if (displayMode == Menu::Fixed)
	{
		scrollIndex = 0;
	}
	if (hasMarquee)
	{
		setTimeout();
	}
	std::string tmpbuf;
	
	while (!selected)
	{
		this->clearLastDisplay();
		//Don't redraw the grid, reposition cursor
		//ROW;COLUMN
		Menu::buffer << "\x1B[" << displayRow << ";1f";

		Menu::buffer << title << std::endl;
		if (hasMarquee) { displayMarquee(); }
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
	if (hasMarquee)
	{
		unsetTimeout();
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
		ch = getchar() + 128;
	}	

	//Reset terminal settings to old ones
  	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

	//Return
  	return ch;
}
void Menu::unsetTimeout()
{
	struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
	//tty.c_lflag |= ECHO;
	tty.c_lflag |= ( ICANON | ECHO );

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	//Reset terminal settings to old ones
  	//tcsetattr( STDIN_FILENO, TCSANOW, &oldSettings );
}
void Menu::setMarquee(const std::string &msg, const double timeout)
{
	marqueeMsg = msg;
	// save old settings for terminal
	if (!hasMarquee)
	{
		setTimeout();
  		hasMarquee = true;
  	}
  	if (timeout > 0.1 && timeout < 5)
  	{
  		inputTimeout = timeout * 1000000;
  	}
}
void Menu::setTimeout()
{
	//To keep terminal settings
  	struct termios newSettings, oldSettings;
	//Get current settings
  	tcgetattr( STDIN_FILENO, &oldSettings );
  	newSettings = oldSettings;
	//Set new flags
  	newSettings.c_lflag &= ~( ICANON | ECHO );
	//Set new terminal settings
  	tcsetattr( STDIN_FILENO, TCSANOW, &newSettings );
}
void Menu::displayMarquee()
{
	std::string tmp;
	int msgSize = marqueeMsg.length();
	for (int i = 0; i < barWidth; ++i)
	{
		tmp.clear();
		if (i < msgPos || i > (msgPos + msgSize -1))
		{
			tmp += ' ';
			Menu::buffer << text::styleString(tmp, text::Colour_White, text::Effect_Bold, text::Bkg_Black);
		}
		else if (i >= msgPos && i <= (msgPos + msgSize - 1))
		{
			tmp += marqueeMsg[i - msgPos];
			Menu::buffer << text::styleString(tmp, text::Colour_Yellow, text::Effect_Bold, text::Bkg_Black);
		}
	}
	Menu::buffer << std::endl;
	msgPos--;
	if (animFrame < (msgSize + barWidth))
	{
		++animFrame;
	}
	else
	{
		animFrame = 0;
		msgPos = barWidth;
	}
}
int Menu::getTimeoutCmd()
{
	fd_set fdset;
	struct timeval timeout;
   	int  rc;
   	int  value;

   	timeout.tv_sec = 0;   /* wait for x seconds for data */
   	timeout.tv_usec = inputTimeout;		/* 500000 is 0.5 seconds ? */

   	FD_ZERO(&fdset);
   	FD_SET(0, &fdset);

   	rc = select(1, &fdset, NULL, NULL, &timeout);
   	if (rc == -1)  /* select failed */
   	{
	   	value = -1;
   	}
   	else if (rc == 0)  /* select timed out */
   	{
	   	value = -1;
   	}
   	else 
   	{
	  	//Get content (arrow keys or enter)
	  	value = getchar();
	  	if (value == 27)	//If the character is a '^[', it's probably an arrow key
		{
			value = getchar();
			value = getchar() + 128;
		}	
   	}
   	return value;
}
bool Menu::notExited(const int result)
{
	return (!((result == optionNum-1 && doesLastReturn) || result == -1));
}
void Menu::clear()
{
	this->clearLastDisplay();
	//Reposition the cursor
	std::cout << "\x1B[" << displayRow << ";1f";
}

void Menu::disableBackButton()
{
	disableBack = true;
}

void Menu::setLastAsChoice()
{
	doesLastReturn = false;
}

void Menu::eraseLine(const std::string &s)
{
	Menu::eraseLine(s.length());
}
void Menu::eraseLine(int len)
{
	// move cursor up a line
	std::cout << "\x1b[A";
	if (len < 0) { len = 0; }
	// overwrite the line with spaces
	for (int i = 0; i < len; ++i)
	{
		std::cout << ' ';
	}
	// reset cursor to start of line
	for (int i = 0; i < len; ++i)
	{
		std::cout << '\b';
	}
}
void Menu::clearDisplay()
{
	//Clear and reset cursor
	std::cout << "\033[2J";
	std::cout << "\x1B[1;1f";
}

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

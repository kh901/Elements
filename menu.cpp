#include "menu.h"

Menu::Menu(std::ostringstream *buf, std::ostringstream *prevBuf, int *bufSize)
{
	buffer = buf;
	lastBuffer = prevBuf;
	bufferSize = bufSize;

	options = NULL;
	descriptions = NULL;
	values = NULL;
}
Menu::~Menu()
{
	buffer = NULL;
	lastBuffer = NULL;
	bufferSize = NULL;
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
}
void Menu::setDescriptions(std::string * desc)
{
	descriptions = desc;
}
void Menu::setValues(int * val)
{
	values = val;
}
void Menu::clearLastBuffer()
{
	std::string tmpbuf;
	if (*bufferSize > 0)
	{
		//Reposition the cursor
		std::cout << "\x1B[" << displayRow << ";1f";

		//Copy last buf to string
		tmpbuf = lastBuffer->str();

		//Clear the screen for the last buffer size
		for(int i = 0; i < *bufferSize; i++)
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

		*bufferSize = 0;

		//clear last buf
		lastBuffer->str("");
		lastBuffer->clear();
	}
}
int Menu::doMenu()
{
	static char BAR [] = "************************";
	std::string tmpbuf;
	bool selected = false;
	const int ENTERKEY = 10, UPKEY = 65, DOWNKEY = 66, BACKSPACE = 8, DELETE = 127;
	const int LEFTKEY = 68, RIGHTKEY = 67;
	int option = 0;

	while (!selected)
	{
		this->clearLastBuffer();
		//Don't redraw the grid, reposition cursor
		//ROW;COLUMN
		*buffer << "\x1B[" << displayRow << ";1f";

		*buffer << title << std::endl;
		*buffer << BAR << std::endl;

		for (int i = 0; i < optionNum; i++)
		{
			if(option == i)
			{
				*buffer << "> ";
			}
			else
			{
				*buffer << "  ";
			}

			*buffer << options[i];
			if (values)
			{
				*buffer << std::setw(18 - options[i].length()) << values[i];
			}
			*buffer << std::endl;
		}		
		*buffer << BAR << std::endl;	
		if (descriptions)
		{
			*buffer << descriptions[option] << std::endl;
		}	

		//Copy the buffer to a temporary string
		tmpbuf = buffer->str();

		//Copy one instance of the buffer so we can overwrite it later
		if(*bufferSize == 0)
		{
			*lastBuffer << buffer->str();
		}

		//Set the buffer size	
		*bufferSize = tmpbuf.length();

		//Output the buffer to the screen
		std::cout << buffer->str();

		//Clear the buffer
		buffer->str("");
		buffer->clear();

		//Get and process input
		int key = getCmd();

		if(key == ENTERKEY)
		{
			selected = true;
		}
		else
		{
			switch(key)
			{
				case UPKEY:	//Up
					if(option > 0)
					{ option--; }
					else
					{ option = optionNum -1; }
				break;

				case DOWNKEY:		//Down
					if(option < optionNum -1)
					{ option++; }
					else
					{ option = 0; }
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

				case BACKSPACE:     //Back/Cancel
                case DELETE:
					
				break;
			}
		}

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

using namespace std;

int main ()
{
	ostringstream buf, lastBuf;
	int bufSize = 0;

	string title = "Title";
	string opts[3] = {"Option 1", "Option 2", "Option 3"};
	string desc[3] = {"Describe 1", "Describe 2", "Describe 3"};
	int val[3] = {1, 2, 3};

	//Clear and reset cursor
	cout << "\033[2J";
	cout << "\x1B[1;1f";

	Menu test(&buf, &lastBuf, &bufSize);
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

	Menu vtest(&buf, &lastBuf, &bufSize);
	vtest.setOptions("Value Menu", opts, 3);
	vtest.setValues(val);
	cout << "Picked: " << vtest.doMenu() << endl;

	Menu dtest(&buf, &lastBuf, &bufSize);
	dtest.setOptions("Describe Menu", opts, 3);
	dtest.setDescriptions(desc);
	cout << "Picked: " << dtest.doMenu() << endl;
	
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
	
	Menu mainMenu(&buf, &lastBuf, &bufSize);
	mainMenu.setOptions("Main Menu", menuOptions, 6);
	mainMenu.setDescriptions(menuDesc);
	int menuResult = -1;
	while (menuResult != 5)
	{
		menuResult = mainMenu.doMenu();
		switch(menuResult)
		{
			case 0: break;
			case 1: break;
			case 2: break;
			case 3: break;
			case 4: break;
			case 5: break;
		}
	}
}

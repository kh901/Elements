/*
sendLog(" has submitted a paper.")
void sendLog(string details)
{
    time_t now = time(0);
    char* dt = ctime(&now);
    string log = dt + " " + username + " " + details;
    // send log through socket to database
}
*/


#include "UserController.h"

int main ()
{
	UserController uc;
	uc.run();
}

UserController::UserController()
{
    
}

bool UserController::connect()
{
	sf::Time time = sf::seconds(7);
	
	std::string serverAddress;
	std::cout << "Enter server address: ";
	getline(std::cin, serverAddress);
	
	sf::Socket::Status status = socket.connect(serverAddress.c_str(), NETWORK_PORT, time);
	
	return (status == sf::Socket::Done);
}
void UserController::run()
{
	if (!this->connect())
	{
		std::cout << "Could not connect to server." << std::endl;
		return;
	}
	//Clear and reset cursor
	std::cout << "\033[2J";
	std::cout << "\x1B[1;1f";
	this->startMenu();
}
void UserController::startMenu()
{
	std::string options[] = {
		"Login",
		"Register",
		"Exit"
	};
	std::string descriptions[] = {
		"Log in as an existing user",
		"Register a new account",
		"Exit from the system"
	};
	Menu start;
	start.setOptions("Start Menu", options, 3);
	start.setDescriptions(descriptions);
	int option;
	do
	{
		option = start.doMenu();
		switch(option)
		{
			// Login in
			case 0: this->loginAccount();
			break;
			// Register
			case 1: this->registerAccount();
			break;
			// Exit
			case 2: case -1:
			break;
		}
		if (option == 1 || option == 0)
		{
			// here: get system admin status
			if (this->pickConference())
			{
				this->mainMenu();
			}
		}
	} while (start.notExited(option));
}

bool UserController::pickConference()
{
	sf::Packet request, response;
	std::string requestProtocol = "CONFERENCE_LIST";
	request << requestProtocol << username;
	std::vector<std::string> conferences;
	int conferenceSize = 0;
	
	// request a list of conferences from the server
	socket.send(request);
	socket.receive(response);
	response >> conferenceSize;
	std::string tmpconference;
	
	for (int i = 0; i < conferenceSize; ++i)
	{
		response >> tmpconference;
		conferences.push_back(tmpconference);
	}
	if (conferenceSize == 0) 
	{
		std::cout << "No conferences!" << std::endl;
		// admins can create conferences if none exist here
		return false;
	}
	int pick = 0;
	Menu pickMenu;
	pickMenu.setOptions("Pick a conference:", &conferences[0], conferences.size());
	pickMenu.disableBackButton();
	pick = pickMenu.doMenu();
	this->conference = conferences[pick];
	this->getConferenceAccess();
	return true;
}

void UserController::getConferenceAccess()
{
	// get the user's access level from the server
	sf::Packet accessPacket, response;
	Account::AccessLevel access;
	std::string requestProtocol = "CONFERENCE_ACCESS";
	accessPacket << requestProtocol << this->username << this->conference;
	socket.send(accessPacket);
	socket.receive(response);
	response >> access;
	this->level = access;
}

void UserController::loginAccount()
{
	sf::Packet request, response;
	bool valid = false;
	
	std::string tmpUser, tmpPass;
	std::string protocol = "LOGIN";
	request << protocol;
	
	while(valid==false){
		std::cout<<"Username: ";
		std::cin>>tmpUser;
		std::cin.ignore(1, '\n');
		
		std::cout<<"Password: ";
		std::cin>>tmpPass;
		std::cin.ignore(1, '\n');
	
		request << tmpUser << tmpPass;
		socket.send(request);
		socket.receive(response);
		response >> valid;
		if (valid == false)
		{
			std::cout << "Invalid details" << std::endl;
		}
		request.clear();
		request << protocol;
	}
	this->username = tmpUser;
	std::cout << "You have logged in." << std::endl;
}
void UserController::registerAccount()
{
	sf::Packet request, response;
	bool exists = true;
	std::string protocol = "REGISTER";	
	request << protocol;
	std::string tmpUser, tmpPass;
	std::string firstname, lastname;
	std::string email, uni;
	std::vector<std::string> keywords;
	
	std::string tmp;
	
	while(exists == true)
	{
		std::cout << "Username: ";
		std::cin >> tmpUser;
		std::cin.ignore(1, '\n');
		
		std::cout << "Password: ";
		std::cin >> tmpPass;
		std::cin.ignore(1, '\n');
		
		std::cout << "Firstname: ";
		std::cin >> firstname;
		std::cin.ignore(1, '\n');
		
		std::cout << "Lastname: ";
		std::cin >> lastname;
		std::cin.ignore(1, '\n');
		
		std::cout << "Email: ";
		std::cin >> email;
		std::cin.ignore(1, '\n');
		
		std::cout << "Uni: ";
		std::cin >> uni;
		std::cin.ignore(1, '\n');
		
		std::cout << "Keywords (when finished, type 'stop'): " << std::endl;
		while(std::getline(std::cin, tmp), tmp != "stop")
		{
			keywords.push_back(tmp);
		}
		
		request << tmpUser << tmpPass;
		request << firstname << lastname;
		request << email << uni;
		request << (int) keywords.size();
		for (int i = 0; i < (int)keywords.size(); ++i)
		{
			request << keywords[i];
		}
		
		socket.send(request);
		socket.receive(response);
		response >> exists;
		
		if (exists)
		{
			std::cout << "That account already exists" << std::endl;
			request.clear();
			request << protocol;
		}
		keywords.clear();
	}
	this->username = tmpUser;
	std::cout << "You have been registered" << std::endl;
}

void UserController::fillMainMenu(std::vector<std::string> &list)
{
	std::string options [] = {
		"Manage Account",
		"Check Notifications",
		"Log out"
	};
	std::string accessOptions [] = {
		"Manage Submissions", 			// 1 (#0) - author 
		"Manage Reviews",				// 2 (#1) - reviewer
		"Configuration"					// 3 (#2) - chairman
	};
	list.push_back(options[0]);
	list.push_back(options[1]);
	int end = level;
	if (end > 3) { end = 3; }
	for (int i = 0; i < end; ++i)
	{
		list.push_back(accessOptions[i]);
	}
	list.push_back(options[2]);
}

void UserController::mainMenu()
{
	if (conference.length() <= 0) { return; }
	std::vector<std::string> fullOptions;
	fillMainMenu(fullOptions);
	Menu accessMenu;
	accessMenu.setOptions("Main Menu", &fullOptions[0], fullOptions.size());
	int option = 0;
	do
	{
		option = accessMenu.doMenu();
		if (option != -1 && option != (int)fullOptions.size())
		{
			if (fullOptions[option].find("Account") != std::string::npos)
			{
				this->account();
			}
			else if (fullOptions[option].find("Notification") != std::string::npos)
			{
				this->notifications();
			}
			else if (fullOptions[option].find("Submission") != std::string::npos)
			{
				this->submissions();
			}
			else if (fullOptions[option].find("Review") != std::string::npos)
			{
				this->reviews();
			}
			else if (fullOptions[option].find("Config") != std::string::npos)
			{
				this->configuration();
			}
		}
	} while (accessMenu.notExited(option));
}

/*
void UserController::createConference()
{
    std::cout << "Main Menu > Create Conference" << std::endl << std::endl;
    Conference conference;
    std::string buffer;
    
    std::cout << "Enter conference name: ";
    getline(std::cin, buffer);
    conference.setName(buffer);
    
    std::cout << "Enter conference date: ";
    getline(std::cin, buffer);
    conference.setDate(buffer);
    
    std::cout << "Enter conference location: ";
    getline(std::cin, buffer);
    conference.setLocation(buffer);
    
    conference.Phase phase = Phase_Submission;
    conference.setCurrentPhase(phase);
    
    std::cout << "Enter chairmans name: ";
    getline(std::cin, buffer);
    conference.setChairman(buffer);
    
    std::cout << "Do you want to add subchairs now? (y/n): ";
    getline(std::cin, buffer);
    if(buffer[0] == 'y')
    {        
        do
        {
            std::cout << "Enter subchair name: ";
            getline(std::cin, buffer);
            conference.addSubchair(buffer);
            
            std::cout << "Do you want to add another subchair? (y/n): ";
            getline(std::cin, buffer);
            
        }while(buffer[0] != '0');
    }
    
    std::cout << "Do you want to add reviewers now? (y/n): ";
    std::cin >> buffer;
    if(buffer[0] '0')
    {        
        do
        {
            std::cout << "Enter reviewer name: ";
            getline(std::cin, buffer);
            conference.addReviewer(buffer);
            
            std::cout << "Do you want to add another reviewer? (y/n): ";
            getline(std::cin, buffer);
            
        }while(buffer[0] == '0');
    }
    // clear screen
    std::cout << "\033[2J";  
}
*/

void UserController::account()
{
	Menu accountMenu;
	std::string accountMenuOptions[] = {
		"Change username",
		"Change password",
		"Back"
	};
	accountMenu.setOptions("Main Menu > Account", accountMenuOptions, 3);
	int option;
    do
    {
    	option = accountMenu.doMenu();
    	switch(option)
    	{
    		// changing username
    		case 0:
    			// to do: add changing username functionality
		        // clear screen
		        std::cout << "\033[2J";
    		break;
    		// changing password
    			// to do: add changing password functionality
    		case 1:
		        // clear screen
		        std::cout << "\033[2J";
    		break;
    	}
    } while (accountMenu.notExited(option));
}

void UserController::submissions()
{
	// set up menu for this part
	Menu submissionMenu;
	std::string subMenuOptions [] = {
		"Submit a paper",
		"View submissions",
		"Back"
	};
	submissionMenu.setOptions("Main Menu > Submissions", subMenuOptions, 3);
  
    int option = 0;
    while (submissionMenu.notExited(option))
    {
    	// employ the menu and get selected option
    	option = submissionMenu.doMenu();
       	switch(option)
       	{
		    // Submit a paper
		   	case 0:
		   	{
		   		Submission sub;
		        sub.submit();
            /*
                pack submission and send
                sf::Packet submitPacket;
                submitPacket << sub;
            */                
		        // clear screen
		        std::cout << "\033[2J";
		    }
		    break;
		    
            // View submissions
		    case 1:      
		    /*
                //Withdraw a paper
		        std::cout << "Which submission would you like to withdraw?" << std::endl;     
		        std::string whichSub;
		        getline(std::cin, whichSub);    
		    */
                // clear screen
		        std::cout << "\033[2J";
            break;
		    
            // Back
            case 2:
            break;
        }
    }
}

void UserController::reviews()
{
	Menu reviewMenu;
	std::string reviewMenuOptions [2] = {
		"Review a paper",
		"Back"
	};
	reviewMenu.setOptions("Main Menu > Reviews", reviewMenuOptions, 2);
    
    // Need database code for reviews
    // Need to create review form 
    int option;
    do
    {
    	option = reviewMenu.doMenu();
    	// put code to handle choices here
    } while(reviewMenu.notExited(option));
}

void UserController::configuration()
{
    std::cout << "Main Menu > Configuration" << std::endl << std::endl;
    
    Menu configurationMenu;
	std::string configurationMenuOptions[] = {
		"Advance to next phase",
		"Add reviewers",
        "Change papers per reviewer limit",
        "Back"
	};
	configurationMenu.setOptions("Main Menu > Configuration", configurationMenuOptions, 4);
	int option;
    do
    {
    	option = configurationMenu.doMenu();
    	switch(option)
    	{
    		case 0:
                
                break;
    		case 1:
                break;
            case 2:
                break;
            case 3:
                break;
    	}
    } while (configurationMenu.notExited(option));
}

void UserController::discussion()
{
    std::cout << "Main Menu > Discussions" << std::endl << std::endl;

        // send request to server to view submissions
        // print list of submissions
        // select submission
        // print comments about submission
        // enter comment
        // pack comment and send to server to add to comments
}

void UserController::notifications()
{
    std::cout << "Main Menu > Notifications" << std::endl << std::endl;
    // send query to server to check if papers have been reviewed and print to screen
}

void UserController::viewLogs()
{
    std::cout << "Main Menu > View Logs" << std::endl << std::endl;
    
    // Function call from Server Controller to view logs from database
}

void UserController::adminTools()
{
    std::cout << "Main Menu > Admin Tools" << std::endl << std::endl;

    // Need to ask team what goes in here, i think its the same as configuration
}

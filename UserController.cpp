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

UserController::UserController()
{
    
}

bool UserController::connect()
{
	sf::Time time = sf::seconds(3);
	
	std::string serverAddress;
	std::cout << "Enter server address: ";
	getline(std::cin, serverAddress);
	
	std::cout << "Connecting..." << std::endl;
	sf::Socket::Status status = socket.connect(serverAddress.c_str(), NETWORK_PORT, time);
	Menu::eraseLine("Connecting...");
	
	return (status == sf::Socket::Done);
}
void UserController::run()
{
	if (!this->connect())
	{
		std::cout << "Could not connect to server." << std::endl;
		return;
	}
	Menu::clearDisplay();
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
	start.setSelectColour(text::Colour_Blue);
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
			this->getPhase();
			this->getAdminStatus();
			if (this->pickConference())
			{
				this->mainMenu();
			}
		}
	} while (start.notExited(option));
}

void UserController::getAdminStatus()
{
	sf::Packet request, response;
	std::string protocol = "ADMIN_STATUS";
	request << protocol << username;
	
	socket.send(request);
	socket.receive(response);
	response >> isAdmin;
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
	if (isAdmin)	// admins can create conferences 
	{
		conferences.push_back("Admin: Create Conference");
	}
	conferences.push_back("Log out");
	if (conferenceSize == 0 && !isAdmin) 
	{
		std::cout << "No conferences!";
		std::cin.ignore(1, '\n');
		Menu::eraseLine("No conferences!");
		logOut();
		return false;
	}
	int pick = 0;
	int createOption = (conferences.size()-2);
	int lastOption = (conferences.size()-1);
	Menu pickMenu;
	pickMenu.setOptions("Pick a conference:", &conferences[0], conferences.size());
	pickMenu.setSelectColour(text::Colour_Cyan);
	//pickMenu.disableBackButton();
	bool created = false;
	
	do
	{
		// if the user picked to create conference but then cancelled, they should end up back here
		pick = pickMenu.doMenu();
		if (isAdmin && pick == createOption)
		{
			created = this->createConference();
			if (created)
			{
				return created;		
			}
		}
		// user pressed back button or wants to log out
		else if (pick == -1 || pick == lastOption)
		{
			return false;
		}
	} while (isAdmin && pick == createOption);
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
		
		Menu::eraseLine(tmpPass + "Password: ");
		Menu::eraseLine(tmpUser + "Username: ");
	
		request << tmpUser << tmpPass;
		socket.send(request);
		socket.receive(response);
		response >> valid;
		if (valid == false)
		{
			std::cout << "Invalid details";
			std::cin.ignore(1, '\n');
			Menu::eraseLine("Invalid details");
		}
		request.clear();
		request << protocol;
	}
	this->username = tmpUser;
	std::cout << "You have logged in." << std::endl;
	Menu::eraseLine("You have logged in.");
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
		Menu::eraseLine("Username: " + tmpUser);
		
		std::cout << "Password: ";
		std::cin >> tmpPass;
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Password: " + tmpPass);
		
		std::cout << "Firstname: ";
		std::cin >> firstname;
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Firstname: " + firstname);
		
		std::cout << "Lastname: ";
		std::cin >> lastname;
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Lastname: " + lastname);
		
		std::cout << "Email: ";
		std::cin >> email;
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Email: " + email);
		
		std::cout << "Univerisity: ";
		std::cin >> uni;
		std::cin.ignore(1, '\n');
		Menu::eraseLine("University: " + uni);
		
		std::cout << "Keywords (when finished, type 'stop'): " << std::endl;
		while(std::getline(std::cin, tmp), tmp != "stop")
		{
			keywords.push_back(tmp);
		}
		Menu::eraseLine("stop");
		for (int i = (int)(keywords.size()-1); i >= 0; i--)
		{
			Menu::eraseLine(keywords[i]);
		} 
		Menu::eraseLine("Keywords (when finished, type 'stop'): ");
		
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
			std::cout << "That account already exists";
			std::cin.ignore(1, '\n');
			request.clear();
			request << protocol;
			Menu::eraseLine("That account already exists");
		}
		keywords.clear();
	}
	this->username = tmpUser;
	std::cout << "You have been registered" << std::endl;
	Menu::eraseLine("You have been registered");
}

void UserController::fillMainMenu(std::vector<std::string> &list)
{
	// setup main menu options according to user's access level in conference
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
	std::ostringstream menuTitle;
	menuTitle << "Main Menu - " << conference;
	Menu accessMenu;
	accessMenu.setOptions(menuTitle.str(), &fullOptions[0], fullOptions.size());
	int option = 0;
	do
	{
		option = accessMenu.doMenu();
		if (option != -1 && option != (int)(fullOptions.size() - 1))
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
	this->logOut();
}

void UserController::logOut()
{
	sf::Packet request;
	std::string protocol = "BYE";
	request << protocol << username;
	socket.send(request);
}

bool UserController::createConference()
{
	bool created = false;
	std::string formOptions[] = {
		"Name: ",
		"Date: ",
		"Location: ",
		"Chairman: ",
		"Set Subchairs",
		"Set Reviewers",
		"Create this conference",
		"Cancel"
	};
	Conference tmpConf;
	Menu conferenceForm;
	conferenceForm.setOptions("Create a Conference", formOptions, 8);
	conferenceForm.setPaged();
	conferenceForm.setVisibleNum(4);
	int option;
	std::string buffer;
	do
	{
		option = conferenceForm.doMenu();
		switch(option)
		{
			// enter name
			case 0:
				std::cout << "Enter conference name: ";
				getline(std::cin, buffer);
				Menu::eraseLine(buffer + "Enter conference name: ");
				formOptions[0] = "Name: " + buffer;
				tmpConf.setName(buffer);
			break;
			// enter date
			case 1:
				std::cout << "Enter conference date: ";
				getline(std::cin, buffer);
				Menu::eraseLine(buffer + "Enter conference date: ");
				tmpConf.setDate(buffer);
				formOptions[1] = "Date: " + buffer;
			break;
			// enter location
			case 2:
				std::cout << "Enter conference location: ";
				getline(std::cin, buffer);
				Menu::eraseLine(buffer + "Enter conference location: ");
				tmpConf.setLocation(buffer);
				formOptions[2] = "Location: " + buffer;
			break;
			// enter chairman
			case 3:
				std::cout << "Enter chairmans name: ";
				getline(std::cin, buffer);
				Menu::eraseLine(buffer + "Enter chairmans name: ");
				tmpConf.setChairman(buffer);
				formOptions[3] = "Chairman: " + buffer;
			break;
			// enter subchairs
			case 4:
				do
				{
					std::cout << "Enter subchair name: ";
					getline(std::cin, buffer);
					tmpConf.addSubchair(buffer);
					Menu::eraseLine(buffer + "Enter subchair name: ");
				
					std::cout << "Do you want to add another subchair? (y/n): ";
					getline(std::cin, buffer);
					Menu::eraseLine(buffer + "Do you want to add another subchair? (y/n): ");
				}while(buffer[0] == 'y');
			break;
			// enter reviewers
			case 5:
				do
				{
				    std::cout << "Enter reviewer name: ";
				    getline(std::cin, buffer);
				    tmpConf.addReviewer(buffer);
				    Menu::eraseLine(buffer + "Enter reviewer name: ");
				    
				    std::cout << "Do you want to add another reviewer? (y/n): ";
				    getline(std::cin, buffer);
			  		Menu::eraseLine(buffer + "Do you want to add another reviewer? (y/n): ");
				}while(buffer[0] == 'y');
			break;
			// submit this conference to server
			case 6:
				if (tmpConf.getName().length() == 0)
				{
					std::cout << "Error: Conferences require a name.";
					std::cin.ignore(1, '\n');
					Menu::eraseLine("Error: Conferences require a name.");
				}
				else if(sendCreatedConference(tmpConf))
				{
					option = -1;
					created = true;
					conferenceForm.clear();
				}
			break;
			// cancel conference creation
			case 7:
				created = false;
			break;
		}
	} while (conferenceForm.notExited(option));
	return created;
}

bool UserController::sendCreatedConference(const Conference &conf)
{
	sf::Packet request, response;
	std::string protocol = "CREATE_CONFERENCE";
	bool exists = false;
	response.clear();
	request.clear();
	request << protocol << username << conf;
	socket.send(request);
	socket.receive(response);
	response >> exists;
	if (!exists)
	{
		std::cout << "Conference created!";
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Conference created!");
		conference = conf.getName();
		// an admin made the conference, so they have admin access
		level = Account::Access_Admin;		
	}
	else
	{
		std::cout << "Error: Conference already exists";
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Error: Conference already exists");
	}
	return !exists;
}

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
    			this->changeUsername();
    		break;
    		// changing password
    		case 1:
		        this->changePassword();
    		break;
    	}
    } while (accountMenu.notExited(option));
}

void UserController::changeUsername()
{
	std::cout << "Change username: ";
	std::string newUser;
	getline(std::cin, newUser);
	Menu::eraseLine(newUser + "Change username: ");
}
void UserController::changePassword()
{
	std::cout << "Change password: ";
	std::string newPass;
	getline(std::cin, newPass);
	Menu::eraseLine(newPass + "Change password: ");
}

void UserController::submitPaper()
{
	sf::Packet request, response;
	std::string protocol = "SUBMIT_PAPER";
	bool exists;
	
	request << protocol << username;
	Submission sub;
	sub.submit();
	sub.setConference(conference);
	request << sub;
	
	socket.send(request);
	socket.receive(response);
	response >> exists;
	if (exists)
	{
		std::cout << "Error: That submission exists already.";
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Error: That submission exists already.");
	}
}
void UserController::viewSubmissions()
{
	sf::Packet request, response;
	std::string protocol = "VIEW_SUBMISSIONS";
	request << protocol << username;
	
	socket.send(request);			// send a request for a list of submissions
	socket.receive(response);		// get response
	
	std::vector<std::string> subList;
	int subCount;
	response >> subCount;
	if (subCount > 0)
	{
		for (int i = 0; i < subCount; ++i)
		{
			std::string tmpSub;
			response >> tmpSub;
			subList.push_back(tmpSub);
		}
		subList.push_back("Back");
	
		Menu viewSubMenu;
		viewSubMenu.setOptions("Submissions > View Submissions", &subList[0], subList.size());
		int option = 0; 
		do
		{
			option = viewSubMenu.doMenu();
			if (option != (int)(subList.size()-1) && option != -1)
			{
				std::cout << "Viewing sub: " << subList[option];
				std::cin.ignore(1, '\n');
				viewSubMenu.eraseLine(subList[option] + "Viewing sub: ");
			}
		} while (viewSubMenu.notExited(option));
	}
	else
	{
		std::cout << "No submissions!";
		std::cin.ignore(1, '\n');
		Menu::eraseLine("No submissions!");
	}
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
		   		this->submitPaper(); 
		    }
		    break;
		    
            // View submissions
		    case 1:      
		    	this->viewSubmissions();
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
	std::string reviewMenuOptions [] = {
		"Bid for a paper",			// in bidding phase only
		"Submit a review",			// in review phase only
		"Discuss a paper",			// in discussion phase (unlocked), finalisation-completion (locked, viewable)
		"Back"
	};
	std::vector<std::string> optionList;
	
	if (phase == "Allocation")
	{
		optionList.push_back(reviewMenuOptions[0]);
	}
	else if (phase == "Reviewing")
	{
		optionList.push_back(reviewMenuOptions[1]);
	}
	else if (phase == "Discussing")
	{
		optionList.push_back(reviewMenuOptions[2]);
	}
	optionList.push_back(reviewMenuOptions[3]);
	reviewMenu.setOptions("Main Menu > Reviews", reviewMenuOptions, optionList.size());
    int option;
    do
    {
    	option = reviewMenu.doMenu();
    	// as long as user hasn't tried to exit this menu
    	if (option != -1 && option != (int)(optionList.size()-1))
    	{
    		// interpret choice
    		if (optionList[option].find("Bid") != std::string::npos)
			{
				this->bidPaper();
			}
			else if (optionList[option].find("Submit") != std::string::npos)
			{
				this->submitReview();
			}
			else if (optionList[option].find("Discuss") != std::string::npos)
			{
				this->discussion();
			}
    	}
    } while(reviewMenu.notExited(option));
}

void UserController::getBidList(std::vector<std::string> &list)
{
	sf::Packet request, response;
	std::string protocol = "BID_LIST";
	request << protocol << username << conference;
	
	socket.send(request);
	socket.receive(response);
	
	int responseSize = 0;
	response >> responseSize;
	for (int i = 0; i < responseSize; ++i)
	{
		std::string tmpSub;
		response >> tmpSub;
		list.push_back(tmpSub);
	}
}

void UserController::bidPaper()
{
	// display a list of biddable submissions
	std::vector<std::string> subList;
	getBidList(subList);
	subList.push_back("Back");

	Menu bidMenu;
	bidMenu.setOptions("Reviews > Bid", &subList[0], subList.size());
	int pick = 0, bidIndex = -1;
	do 
	{
		pick = bidMenu.doMenu();
		// as long as the user isn't exiting the menu
		if (pick != -1 && pick != (int)(subList.size()-1))
		{
			if (confirmMenu("Bid for this paper? (Final decision)"))
			{
				bidIndex = pick;
				pick = -1;
			}
		}
	} while (bidMenu.notExited(pick));

	if (bidIndex != -1)
	{
		sf::Packet request;
		std::string protocol = "BID_PAPER";
		request << protocol << username << conference << subList[bidIndex];
		socket.send(request);
	}
}
void UserController::submitReview()
{
	sf::Packet request;
	// FILL IN CODE
}

bool UserController::confirmMenu(const std::string &msg)
{
	static std::string choices[2] = {
		"Yes",
		"No"
	};
	Menu confirm;
	confirm.setOptions(msg, choices, 2);
	int choice = confirm.doMenu();
	return (choice == 0);
}

void UserController::configuration()
{
    Menu configurationMenu;
	std::string configurationMenuOptions[] = {
		"Current Phase:\n  " + phase,
		"Advance to next phase",
		"Add reviewers",
        "Change papers per reviewer limit",
        "Change reviewers per paper limit",
        "Back"
	};
	// alter the first option to display the current phase
	configurationMenu.setOptions("Main Menu > Configuration", configurationMenuOptions, 6);
	int option;
    do
    {
    	option = configurationMenu.doMenu();
    	switch(option)
    	{
    		// advance to next phase
    		case 1:
    			// confirm choice with user
    			if (confirmMenu("Are you sure?"))
    			{
    				this->advancePhase();
    				// get next phase by request
					this->getPhase();
					configurationMenuOptions[0] = "Current Phase:\n  " + phase;
    			}
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
            	break;
            case 5:
            	break;
    	}
    } while (configurationMenu.notExited(option));
}

void UserController::getPhase()
{
	sf::Packet request, response;
	std::string protocol = "CHECK_PHASE";
	request << protocol << conference;
	socket.send(request);
	socket.receive(response);
	response >> phase;
}

void UserController::advancePhase()
{
	sf::Packet request;
	std::string protocol = "ADVANCE_PHASE";
	request << protocol << username << conference;
	socket.send(request);
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
	// send query to server to check if papers have been reviewed and print to screen
	sf::Packet request, response;
	std::string protocol = "GET_NOTIFICATIONS";
	request << protocol << username;
	
	socket.send(request);
	socket.receive(response);
	
	std::vector<std::string> notifyList;
	int count = 0;
	response >> count;
	for (int i = 0; i < count; ++i)
	{
		std::string event;
		response >> event;
		notifyList.push_back(event);
	}
	notifyList.push_back("Clear and go back");
	
	Menu notifyMenu;
	notifyMenu.setOptions("Main Menu > Notifications", &notifyList[0], notifyList.size());
	notifyMenu.setScrolling();
	notifyMenu.setVisibleNum(5);
	int option;
	do
	{
		option = notifyMenu.doMenu();
	} while(notifyMenu.notExited(option));
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

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
	sf::Time time = sf::seconds(0);
	
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
	if (conferenceSize == 0 && !isAdmin) 
	{
		std::cout << "No conferences!";
		std::cin.ignore(1, '\n');
		Menu::eraseLine("No conferences!");
		
		return false;
	}
	int pick = 0;
	Menu pickMenu;
	pickMenu.setOptions("Pick a conference:", &conferences[0], conferences.size());
	pickMenu.disableBackButton();
	pick = pickMenu.doMenu();
	
	if (isAdmin && pick == (int)(conferences.size()-1))
	{
		this->createConference();
		return true;
	}
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
	this->logOut();
}

void UserController::logOut()
{
	sf::Packet request;
	std::string protocol = "BYE";
	request << protocol << username;
	socket.send(request);
}

void UserController::createConference()
{
	sf::Packet request, response;
	std::string protocol = "CREATE_CONFERENCE";
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
			{
				bool exists = false;
				response.clear();
				request.clear();
				request << protocol << username << tmpConf;
				socket.send(request);
				socket.receive(response);
				response >> exists;
				if (!exists)
				{
					std::cout << "Conference created!";
					std::cin.ignore(1, '\n');
					Menu::eraseLine("Conference created!");
					conference = tmpConf.getName();
					// an admin made the conference, so they have admin access
					level = Account::Access_Admin;		
					option = -1;
					conferenceForm.clear();
				}
				else
				{
					std::cout << "Error: Conference already exists";
					std::cin.ignore(1, '\n');
					Menu::eraseLine("Error: Conference already exists");
				}
			}
			break;
		}
	} while (conferenceForm.notExited(option));
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
	std::string outcome;
	
	request << protocol << username;
	Submission sub;
	sub.submit();
	request << sub;
	
	socket.send(request);
	//socket.receive(response);
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
		"Discuss a paper"			// in discussion phase (unlocked), finalisation-completion (locked, viewable)
		"Back"
	};
	reviewMenu.setOptions("Main Menu > Reviews", reviewMenuOptions, 4);
    
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
        "Change reviewers per paper limit",
        "Back"
	};
	configurationMenu.setOptions("Main Menu > Configuration", configurationMenuOptions, 5);
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
            case 4:
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

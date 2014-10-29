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
	bool loggedIn = false;
	do
	{
		option = start.doMenu();
		switch(option)
		{
			// Login in
			case 0: loggedIn = this->loginAccount();
			break;
			// Register
			case 1: this->registerAccount();
			break;
			// Exit
			case 2: case -1:
			break;
		}
		if (option == 1 || (option == 0 && loggedIn))
		{
			this->getAdminStatus();
			if (this->pickConference())
			{
				this->getPhase();
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

bool UserController::loginAccount()
{
	sf::Packet request, response;
	bool valid = false;
	int invalidCount = 0;
	
	std::string tmpUser, tmpPass;
	std::string protocol = "LOGIN";
	request << protocol;
	
	while(valid==false && invalidCount < MAX_INVALID_LOGINS){
		std::cout<<"Username: ";
		std::cin>>tmpUser;
		std::cin.ignore(1, '\n');
		
		std::cout<<"Password: ";
		std::cin>>tmpPass;
		std::cin.ignore(1, '\n');
		
		Menu::eraseLine(tmpPass + "Password: ");
		Menu::eraseLine(tmpUser + "Username: ");
	
		request << tmpUser << encrypt(tmpPass);
		socket.send(request);
		socket.receive(response);
		response >> valid;
		if (valid == false)
		{
			std::cout << "Invalid details";
			std::cin.ignore(1, '\n');
			Menu::eraseLine("Invalid details");
			invalidCount++;
		}
		request.clear();
		request << protocol;
	}
	if (valid)
	{
		this->username = tmpUser;
		std::cout << "You have logged in." << std::endl;
		Menu::eraseLine("You have logged in.");
		return true;
	}
	std::cout << "Maximum number of invalid logins reached.";
	std::cin.ignore(1, '\n');
	Menu::eraseLine("Maximum number of invalid logins reached.");
	return false;
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
		
		std::cout << "University: ";
		std::cin >> uni;
		std::cin.ignore(1, '\n');
		Menu::eraseLine("University: " + uni);
		
		std::cout << "Enter keywords (when finished, type 'stop'): " << std::endl;
		while(std::getline(std::cin, tmp), tmp != "stop")
		{
			keywords.push_back(tmp);
		}
		Menu::eraseLine("stop");
		for (int i = (int)(keywords.size()-1); i >= 0; i--)
		{
			Menu::eraseLine(keywords[i]);
		} 
		Menu::eraseLine("Enter keywords (when finished, type 'stop'): ");
		
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
		"Manage Submissions", 				// 1 (#0) - author 
		"Manage Reviews",					// 2 (#1) - reviewer
		"Finalise Papers",					// 3 (#2) - chairman
		"Configuration"						// 4 (#3) - chairman / admin
	};
	list.push_back(options[0]);
	list.push_back(options[1]);
	int end = level;
	if (end >= 3) { end = 4; }
	for (int i = 0; i < end; ++i)
	{
		list.push_back(accessOptions[i]);
	}
	list.push_back(options[2]);
}

void UserController::updateMainMenuMarquee(std::ostringstream &msg)
{
	msg.str("");
	int numNotify = getNotifyCount();
	if (numNotify > 0)
	{
		msg << "You have " << numNotify << " unread notifications";
	}
	else
	{
		msg << "No new notifications";
	}
}

void UserController::mainMenu()
{
	if (conference.length() <= 0) { return; }
	
	std::string menuDesc[7] = {
		"Manage your account details.", 
		"Manage notifications.", 
		"Manage paper submissions.",
		"Manage reviews for assigned papers.",
		"Finalise papers for the conference.",
		"Configure conference options.",
		"Log out of the system."
	};
	if (level > 0 && level < 3) { menuDesc[2+level] = "Log out of the system."; }
	
	std::vector<std::string> fullOptions;
	fillMainMenu(fullOptions);
	std::ostringstream menuTitle, marqueeText;
	menuTitle << "Main Menu - " << conference << "\nWelcome " << username;
	updateMainMenuMarquee(marqueeText);
	
	Menu accessMenu;
	accessMenu.setMarquee(marqueeText.str(), 0.3);
	accessMenu.setOptions(menuTitle.str(), &fullOptions[0], fullOptions.size());
	accessMenu.setDescriptions(menuDesc);
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
			else if (fullOptions[option].find("Final") != std::string::npos)
			{
				this->finalise();
			}
			else if (fullOptions[option].find("Config") != std::string::npos)
			{
				this->configuration();
			}
		}
		// recheck notification count
		updateMainMenuMarquee(marqueeText);
		accessMenu.setMarquee(marqueeText.str(), 0.3);
	} while (accessMenu.notExited(option));
	this->logOut();
}

int UserController::getNotifyCount()
{
	sf::Packet request, response;
	std::string protocol = "NOTIFY_COUNT";
	request << protocol << username;
	sf::Int16 recVal = 0;
	socket.send(request);
	socket.receive(response);
	response >> recVal;
	return recVal;
}

void UserController::finalise()
{
	if (phase != "Finalising")
	{
		std::cout << "Error: The conference is not in the finalising stage.";
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Error: The conference is not in the finalising stage.");
		return;
	}

	sf::Packet request, response;
	std::vector<std::string> subs;
	std::string protocol = "CONFERENCE_SUBMISSIONS";
	request << protocol << conference;
	
	socket.send(request);
	socket.receive(response);
	int listSize = 0;
	response >> listSize;
	
	for (int i = 0; i < listSize; ++i)
	{
		std::string tmp;
		response >> tmp;
		subs.push_back(tmp);
	}
	subs.push_back("Back");
	
	Menu finaliseMenu;
	finaliseMenu.setOptions("Main Menu > Finalise submissions", &subs[0], subs.size());
	finaliseMenu.setPaged();
	finaliseMenu.setVisibleNum(5);
	int option, backOption = (int)(subs.size()-1);
	do
	{
		option = finaliseMenu.doMenu();
		if (option != -1 && option != backOption)
		{
			prepareFinalReview(subs[option]);	
		}
	} while (finaliseMenu.notExited(option));
}

void UserController::prepareFinalReview(const std::string &paper)
{
	sf::Packet request, response;
	std::string protocol = "REVIEW_LIST";
	request << protocol << conference << paper;
	
	socket.send(request);
	socket.receive(response);
	
	// request a list of reviews filled with review ids
	std::vector<std::string> list;
	int size;
	response >> size;
	for (int i = 0; i < size; ++i)
	{
		std::string tmp;
		response >> tmp;
		list.push_back(tmp);
	}
	list.push_back("Create final review");
	list.push_back("Approve / Reject submission");
	list.push_back("Back");
	
	Menu detailReviewMenu;
	detailReviewMenu.setOptions("Finalise submissions > Finalise Review", &list[0], list.size());
	detailReviewMenu.setPaged();
	detailReviewMenu.setVisibleNum(5);
	int option, backOption = (int)(list.size()-1), createOption = (int)(list.size()-3);
	int approveOption = (int)(list.size()-2);
	do
	{
		option = detailReviewMenu.doMenu();
		// create final review for this submission
		if (option == createOption)
		{
			sf::Packet request;
			std::string protocol = "SUBMIT_REVIEW";
			Review tmpReview;
			tmpReview.setFinal();
			tmpReview.setTitle(paper);
			tmpReview.setConference(conference);
			// use a form to enter in a review about this paper
			if (createReviewForm(tmpReview))
			{
				request << protocol << username << conference << paper;
				request << tmpReview;
			
				socket.send(request);
				option = -1;
			}
		}
		// approve or reject this paper
		else if (option == approveOption)
		{
			sf::Packet request;
			std::string protocol = "APPROVE_PAPER";
			if (!confirmMenu("Do you want to approve this paper? \n(Answering No will reject the paper)"))
			{
				protocol = "REJECT_PAPER";
			}
			request << protocol << username << conference << paper;
			socket.send(request);
		}
		// view the details of each non final review
		else if (option != -1 && option != backOption)
		{
			viewReview(list[option]);
		}
	} while (detailReviewMenu.notExited(option));
}

void UserController::viewReview(const std::string &id)
{
	sf::Packet request, response;
	std::string protocol = "VIEW_REVIEW";
	bool found;
	request << protocol << username << conference << id;
	
	socket.send(request);
	socket.receive(response);
	Review tmpReview;
	response >> found;
	if (found)
	{
		response >> tmpReview;
		std::vector<std::string> reviewDetails;
		std::ostringstream os;
		// manually set up a display menu for a review
		reviewDetails.push_back("Conference: " + tmpReview.getConference());
		reviewDetails.push_back("Paper: " + tmpReview.getTitle());
		reviewDetails.push_back("PC Member: " + tmpReview.getPCMember());
		reviewDetails.push_back("Strengths:\n   " + tmpReview.getStrengths());
		reviewDetails.push_back("Weaknesses:\n    " + tmpReview.getWeaknesses());
		reviewDetails.push_back("Suggestions:\n    " + tmpReview.getSuggestions());
		reviewDetails.push_back("Remark for short paper listing:\n    " + tmpReview.getShortPaper());
		reviewDetails.push_back("Remark for best paper award:\n    " + tmpReview.getBestPaper());
		reviewDetails.push_back("Remarks for PC members:\n    " + tmpReview.getRemarks());
		if (tmpReview.getReviewerFirst().length() > 0)
		{
			reviewDetails.push_back("Sub reviewer name: " + tmpReview.getReviewerFirst() + " " + 
				tmpReview.getReviewerLast() + "\n  Sub reviewer email: " + tmpReview.getReviewerEmail());
		}
		os << "Overall evaluation: " << tmpReview.getOverallEvaluation();
		reviewDetails.push_back(os.str());
		os.str("");
		os << "Reviewer confidence: " << tmpReview.getReviewerConfidence();
		reviewDetails.push_back(os.str());
		os.str("");
		os << "Relevance: " << tmpReview.getRelevance();
		reviewDetails.push_back(os.str());
		os.str("");
		os << "Originality: " << tmpReview.getOriginality();
		reviewDetails.push_back(os.str());
		os.str("");
		os << "Significance: " << tmpReview.getSignificance();
		reviewDetails.push_back(os.str());
		os.str("");
		os << "Presentation: " << tmpReview.getPresentation();
		reviewDetails.push_back(os.str());
		os.str("");
		os << "Technical Quality: " << tmpReview.getTechnicalQuality();
		reviewDetails.push_back(os.str());
		os.str("");
		os << "Evaluation: " << tmpReview.getEvaluation();
		reviewDetails.push_back(os.str());
		reviewDetails.push_back("Back");
		
		Menu display;
		os.str("");
		os << "Viewing review id: " << tmpReview.getReviewID();
		if (tmpReview.getFinal())
		{
			os << " (Final Review)";
		}
		display.setOptions(os.str(), &reviewDetails[0], reviewDetails.size());
		display.setPaged();
		display.setVisibleNum(5);
		int option;
		do
		{
			option = display.doMenu();
		} while (display.notExited(option));
		
	}
}

bool UserController::getAccountName(const std::string &target, std::string &first, std::string &last)
{
	sf::Packet request, response;
	bool exists;
	std::string protocol = "GET_FULLNAME";
	request << protocol << username << target;
	socket.send(request);
	socket.receive(response);
	response >> exists;
	if (exists)
	{
		response >> first >> last;
	}
	return exists;
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
				    std::cout << "Enter reviewer username: ";
				    getline(std::cin, buffer);
				    tmpConf.addReviewer(buffer);
				    Menu::eraseLine(buffer + "Enter reviewer username: ");
				    
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
	if (phase != "Submission") 
	{
		std::cout << "Error: Submissions are closed";
		std::cin.ignore(1, '\n');
		Menu::eraseLine("Error: Submissions are closed");
		return;
	}
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
				// if the conference has been completed 
				// give access to the final review for their paper
				std::string finalRevId;
				if (phase == "Completed" && getFinalReview(subList[option], finalRevId)
					&& confirmMenu("Do you want to see the final review?"))
				{
					viewReview(finalRevId);
				}
				else
				{
					detailSub(subList[option]);
				}
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

bool UserController::getFinalReview(const std::string &paper, std::string &final)
{
	sf::Packet request, response;
	bool exists;
	std::string protocol = "FINAL_REVIEW";
	request << protocol << username << conference << paper;
	socket.send(request);
	socket.receive(response);
	response >> exists;
	if (exists)
	{
		response >> final;
	}
	return exists;
}

void UserController::detailSub(const std::string &title)
{
	sf::Packet request, response;
	std::string protocol = "SUB_DETAIL";
	request << protocol << conference << title;
	
	socket.send(request);
	socket.receive(response);
	
	Submission aSub;
	response >> aSub;
	
	aSub.view();
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
		// if user is at least a chairman
		if (level >= 3)
		{
			optionList.push_back("Manage Submission Allocation");
		}
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
	reviewMenu.setOptions("Main Menu > Reviews", &optionList[0], optionList.size());
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
			else if (optionList[option].find("Allocation") != std::string::npos)
			{
				this->viewAllocations();
			}
    	}
    } while(reviewMenu.notExited(option));
}

void UserController::viewAllocations()
{
	// display a list of all reviewer usernames, where selecting them will show
	// their currently allocated papers
	sf::Packet request, response;
	std::string protocol = "CONF_REVIEWERS";
	request << protocol << username << conference;
	
	socket.send(request);
	socket.receive(response);
	
	std::vector<std::string> reviewerList;
	int reviewerListSize;
	response >> reviewerListSize;
	for (int i = 0; i < reviewerListSize; ++i)
	{
		std::string tmp;
		response >> tmp;
		reviewerList.push_back(tmp);	
	}
	reviewerList.push_back("Allocate Reviewers");
	reviewerList.push_back("Back");
	
	Menu viewAllocMenu;
	viewAllocMenu.setOptions("Reviews > Manage Allocations", &reviewerList[0], reviewerList.size());
	viewAllocMenu.setScrolling();
	viewAllocMenu.setVisibleNum(5);
	int option, allocateOption = (int)(reviewerList.size()-2), backOption = (int)(reviewerList.size()-1);
	do
	{
		option = viewAllocMenu.doMenu();
		// selected to manually allocate papers to reviewers
		if (option == allocateOption)
		{
			doAllocations();
		}
		// selected a specific reviewer to see their current allocations
		else if (option != -1 && option != backOption)
		{
			viewReviewerAllocations(reviewerList[option]);
		}
	} while (viewAllocMenu.notExited(option));
}

void UserController::getConferenceAlloc(std::vector<std::string> &unAllocList)
{
	sf::Packet request, response;
	std::string protocol = "CONF_SUBMISSIONS";
	request << protocol << conference;
	
	socket.send(request);
	socket.receive(response);
	
	int listSize = 0;
	response >> listSize;
	for (int i = 0; i < listSize; ++i)
	{
		std::string tmp;
		response >> tmp;
		unAllocList.push_back(tmp);
	}
}

void UserController::doAllocations()
{
	std::vector<std::string> paperList;
	getConferenceAlloc(paperList);
	paperList.push_back("Back");
	
	Menu allocPaperMenu;
	allocPaperMenu.setOptions("Manage Allocations > Papers to allocate", 
		&paperList[0], paperList.size());
	int option, backOption = (int)(paperList.size()-1);
	do
	{
		option = allocPaperMenu.doMenu();
		if (option != -1 && option != backOption)
		{
			if (needsAllocation(paperList[option]))
			{
				assignReviewers(paperList[option]);
			}
			else
			{
				std::cout << "Paper does not need to be allocated.";
				std::cin.ignore(1, '\n');
				Menu::eraseLine("Paper does not need to be allocated.");
			}
		}
	} while (allocPaperMenu.notExited(option));
}
void UserController::getFreeReviewers(std::vector<std::string> &list)
{
	sf::Packet request, response;
	std::string protocol = "GET_FREE_REVIEWERS";
	request << protocol << conference;
	
	socket.send(request);
	socket.receive(response);
	
	int size = 0;
	response >> size;
	for (int i = 0; i < size; ++i)
	{
		std::string tmp;
		response >> tmp;
		list.push_back(tmp);
	}
}
void UserController::assignReviewers(const std::string &paper)
{
	// get a list of reviewers that can be allocated to papers
	std::vector<std::string> freeReviewerList;
	getFreeReviewers(freeReviewerList);
	freeReviewerList.push_back("Back");
	
	Menu allocateMenu;
	allocateMenu.setOptions("Papers to allocate > Allocate reviewers", 
		&freeReviewerList[0], freeReviewerList.size());
	int option, backOption = (int)(freeReviewerList.size()-1);
	bool assigned = false;
	
	do
	{
		option = allocateMenu.doMenu();
		if (option != -1 && option != backOption &&
			confirmMenu("Allocate to this reviewer?"))
		{
			sf::Packet request;
			std::string protocol = "ASSIGN_REVIEWER";
			request << protocol << conference << paper;
			request << freeReviewerList[option];
			assigned = true;
			
			socket.send(request);
		}
	} while (!assigned && option != -1 && option != backOption);
}
bool UserController::needsAllocation(const std::string &paper)
{
	sf::Packet request, response;
	std::string protocol = "FILLED_ALLOCATION";
	bool result = false;
	request << protocol << conference << paper;
	
	socket.send(request);
	socket.receive(response);
	
	response >> result;
	return result;
}
void UserController::viewReviewerAllocations(const std::string &revUser)
{
	std::vector<std::string> allocList;
	getAllocations(allocList, revUser);
	allocList.push_back("Back");
	
	Menu displayAlloc;
	displayAlloc.setOptions("Manage Allocations > Current Allocations for " + revUser, 
		&allocList[0], allocList.size());
	displayAlloc.setPaged();
	displayAlloc.setVisibleNum(5);
	int option;
	do
	{
		option = displayAlloc.doMenu();
	} while (displayAlloc.notExited(option));
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
				sf::Packet request;
				std::string protocol = "BID_PAPER";
				request << protocol << username << conference << subList[pick];
				socket.send(request);
				pick = -1;
			}
		}
	} while (bidMenu.notExited(pick));
}
void UserController::getAllocations(std::vector<std::string> &list, const std::string &target)
{
	// send request to server to view submissions
	sf::Packet request, response;
	std::string protocol = "GET_ALLOCATIONS";
	request << protocol << target << conference;
	
	socket.send(request);
	socket.receive(response);
	
	int subSize = 0;
	list.clear();
	list.reserve(subSize);
	response >> subSize;
	for (int i = 0; i < subSize; ++i)
	{
		std::string tmpTitle;
		response >> tmpTitle;
		list.push_back(tmpTitle);
	}
}
void UserController::submitReview()
{
	// send request to server to view submissions
	sf::Packet request;
	std::string protocol = "SUBMIT_REVIEW";
	std::vector<std::string> subs;
	getAllocations(subs, username);
	subs.push_back("Cancel");

	// select submission	
	Menu submit;
	submit.setOptions("Reviews > Submit review", &subs[0], subs.size());
	int option, cancelOption = (int)(subs.size()-1);
	do
	{
		option = submit.doMenu();
		if (option != -1 && option != cancelOption)
		{
			std::string paperTitle = subs[option];
			if (hasReviewed(paperTitle) && !confirmMenu("Do you want to resubmit your review?"))
			{
				viewMyReview(paperTitle);
			}
			else
			{
				Review tmpReview;
				tmpReview.setTitle(paperTitle);
				tmpReview.setConference(conference);
				// use a form to enter in a review about this paper
				if (createReviewForm(tmpReview))
				{
					request << protocol << username << conference << paperTitle;
					request << tmpReview;
			
					socket.send(request);
				}
			}
		}
	} while (submit.notExited(option));
}

bool UserController::hasReviewed(const std::string &paper)
{
	sf::Packet request, response;
	std::string protocol = "DID_REVIEW";
	request << protocol << username << conference << paper;
	socket.send(request);
	socket.receive(response);
	bool result = false;
	response >> result;
	return result;
}

void UserController::viewMyReview(const std::string &paper)
{
	sf::Packet request, response;
	std::string protocol = "MY_REVIEWS";
	request << protocol << conference << paper << username;
	socket.send(request);
	socket.receive(response);
	
	int receiveNum = 0;
	response >> receiveNum;
	std::vector<std::string> idList;
	for (int i = 0; i < receiveNum; ++i)
	{
		std::string tmp;
		response >> tmp;
		idList.push_back(tmp);
	}
	idList.push_back("Back");
	
	Menu myReviewMenu;
	myReviewMenu.setOptions("Submit Review > View My Reviews", &idList[0], idList.size());
	int option, backOption = (int)(idList.size()-1);
	do
	{
		option = myReviewMenu.doMenu();
		if (option != -1 && option != backOption)
		{
			viewReview(idList[option]);	
		}
	} while (myReviewMenu.notExited(option));
}

bool UserController::createReviewForm(Review &rev)
{
	bool submitted = false;
	rev.generateReviewID();
	std::string fields [10] = {
		"Enter Strengths",
		"Enter Weaknesses",
		"Enter Suggestions",
		"Enter Short Paper Remark",
		"Enter Best Paper Remark",
		"Enter Remarks for PC Members",
		"Set subreviewer's details",
		"Enter evaluations",
		"Submit",
		"Cancel"
	};
	Menu form;
	form.setOptions("Fill out a review", fields, 10);
	form.setScrolling();
	form.setVisibleNum(4);
	int option = 0, submitOption = 8, cancelOption = 9;
	std::string buffer;
	int overallEval [1] = {0};
	int revConf [1] = {0};
	int scores [6] = {3, 3, 3, 3, 3, 3};
	do
	{
		option = form.doMenu();
		switch(option)
		{
			// strengths
			case 0:
			std::cout << "Enter strengths: ";
			getline(std::cin, buffer);
			Menu::eraseLine("Enter strengths: " + buffer);
			rev.setStrengths(buffer);
			fields[0] = "Enter Strengths\n    " + buffer;
			break;
			// weaknesses
			case 1:
			std::cout << "Enter weaknesses: ";
			getline(std::cin, buffer);
			Menu::eraseLine("Enter weaknesses: " + buffer);
			rev.setWeaknesses(buffer);
			fields[1] = "Enter Weaknesses\n    " + buffer;
			break;
			// suggestions
			case 2:
			std::cout << "Enter suggestions: ";
			getline(std::cin, buffer);
			Menu::eraseLine("Enter suggestions: " + buffer);
			rev.setSuggestions(buffer);
			fields[2] = "Enter Suggestions\n    " + buffer;
			break;
			// short paper
			case 3:
			std::cout << "Enter short paper remark: ";
			getline(std::cin, buffer);
			Menu::eraseLine("Enter short paper remark: " + buffer);
			rev.setShortPaper(buffer);
			fields[3] = "Enter Short Paper Remark\n    " + buffer;
			break;
			// best paper
			case 4:
			std::cout << "Enter best paper remark: ";
			getline(std::cin, buffer);
			Menu::eraseLine("Enter best paper remark: " + buffer);
			rev.setBestPaper(buffer);
			fields[4] = "Enter Best Paper Remark\n    " + buffer;
			break;
			// remarks
			case 5:
			std::cout << "Enter remarks for PC members: ";
			getline(std::cin, buffer);
			Menu::eraseLine("Enter remarks for PC members: " + buffer);
			rev.setRemarks(buffer);
			fields[5] = "Enter Remarks for PC Members\n    " + buffer;
			break;
			// sub reviewer's details
			case 6:
			{
				std::string first, last, email;
				std::cout << "Enter sub reviewer first name: ";
				getline(std::cin, first);
				Menu::eraseLine("Enter sub reviewer first name: " + first);
	
				std::cout << "Enter sub reviewer last name: ";
				getline(std::cin, last);
				Menu::eraseLine("Enter sub reviewer last name: " + last);
	
				std::cout << "Enter sub reviewer email: ";
				getline(std::cin, email);
				Menu::eraseLine("Enter sub reviewer email: " + email);
				rev.setReviewer(first, last, email);
				fields[6] = "Set subreviewer's details\n    " + first + " " + last + ", " + email;
			}
			break;
			// evaluations value menu
			case 7:
			{
				std::string overallEvalStr [1] = {
					"Overall Evaluation: "
				};
				Menu overallMenu;
				overallMenu.setOptions("Set overall evaluation:\n-3 (strong reject) <----> 3 (strong accept)", overallEvalStr, 1);
				overallMenu.setValues(overallEval);
				overallMenu.setValueBounds(-3, 3);
				overallMenu.disableBackButton();
				overallMenu.setLastAsChoice();
				overallMenu.setShowControls();
				
				overallMenu.doMenu();
				rev.setOverallEvaluation(overallEval[0]);
				
				std::string revConfStr [1] = {
					"Reviewer Confidence: "
				};
				Menu revConfMenu;
				revConfMenu.setOptions("Set reviewer confidence:\n0 (null) <----> 4 (expert)", revConfStr, 1);
				revConfMenu.setValues(revConf);
				revConfMenu.setValueBounds(0, 4);
				revConfMenu.disableBackButton();
				revConfMenu.setLastAsChoice();
				revConfMenu.setShowControls();
				
				revConfMenu.doMenu();
				rev.setReviewerConfidence(revConf[0]);
				
				std::string scoreStr [6] = {
					"Relevance: ",
					"Originality: ",
					"Significance: ",
					"Presentation: ",
					"Technical Quality: ",
					"Evaluation: "
				};
				Menu scoreMenu;
				scoreMenu.setOptions("Set scores:\n1 (lowest score) <----> 5 (highest score)", scoreStr, 6);
				scoreMenu.setValues(scores);
				scoreMenu.setValueBounds(1, 5);
				scoreMenu.disableBackButton();
				scoreMenu.setLastAsChoice();
				scoreMenu.setShowControls();
				
				scoreMenu.doMenu();
				rev.setRelevance(scores[0]);
				rev.setOriginality(scores[1]);
				rev.setSignificance(scores[2]);
				rev.setPresentation(scores[3]);
				rev.setTechnicalQuality(scores[4]);
				rev.setEvaluation(scores[5]);
			}
			break;
			// submit
			case 8: submitted = true; option = -1;
			break;
			// cancel
			case 9:	submitted = false;
			break;
		}
	} while (form.notExited(option));
	return submitted;
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
		"Current Phase: " + phase,
		"Advance to next phase",
		"Add reviewers",
		"Add authors",
        "Change papers per reviewer limit",
        "Change reviewers per paper limit",
        "Back"
	};
	// alter the first option to display the current phase
	configurationMenu.setOptions("Main Menu > Configuration", configurationMenuOptions, 7);
	configurationMenu.setSelectColour(text::Colour_Yellow);
	int option;
    do
    {
    	option = configurationMenu.doMenu();
    	switch(option)
    	{
    		// advance to next phase
    		case 1:
    			// confirm choice with user
    			if (confirmMenu("Advance to next phase?"))
    			{
    				std::string failReason;
    				if (this->advancePhase(failReason))
    				{
						// get next phase by request
						this->getPhase();
						configurationMenuOptions[0] = "Current Phase: " + phase;
					}
					else
					{
						std::cout << "Error: " << failReason;
						std::cin.ignore(1, '\n');
						Menu::eraseLine("Error: " + failReason);
					}
    			}
            break;
            // add reviewers
            case 2:
            	do
            	{
            		std::string tmpReviewer;
            		std::cout << "Enter reviewer name: ";
            		getline(std::cin, tmpReviewer);
            		Menu::eraseLine("Enter reviewer name: " + tmpReviewer);
            		
            		if (!addMember(tmpReviewer, "ADD_REVIEWER"))
            		{
            			std::cout << "Error: Could not add reviewer to conference.";
            			std::cin.ignore(1, '\n');
            			Menu::eraseLine("Error: Could not add reviewer to conference.");
            		}
            	} while (confirmMenu("Add more reviewers?"));
            break;
            // add authors
            case 3:
            	do
            	{
            		std::string tmpAuthor;
            		std::cout << "Enter author name: ";
            		getline(std::cin, tmpAuthor);
            		Menu::eraseLine("Enter author name: " + tmpAuthor);
            		
            		if (!addMember(tmpAuthor, "ADD_AUTHOR"))
            		{
            			std::cout << "Error: Could not add author to conference.";
            			std::cin.ignore(1, '\n');
            			Menu::eraseLine("Error: Could not add author to conference.");
            		}
            	} while (confirmMenu("Add more authors?"));
            break;
            // change number of papers that reviewers can review
            case 4:
            {
            	int limit;
            	do
            	{
            		std::string input;
            		std::cout << "Enter new papers per reviewer limit: ";
            		getline(std::cin, input);
            		Menu::eraseLine("Enter new papers per reviewer limit: " + input);
            		limit = atoi(input.c_str());
            		if (limit > 0)
            		{
            			changeAllocLimit(limit, "CHANGE_MAX_ALLOCATED_CONF");
            			std::ostringstream os;
            			limit = getAllocLimit("GET_MAX_ALLOCATED_CONF");
            			os << "Papers per reviewer limit is now " << limit;
            			std::cout << os.str();
            			std::cin.ignore(1, '\n');
            			Menu::eraseLine(os.str());
            		} 
            		else
            		{
            			std::cout << "Invalid limit. Limit must be 1 or more.";
            			std::cin.ignore(1, '\n');
            			Menu::eraseLine("Invalid limit. Limit must be 1 or more.");
            		}
            	} while (limit <= 0);
            }
            break;
            // change number of reviewers that can be assigned to a paper
            case 5:
            {
            	int limit;
            	do
            	{
            		std::string input;
            		std::cout << "Enter new reviewers per paper limit: ";
            		getline(std::cin, input);
            		Menu::eraseLine("Enter new reviewers per paper limit: " + input);
            		limit = atoi(input.c_str());
            		if (limit > 0)
            		{
            			changeAllocLimit(limit, "CHANGE_MAX_PAPERREVIEWERS_CONF");
            			std::ostringstream os;
            			limit = getAllocLimit("GET_MAX_PAPERREVIEWERS_CONF");
            			os << "Reviewers per paper limit is now " << limit;
            			std::cout << os.str();
            			std::cin.ignore(1, '\n');
            			Menu::eraseLine(os.str());
            		} 
            		else
            		{
            			std::cout << "Invalid limit. Limit must be 1 or more.";
            			std::cin.ignore(1, '\n');
            			Menu::eraseLine("Invalid limit. Limit must be 1 or more.");
            		}
            	} while (limit <= 0);
            }
           	break;
            case 6:
            break;
    	}
    } while (configurationMenu.notExited(option));
}

int UserController::getAllocLimit(const std::string &protocol)
{
	sf::Packet request, response;
	sf::Int16 receiveVal;
	request << protocol << username << conference;
	socket.send(request);
	socket.receive(response);
	response >> receiveVal;
	return receiveVal;
}

void UserController::changeAllocLimit(const int val, const std::string &protocol)
{
	sf::Packet request;
	sf::Int16 transmittedVal = val;
	request << protocol << username << conference << transmittedVal;
	socket.send(request);
}

bool UserController::addMember(const std::string &user, const std::string &protocol)
{
	sf::Packet request, response;
	bool result = false;
	request << protocol << username << conference << user;
	socket.send(request);
	socket.receive(response);
	response >> result;
	return result;
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

bool UserController::advancePhase(std::string &reason)
{
	bool result = false;
	sf::Packet request, response;
	std::string protocol = "ADVANCE_PHASE";
	request << protocol << username << conference;
	socket.send(request);
	socket.receive(response);
	response >> result;
	if (!result)
	{
		response >> reason;
	}
	return result;
}

void UserController::discussion()
{
	sf::Packet request, response;
	std::string protocol = "GET_COMMENTS";
	request << protocol << conference;
	// send request to server to view submissions
	std::vector<std::string> subs;
	getAllocations(subs, username);
	subs.push_back("Back");

	// select submission	
	Menu discuss;
	discuss.setOptions("Reviews > Discussions", &subs[0], subs.size());
	int option, backOption = subs.size()-1;
	std::string submissionTitle;
	option = discuss.doMenu();
	if (option != -1 && option != backOption)
	{
		// request comments from server
		submissionTitle = subs[option];
		request << submissionTitle;
		socket.send(request);
		socket.receive(response);
		
		// print comments about submission
		std::vector<Comment> comList;
		int comSize;
		response >> comSize;
		for (int i = 0; i < comSize; ++i)
		{
			Comment tmp;
			response >> tmp;
			comList.push_back(tmp);
			std::cout << tmp.username << ": " << tmp.comment << std::endl;
		}
		
		// enter comment
		std::string mycomment;
		std::cout << "Enter comment (or type 'stop' to cancel): ";
		getline(std::cin, mycomment);
		Menu::eraseLine("Enter comment (or type 'stop' to cancel): " + mycomment);
		
		// erase previous comments
		for (int i = comSize-1; i >= 0; i--)
		{
			Menu::eraseLine(comList[i].username + ": " + comList[i].comment);
		}
		
		if (mycomment != "stop")
		{
			// pack comment and send to server to add to comments
			sf::Packet comPacket;
			protocol = "SEND_COMMENT";
			comPacket << protocol << username << conference << submissionTitle << mycomment;
			socket.send(comPacket);
		}
	}
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

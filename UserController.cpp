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
	configurationMenu.setOptions("Main Menu > Account", configurationMenuOptions, 4);
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
            case 2;
                break;
            case 3;
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

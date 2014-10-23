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
		"Withdraw a paper",
        "Add comment",
        "Display discussion",
		"Back"
	};
	submissionMenu.setOptions("Main Menu > Submissions", subMenuOptions, 6);
  
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
            /*
                pack submission and send
                sf::Packet submitPacket;
                submitPacket << sub;
            */
		   		Submission sub;
		        sub.submit();            
		        // clear screen
		        std::cout << "\033[2J";
		    }
		    break;
		    // View submissions
		    case 1:      
		        // clear screen
		        std::cout << "\033[2J";
		    break;
		    // Withdraw a paper
		    case 2:
		        std::cout << "Which submission would you like to withdraw?" << std::endl;     
		        std::string whichSub;
		        getline(std::cin, whichSub);
		        
		        // clear screen
		        std::cout << "\033[2J";   
		    break;
            // Add comment
            case 3:
            break;
            // Display discussion
            case 4:
            break:
            // Back
            case 5:
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
		"Change reviewers",
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
                // 
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

        // TO DO: get comments about submission
        // query to server getting submissions
}

void UserController::notifications()
{
    std::cout << "Main Menu > Notifications" << std::endl << std::endl;
    // send query to server to check if papers have been reviewed
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

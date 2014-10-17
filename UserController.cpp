#include "UserController.h"

UserController::UserController()
{
    reviewed = false;
}

void UserController::account()
{
    // Need to change Main Menu to whatever type of Account the user is, (Jon is working on accounts atm).
    std::cout << "Main Menu > Account" << std::endl << std::endl;
    
    // Output Account details here.
}

void UserController::submissions()
{
	// set up menu for this part
	Menu submissionMenu;
	std::string subMenuOptions [4] = {
		"Submit a paper",
		"View submissions",
		"Withdraw a paper",
		"Back"
	};
	submissionMenu.setOptions("Main Menu > Submissions", subMenuOptions, 4);
  
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
		        mySubmissions.push_back(sub);
		        // clear screen
		        std::cout << "\033[2J";
		    }
		    break;
		    // View submissions
		    case 1:
		        for(int i = 0; i < (int)mySubmissions.size(); i++)
		            mySubmissions[i].view();  
		        std::cout << "(Press Enter to continue)";
		        std::cin.ignore(1, '\n');      
		        // clear screen
		        std::cout << "\033[2J";
		    break;
		    // Review a paper
		    case 2:
		        std::cout << "Which submission would you like to withdraw?" << std::endl;     
		        std::string whichSub;
		        getline(std::cin, whichSub);
		        
		        // clear screen
		        std::cout << "\033[2J";   
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
    // Need to ask team what goes in here
}

void UserController::discussion()
{
    std::cout << "Main Menu > Discussions" << std::endl << std::endl;
    // Need to create discussion forum some how
    
    // Simple forum: List of messages from different reviewers
    // in chronological order about a topic (the paper they're reviewing)
}

void UserController::notifications()
{
    std::cout << "Main Menu > Notifications" << std::endl << std::endl;
    // include notifications such as paper reviewed, need more than that i think
}

void UserController::setReviewed(bool reviewed)
{
    this->reviewed = reviewed;
}

void UserController::viewLogs()
{
    std::cout << "Main Menu > View Logs" << std::endl << std::endl;
    
    // Need to create logging system/class
}

void UserController::adminTools()
{
    std::cout << "Main Menu > Admin Tools" << std::endl << std::endl;

    // Need to ask team what goes in here, i think its the same as configuration
}

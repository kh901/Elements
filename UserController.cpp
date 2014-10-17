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
    std::cout << "Main Menu > Submissions" << std::endl << std::endl;
    
    int option;
    
    do
    {
        // I think Jon will need to change this menu form, so that it conforms with the actual menu system UI capabilities such as scrolling/backspace to go back and enter to select an option.
        std::cout << "1 - submit a paper" << std::endl;
        std::cout << "2 - view submissions" << std::endl;
        std::cout << "3 - review a paper" << std::endl;
        std::cout << "4 - go back" << std::endl;
        std::cin >> option;
        
        /*  HUGE ERROR WHEN COMPILING NO IDEA WHATS CAUSING, NOT ENOUGH INFORMATION IS GIVEN FROM ERROR
            HELP!!
        if(option == 1)
        {
            Submission sub;
            sub.submit();            
            mySubmissions.push_back(sub);
        }*/
        if(option == 2)
        {
            for(int i = 0; i < mySubmissions.size(); i++)
                mySubmissions[i].view();        
        }
        if(option == 3)
        {
            std::cout << "Which submission would you like to withdraw?" << std::endl;        
        }
        if(option == 4)
        {
            std::cout << "Invalid input!" << std::endl;        
        }
    }while(option != 4);
}

void UserController::reviews()
{
    std::cout << "Main Menu > Reviews" << std::endl << std::endl;;
    
    // Need database code for reviews
    // Need to create review form 
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

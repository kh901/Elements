#include "Menu.h"
#include "Submission.h"

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#ifndef USER_CONTROLLER
#define USER_CONTROLLER

class UserController
{
    public:
        UserController();
        void createConference();
        void account();
        void submissions();
        void reviews();
        void configuration();
        void discussion();
        void notifications();
        void viewLogs();
        void adminTools();
        void mainMenu();
        
    private
        std::string username;
        std::string conference;
        //access level
};

#endif


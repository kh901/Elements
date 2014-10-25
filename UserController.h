#include "Account.h"
#include "Menu.h"
#include "Conference.h"
#include "Submission.h"

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include<SFML/Network.hpp>
#include<SFML/System/Time.hpp>

#ifndef USER_CONTROLLER
#define USER_CONTROLLER

#define NETWORK_PORT 60000

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
        void startMenu();
       	bool connect();
        void run();
        
    private:
        std::string username;
        std::string conference;
        Account::AccessLevel level;
        bool isAdmin;
        
        sf::TcpSocket socket;
        
        void fillMainMenu(std::vector<std::string> &);
        
        void loginAccount();
        void registerAccount();
       	bool pickConference();
        void getConferenceAccess();
        void submitPaper();
        void viewSubmissions();
        void changeUsername();
        void changePassword();
        void getAdminStatus();
        void logOut();
};

#endif


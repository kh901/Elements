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
#define MAX_INVALID_LOGINS 3

class UserController
{
    public:
        UserController();
        void run();
        
    private:
        std::string username;
        std::string conference;
        Account::AccessLevel level;
        bool isAdmin;
        std::string phase;
        
        sf::TcpSocket socket;
        
        void fillMainMenu(std::vector<std::string> &);
        bool confirmMenu(const std::string &);
        
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
       	void finalise();
        
        void prepareFinalReview(const std::string &);
        bool addReviewer(const std::string &);
        void getAllocations(std::vector<std::string> &);
        void getBidList(std::vector<std::string> &);
        void bidPaper();
        void submitReview();
        void getPhase();
        void advancePhase();
        bool loginAccount();
        void registerAccount();
       	bool pickConference();
       	bool createConference();
        void getConferenceAccess();
        void submitPaper();
        void viewSubmissions();
        void changeUsername();
        void changePassword();
        void getAdminStatus();
        void logOut();
        bool sendCreatedConference(const Conference &);
};

#endif


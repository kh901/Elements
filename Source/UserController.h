#include "Account.h"
#include "Menu.h"
#include "Conference.h"
#include "Submission.h"
#include "Review.h"

#include <ctime>
#include <cstdlib>
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
        
        // Manual allocation
        void getFreeReviewers(std::vector<std::string> &);
        bool needsAllocation(const std::string &);
        void assignReviewers(const std::string &);
        void getConferenceAlloc(std::vector<std::string> &);
        void doAllocations();
        void viewReviewerAllocations(const std::string &);
        void viewAllocations();
        // ---------------------------------------------------
        bool getFinalReview(const std::string &, std::string &);
        bool hasReviewed(const std::string &);
        void viewMyReview(const std::string &);
        void updateMainMenuMarquee(std::ostringstream &);
        int getNotifyCount();
        void viewReview(const std::string &);
       	bool getAccountName(const std::string &, std::string &, std::string &);
        void changeAllocLimit(const int, const std::string &);
        int getAllocLimit(const std::string &);
        bool createReviewForm(Review &);
        void detailSub(const std::string &);
        void prepareFinalReview(const std::string &);
        bool addMember(const std::string &, const std::string &);
        void getAllocations(std::vector<std::string> &, const std::string &);
        void getBidList(std::vector<std::string> &);
        void bidPaper();
        void submitReview();
        void getPhase();
        bool advancePhase(std::string &);
        bool loginAccount();
        void registerAccount();
       	bool pickConference();
       	bool createConference();
        void getConferenceAccess();
        void submitPaper();
        void viewSubmissions();
        void changePassword();
        void getAdminStatus();
        void logOut();
        bool sendCreatedConference(const Conference &);
};

#endif


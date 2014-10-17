#include "Submission.h"

#include <iostream>
#include <string>
#include <vector>

#ifndef USER_CONTROLLER
#define USER_CONTROLLER

class UserController
{
    public:
        UserController();
        void account();
        void submissions();
        void reviews();
        void configuration();
        void discussion();
        void notifications();
        void viewLogs();
        void adminTools();

        void setReviewed(bool);
    private:
        // Object initialization of class type Account here (Jon is working on accounts atm).
        std::vector<Submission> mySubmissions;
        
        bool reviewed;
        
};

#endif


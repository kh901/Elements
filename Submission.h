#include <iostream>
#include <fstream>
#include <vector>
#include "Account.h"

#ifndef SUBMISSION_H
#define SUBMISSION_H

class Submission
{
    public:
        Submission();
        void submit();
        void view();
        void withdraw();
        
        struct Comment
        {
            std::string username;
            std::string comment;
        };
        
        void addComment();
        void displayComments();
        
    private:
        bool submitted;
        std::string filename;
        
        std::vector<std::string> authors;
        std::string title;
        std::string description;
        
        std::vector<Comment> comments;
    
};

#endif


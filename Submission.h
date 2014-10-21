#include <iostream>
#include <fstream>
#include <vector>
#include "Account.h"
#include "File.h"

#ifndef SUBMISSION_H
#define SUBMISSION_H

class Submission : public FileIO
{
    public:
        Submission();
        void submit();
        void view();
        void withdraw();
        
        void setReviewed(bool);
        
        struct Comment
        {
            std::string username;
            std::string comment;
        };
        
        void addComment(Account account);
        void displayComments();

		// write and read this class to a binary file stream
		void writeFile(std::ofstream &);
		bool readFile(std::ifstream &);
    private:
        bool submitted;
        bool reviewed;
        std::string filename;
        std::string title;
        std::string description;
        std::vector<std::string> authors;
        std::vector<std::string> keywords;
        std::vector<Comment> comments;
    
};

#endif


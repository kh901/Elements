#include <iostream>
#include <fstream>
#include <vector>
#include "Account.h"
#include "File.h"

#ifndef SUBMISSION_H
#define SUBMISSION_H

class Comment : public FileIO
{
	public:
	Comment() {}
	Comment(const Comment &);
	
    std::string username;
    std::string comment;
	
	Comment & operator=(const Comment &);
	
	// write and read this class to a binary file stream
	void writeFile(std::ofstream &) const;
	void readFile(std::ifstream &);
};

class Submission : public FileIO
{
    public:
        Submission();
        Submission(const Submission &);
        void submit();
        void view();
        void withdraw();
        
        void setReviewed();
        
        void addComment(Account account);
        void displayComments();
        
        Submission & operator=(const Submission &);

		// write and read this class to a binary file stream
		void writeFile(std::ofstream &) const;
		void readFile(std::ifstream &);
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


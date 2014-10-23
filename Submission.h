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

class Fullname : public FileIO
{
	public:
	Fullname() {}
	Fullname(const Fullname &);
	
	std::string firstname;
	std::string surname;
	
	Fullname & operator=(const Fullname &);
	
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
		void addAuthor(const std::string &, const std::string &);
		std::string getTitle();
		Submission & operator=(const Submission &);
		bool isAuthorIncluded(const std::string &, const std::string &);
		// write and read this class to a binary file stream
		void writeFile(std::ofstream &) const;
		void readFile(std::ifstream &);
    private:
		bool reviewed;
		std::string filename;
		std::string title;
		std::string description;
		std::vector<Fullname> authors;
		std::vector<std::string> keywords;
		std::vector<Comment> comments;
		
};


#endif


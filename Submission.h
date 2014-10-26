#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Network.hpp>
#include "Account.h"
#include "File.h"


#ifndef SUBMISSION_H
#define SUBMISSION_H

class Comment : public FileIO
{
	public:
	Comment() {}
	Comment(const Comment &);
	
	// also store time / date of comment
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
		void accept();
		void reject();
		std::string getStatus() const;
		std::string getTitle() const;
		void setConference(const std::string &);
		std::string getConference() const;
		void setUniversity(const std::string &);
		std::string getUniversity() const;

		void addReviewer(const std::string &);
		bool hasReviewer(const std::string &);
		int getReviewerCount() const;

		void addComment(Account account);
		void displayComments();
		void addAuthor(const std::string &, const std::string &);
		Submission & operator=(const Submission &);
		bool isAuthorIncluded(const std::string &, const std::string &);
		// write and read this class to a binary file stream
		void writeFile(std::ofstream &) const;
		void readFile(std::ifstream &);
    private:
    	std::string status;
		bool reviewed;
		std::string filename;
		std::string title;
		std::string description;
		std::string conference;
		std::string uni;
		std::vector<std::string> reviewers;
		std::vector<Fullname> authors;
		std::vector<std::string> keywords;
		std::vector<Comment> comments;
		
		friend sf::Packet & operator<<(sf::Packet &packet, const Submission &);
		friend sf::Packet & operator>>(sf::Packet &packet, Submission &);	
};

#endif


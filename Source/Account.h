#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm>
#include <vector>
#include <fstream>
#include <SFML/Network.hpp>
#include "Menu.h"
#include "File.h"
#include "Submission.h"

#define ACCOUNT_MAX_ID_LEN 9

std::string encrypt(const std::string &);

class Account : public FileIO
{
	public:
		enum AccessLevel { 
			Access_None,
			Access_Author,
			Access_Reviewer,
			Access_Chairman,
			Access_Admin
		};
		
		enum AccountType {
			Account_User,
			Account_Admin
		};
		
		Account();
		Account(const Account &);
		Account(const AccountType, const bool = false);
	
		// functions dealing with access to conferences
		bool addAccess(const std::string &, const AccessLevel);
		void changeAccess(const std::string &, const AccessLevel);
		AccessLevel getAccess(const std::string &);
		void clearAccess();
		void printAccess();
		bool hasAccess(const std::string &);
		void getConferences(std::vector<std::string> &);
		
		// functions dealing with bidding and papers allocated to review
		bool incrementAllocated(const std::string &, const int);
		bool checkAllocation(const std::string &, const int);
		
		bool isSystemAdmin() { return accountType == Account_Admin; }
		void setSystemAdmin() { accountType = Account_Admin; }			// this must only be done in necessary circumstances
		
		// user and password functions
		std::string getUsername() const { return username; }
		void setUsername(const std::string &);
		void setPassword(const std::string &);		// encrypts the given string with a one way encryption
		bool matchUsername(const std::string &aUser) { return username == aUser; }
		bool matchPassword(const std::string &, const bool doEncryption = true);
				
		// get/set details
		void setFirstName(const std::string &aFirst);
		std::string getFirstName() const;
		void setLastName(const std::string &aLast);
		std::string getLastName() const;
		void setEmail (const std::string &anEmail);
		std::string getEmail() const;
		void setUniversity(const std::string &aUni);
		std::string getUniversity() const;
		void addKeyword(const std::string &word);
		bool findKeyword(const std::string &word);
		void clearKeywords();
		void displayKeywords();
		
		// matching keywords
		bool matchKeywordsForSubmission(const Submission &);
		
		// login session functions
	 	void startSession();
		void endSession();
		bool isLoggedIn() { return loggedIn; }
		
		Account & operator=(const Account &);
		
		// write and read this class to a binary file stream
		void writeFile(std::ofstream &) const;
		void readFile(std::ifstream &);
	protected:
		std::string username;
		std::string password;			// user's "password" encrypted with our oneway encryption
		std::string firstName;
		std::string lastName;
		std::string email;
		std::string university;
		bool loggedIn;
		AccountType accountType;
		std::vector<std::string> keywords;
		// a map of each conference that this account has a level of access higher than none
		std::map<std::string, AccessLevel> accessMap;
		// a map of each conference with the number of papers allocated to review
		std::map<std::string, int> allocatedMap;
	private:
		std::string generateId();
};

sf::Packet & operator<<(sf::Packet &packet, const Account::AccessLevel &level);

sf::Packet & operator>>(sf::Packet &packet, Account::AccessLevel &level);

#endif

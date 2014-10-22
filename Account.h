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
#include "Menu.h"
#include "File.h"

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
		
		bool isSystemAdmin() { return accountType == Account_Admin; }
		void setSystemAdmin() { accountType = Account_Admin; }			// this must only be done in necessary circumstances
		
		// user and password functions
		std::string getUsername() { return username; }
		void setUsername(const std::string &);
		void setPassword(const std::string &);		// encrypts the given string with a one way encryption
		bool matchUsername(const std::string &aUser) { return username == aUser; }
		bool matchPassword(const std::string &, const bool encryptValue = true);
				
		// get/set details
		void setFirstName(const std::string &aFirst)
		{
			firstName = aFirst;
		}
		std::string getFirstName()
		{
			return firstName;
		}
		void setLastName(const std::string &aLast)
		{
			lastName = aLast;
		}
		std::string getLastName()
		{
			return lastName;
		}
		void setEmail (const std::string &anEmail) 
		{ 
			email = anEmail; 
		}
		std::string getEmail() 
		{ 
			return email; 
		}
		void setUniversity(const std::string &aUni) 
		{ 
			university = aUni; 
		}
		std::string getUniversity() 
		{ 
			return university;
	 	}
		void addKeyword(const std::string &word) 
		{ 
			keywords.push_back(word); 
		}
		bool findKeyword(const std::string &word) 
		{ 
			return (std::find(keywords.begin(), keywords.end(), word) != keywords.end()); 
		}
		void clearKeywords() 
		{ 
			keywords.clear(); 
		}
		void displayKeywords()
		{
			std::vector<std::string>::iterator it;
			for (it = keywords.begin(); it != keywords.end(); ++it)
			{
				std::cout << "Keyword: " << *it << std::endl;
			}
		}
		
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
										// 8 character with colon separator - Theoretical maximum ids of 218340105584896
										// a-z, A-Z, 0-9 as characters 
		std::string firstName;
		std::string lastName;
		std::string email;
		std::string university;
		bool loggedIn;
		AccountType accountType;
		std::vector<std::string> keywords;
		// a map of each conference that this account has a level of access higher than none
		std::map<std::string, AccessLevel> accessMap;
	private:
		std::string generateId();
};

#endif

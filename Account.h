#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include "Menu.h"

#define ACCOUNT_MAX_ID_LEN 9

std::string encrypt(const std::string &);

class Account
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
		Account(const std::string &, const AccountType, const bool = false);
	
		// functions dealing with access to conferences
		bool addAccess(const std::string &, const AccessLevel);
		void changeAccess(const std::string &, const AccessLevel);
		AccessLevel getAccess(const std::string &);
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
		
		// login session functions
		std::string startSession();
		void endSession();
		bool isLoggedIn() { return loggedIn; }
	protected:
		std::string username;
		std::string password;			// user's "password" encrypted with our oneway encryption
		std::string uniqueId;			// unique id for this account 
										// 8 character with colon separator - Theoretical maximum ids of 218340105584896
										// a-z, A-Z, 0-9 as characters 
		std::string sessionId;
		bool loggedIn;
		// a map of each conference that this account has a level of access higher than none
		std::map<std::string, AccessLevel> accessMap;
		AccountType accountType;
	private:
		std::string generateId();
};

#endif

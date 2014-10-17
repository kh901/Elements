#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include "Menu.h"

std::string encrypt(const std::string &);

class Account
{
	public:
		Account();
		
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
	
		bool isSystemAdmin() { return accountType == Account_Admin; }
		bool matchPassword(const std::string &);
	protected:
		std::string username;
		std::string password;			// user's "password" encrypted with our oneway encryption
		std::string uniqueId;			// unique id for this account 
										// 8 character with colon separator - Theoretical maximum ids of 218340105584896
										// a-z, A-Z, 0-9 as characters 
										
		// a map of each conference that this account has a level of access higher than none
		std::map<std::string, AccessLevel> accessMap;
		AccountType accountType;
	private:
		void generateId();
};

#endif

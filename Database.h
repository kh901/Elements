#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <fstream>
#include <vector>
#include "Account.h"
#include "Conference.h"
#include "File.h"
#include "Submission.h"

#define DATABASE_ACCOUNTS_FILENAME "elements_acc.bin"
#define DATABASE_CONFERENCES_FILENAME "elements_con.bin"
#define DATABASE_SUBMISSIONS_FILENAME "elements_sub.bin"

#define SYSTEM_ADMIN_DEFAULT_USERNAME "cms_admin"
#define SYSTEM_ADMIN_DEFAULT_PASSWORD "cms_password"

/*
	A class that manages saving, loading and
	data manipulation of the other objects.
*/
class Database
{
	public:
		Database();
		
		// database management
		void save();
		void load();
		
		// data manipulation
		// For every kind of data we store
			// add new
			// return single element based on a search
			// return a list of elements based on a search
			// edit an existing element based on a search
			// OPTIONAL delete an element
		void addAccount(const Account &);
		Account getAccount(const std::string &);
		void editAccount(const Account &);
		
		void addConference(const Conference &);
		Conference getConference(const std::string &);
		void editConference(const Conference &);
		
		void addSubmission(const Submission &);
		Submission getSubmission(const std::string &);
		void editSubmission(const Submission &);
	private:
		// data structures in memory
		std::vector<Account> accounts;
		std::vector<Conference> conferences;
		std::vector<Submission> submissions;
		
		void setupFiles();
		bool filesExist();
};

#endif

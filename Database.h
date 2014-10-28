#ifndef DATABASE_H
#define DATABASE_H

#include <ctime>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>

#include "Account.h"
#include "common.h"
#include "Conference.h"
#include "File.h"
#include "Submission.h"

#define DATABASE_ACCOUNTS_FILENAME "./Storage/Data/elements_acc.bin"
#define DATABASE_CONFERENCES_FILENAME "./Storage/Data/elements_con.bin"
#define DATABASE_SUBMISSIONS_FILENAME "./Storage/Data/elements_sub.bin"

#define DATABASE_PAPERS_DIRECTORY "./Storage/Papers/"
#define DATABASE_LOGS_DIRECTORY "./Storage/Logs/"

#define SYSTEM_ADMIN_DEFAULT_USERNAME "cms_admin"
#define SYSTEM_ADMIN_DEFAULT_PASSWORD "cms_password"

/*
	A class that thinly wraps a vector so that it can intercept when its contents is changed.
	It can manage saving itself as well as loading itself from a given filename.
*/
class Resource
{
	public:
	private:
};

class LogManager
{
	public:
		LogManager();
	
		// automatically adds the timestamp to the front of the log event
		void addLog(const std::string &);
		void getRecentLog(std::vector<std::string> &);
		void updateLogDate();
	private:
		// current log filename - based on the current date
		std::string logFilename;
		int countLogFileLines(const std::string &);
};

class Database
{
	public:
		Database();
		
		// database management
		void save();
		void saveAccounts();
		void saveConferences();
		void saveSubmissions();
		void load();
		void loadAccounts();
		void loadConferences();
		void loadSubmissions();
		
		// data manipulation
		// For every kind of data we store
			// add new
			// return single element based on a search
			// return a list of elements based on a search
			// edit an existing element based on a search
			// OPTIONAL delete an element
		void addAccount(const Account &);
		bool getAccount(const std::string &, Account &);
		void editAccount(const Account &);
		
		void addConference(const Conference &);
		bool getConference(const std::string &, Conference &);
		void editConference(Conference &);
		
		void addSubmission(const Submission &);
		bool getSubmission(const std::string &, Submission &);
		void editSubmission(Submission &);
		
		bool checkFile(const std::string &);
		
		void update();
	private:
		// data structures in memory
		std::vector<Account> accounts;
		std::vector<Conference> conferences;
		std::vector<Submission> submissions;
		
		LogManager log;
		
		void setupFiles();
		bool filesExist();
};

#endif /* DATABASE_H */

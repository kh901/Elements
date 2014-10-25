#include "Database.h"

Database::Database()
{
	// set current log file name according to current date
	std::ostringstream os;
	os << getDate() << ".log";
	logFilename = os.str();
	std::cout << "log file name today: " << logFilename << std::endl;
	load();
}

void Database::save()
{
	// save accounts
	saveAccounts();
	// save conferences
	saveConferences();
	// save submissions
	saveSubmissions();
}

void Database::saveAccounts()
{
	writeFileClassVector<Account>(DATABASE_ACCOUNTS_FILENAME, accounts);
}
void Database::saveConferences()
{
	writeFileClassVector<Conference>(DATABASE_CONFERENCES_FILENAME, conferences);
}
void Database::saveSubmissions()
{
	writeFileClassVector<Submission>(DATABASE_SUBMISSIONS_FILENAME, submissions);
}

void Database::load() 
{
	// check that the files exist
	if (filesExist())
	{
		// load accounts
		loadAccounts();
		
		// load conferences
		loadConferences();
		
		// load submissions
		loadSubmissions();
	}
	else
	{
		setupFiles();
	}
}

void Database::loadAccounts()
{
	readFileClassVector<Account>(DATABASE_ACCOUNTS_FILENAME, accounts);
}
void Database::loadConferences()
{
	readFileClassVector<Conference>(DATABASE_CONFERENCES_FILENAME, conferences);
}
void Database::loadSubmissions()
{
	readFileClassVector<Submission>(DATABASE_SUBMISSIONS_FILENAME, submissions);
}

void Database::addAccount(const Account &acc)
{
	accounts.push_back(acc);
	saveAccounts();
	addLog("Added new account. Username: " + acc.getUsername());
}

bool Database::getAccount(const std::string &request, Account &acc)
{
	std::vector<Account>::iterator itA;
	for (itA = accounts.begin(); itA != accounts.end(); ++itA)
	{
		if (itA->getUsername() == request)
		{
			acc = *itA;									
			return true;
		}
	}
	return false;
}

void Database::editAccount(const Account &acc)
{
	bool edited = false;
	std::vector<Account>::iterator itA;
	for (itA = accounts.begin(); itA != accounts.end(); ++itA)
	{
		if (itA->getUsername() == acc.getUsername())
		{
			*itA = acc;
			edited = true;
			break;
		}
	}
	if (edited)
	{
		saveAccounts();
		addLog("Edited account. Username: " + acc.getUsername());
	}
}

void Database::addConference(const Conference &con)
{
	conferences.push_back(con);
	saveConferences();
	addLog("Added new conference. Conference Name: " + con.getName());
}

bool Database::getConference(const std::string &request, Conference &con)
{
	std::vector<Conference>::iterator itC;
	for (itC = conferences.begin(); itC != conferences.end(); ++itC)
	{
		if (itC->getName() == request)
		{
			con = *itC;
			return true;
		}
	}
	return false;
}

void Database::editConference(Conference &con)
{
	bool edited = false;
	std::vector<Conference>::iterator itC;
	for (itC = conferences.begin(); itC != conferences.end(); ++itC)
	{
		if (con.getName() == itC->getName())
		{
			*itC = con;
			edited = true;
			break;
		}
	}
	if (edited)
	{
		saveConferences();
		addLog("Edited conference. Conference Name: " + con.getName());
	}
}

void Database::addSubmission(const Submission &sub)
{
	submissions.push_back(sub);
	saveSubmissions();
	addLog("Added new submission. Title: " + sub.getTitle());
}

bool Database::getSubmission(const std::string &request, Submission &sub)
{
	std::vector<Submission>::iterator itS;
	for (itS = submissions.begin(); itS != submissions.end(); ++itS)
	{
		if (itS->getTitle() == request)
		{
			sub = *itS;
			return true;
		}
	}
	return false;
}

void Database::editSubmission(Submission &sub)
{
	bool edited = false;
	std::vector<Submission>::iterator itS;
	for (itS = submissions.begin(); itS != submissions.end(); ++itS)
	{
		if (sub.getTitle() == itS->getTitle())
		{
			*itS = sub;
			edited = true;
			break;
		}
	}
	if (edited)
	{
		saveSubmissions();
		addLog("Edited submission. Title: " + sub.getTitle());
	}
}

void Database::setupFiles()
{
	// create default admin account
	Account admin;
	admin.setUsername(SYSTEM_ADMIN_DEFAULT_USERNAME);
	admin.setPassword(SYSTEM_ADMIN_DEFAULT_PASSWORD);
	admin.setSystemAdmin();
	accounts.push_back(admin);
	
	// create the files
	save();
}

bool Database::filesExist()
{
	bool fileStatus = false;
	
	std::ifstream fin;
	fin.open(DATABASE_ACCOUNTS_FILENAME, std::ios::binary);
	fileStatus = fin.good();
	if (fileStatus)
	{
		fin.close();
		fin.open(DATABASE_CONFERENCES_FILENAME, std::ios::binary);
		fileStatus = fin.good();
		if (fileStatus)
		{
			fin.close();
			fin.open(DATABASE_SUBMISSIONS_FILENAME, std::ios::binary);
			fileStatus = fin.good();
		}
	}
	fin.close();
	return fileStatus;
}

bool Database::checkFile(const std::string &filename)
{
	std::ifstream fin;
	std::ostringstream fullFilename;
	fullFilename << DATABASE_PAPERS_DIRECTORY << filename;
	fin.open(fullFilename.str().c_str(), std::ios::binary);
	bool status = fin.good();
	fin.close();
	return status;
}

void Database::addLog(const std::string &aEvent)
{
	std::ofstream fout;
	std::ostringstream fullFilename;
	fullFilename << DATABASE_LOGS_DIRECTORY << logFilename;
	
	std::cout << "filename: " << fullFilename.str() << std::endl;
	fout.open(fullFilename.str().c_str(), std::ios::app);
	// make the file if it doesn't exist
	if (!fout.good())
	{
		fout.close();
		std::cout << "file doesn't exist" << std::endl;
		std::ofstream create(fullFilename.str().c_str());
		create << "Start Log" << std::endl;
		create.close();
		fout.open(fullFilename.str().c_str(), std::ios::app);
	}
	fout << getTimestamp() << ": " << aEvent << std::endl;
	fout.close();
}
void Database::getRecentLog(std::vector<std::string> &list)
{
	std::ifstream fin;
	std::ostringstream fullFilename;
	fullFilename << DATABASE_LOGS_DIRECTORY << logFilename;
	// return if the file doesn't exist
	if (!fin.good())
	{
		return;
	}
	int linesInFile = countLogFileLines(fullFilename.str());
	std::string line;
	int start = 1, current = 1;
	if (linesInFile-30 > 0)
	{
		start = linesInFile-30;
	}
	while (getline(fin, line))	// skip to where the recent logs should start
	{
		if (current < start)
		{
			current++;
		}
		else if (!fin.eof())
		{
			list.push_back(line);
		}
	}
}

int Database::countLogFileLines(const std::string &filename)
{
	std::ifstream fin(filename.c_str());
	   
    fin.unsetf(std::ios_base::skipws);

    // count each line in the file 
    return std::count(std::istream_iterator<char>(fin),
        std::istream_iterator<char>(), '\n');
}

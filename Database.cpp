#include "Database.h"

Database::Database()
{
	// set current log file name according to current date
	std::ostringstream os;
	time_t timeVal = time(NULL);
	struct tm * currentTime = localtime(&timeVal);
	int day = static_cast<int>(currentTime->tm_mday);
	int month = 1 + static_cast<int>(currentTime->tm_mon); 
	int year = 1900 + static_cast<int>(currentTime->tm_year); 
	os << day << '-' << month << '-' << year << ".log";
	logFilename = os.str();
	std::cout << "log file name today: " << logFilename << std::endl;
	load();
}

void Database::addAccount(const Account &acc)
{
	std::ofstream fout;
	fout.open(DATABASE_ACCOUNTS_FILENAME, std::ios::binary | std::ios::out | std::ios::app);
	acc.writeFile(fout);
	fout.close();
}

Account Database::getAccount(const std::string &str)
{
	std::vector<Account>::iterator itA;
	for (itA = accounts.begin(); itA != accounts.end(); itA++)
	{
		/**
		 * 		DATA IS PROTECTED; CANNOT ACCESS CONVENTIONALLY
		 * 
		if (itA[0].username == str)
		{
			return itA[0];
		}
		**/
	}
}

void Database::editAccount(const Account &acc)
{
	std::ofstream fin;
	fin.open(DATABASE_ACCOUNTS_FILENAME, std::ios::binary | std::ios::in);
	/// acc.editFile(fin); 	/// PERHAPS A FUNCTION SIMILAR TO THIS IN ORDER TO EDIT IN ACCOUNT
	fin.close();
}

void Database::addConference(const Conference &con)
{
	std::ofstream fout;
	fout.open(DATABASE_CONFERENCES_FILENAME, std::ios::binary | std::ios::out | std::ios::app);
	con.writeFile(fout);
	fout.close();
}

Conference Database::getConference(const std::string &str)
{
	std::vector<Conference>::iterator itC;
	for (itC = conferences.begin(); itC != conferences.end(); itC++)
	{
		/**
		 * 		DATA IS PROTECTED; CANNOT ACCESS CONVENTIONALLY
		 * 
		if (itC[0].name == str)
		{
			return itC[0];
		}
		**/
	}
}

void Database::editConference(const Conference &con)
{
	std::ofstream fin;
	fin.open(DATABASE_CONFERENCES_FILENAME, std::ios::binary | std::ios::in);
	/// con.editFile(fin); 	/// PERHAPS A FUNCTION SIMILAR TO THIS IN ORDER TO EDIT IN ACCOUNT
	fin.close();
}

void Database::addSubmission(const Submission &sub)
{
	std::ofstream fout;
	fout.open(DATABASE_SUBMISSIONS_FILENAME, std::ios::binary | std::ios::out | std::ios::app);
	sub.writeFile(fout);
	fout.close();
}

Submission Database::getSubmission(const std::string &str)
{
	std::vector<Submission>::iterator itS;
	for (itS = submissions.begin(); itS != submissions.end(); itS++)
	{
		/**
		 * 		DATA IS PROTECTED; CANNOT ACCESS CONVENTIONALLY
		 * 
		if (itS[0].title == str)
		{
			return itS[0];
		}
		**/
	}
}

void Database::editSubmission(const Submission &sub)
{
	std::ofstream fin;
	fin.open(DATABASE_SUBMISSIONS_FILENAME, std::ios::binary | std::ios::in);
	/// sub.editFile(fin); 	/// PERHAPS A FUNCTION SIMILAR TO THIS IN ORDER TO EDIT IN ACCOUNT
	fin.close();
}

void Database::save()
{
	// save accounts
	writeFileClassVector<Account>(DATABASE_ACCOUNTS_FILENAME, accounts);
	
	// load conferences
	writeFileClassVector<Conference>(DATABASE_CONFERENCES_FILENAME, conferences);
	
	// load submissions
	writeFileClassVector<Submission>(DATABASE_SUBMISSIONS_FILENAME, submissions);
}

void Database::load() 
{
	// check that the files exist
	if (filesExist())
	{
		// load accounts
		readFileClassVector<Account>(DATABASE_ACCOUNTS_FILENAME, accounts);
		
		// load conferences
		readFileClassVector<Conference>(DATABASE_CONFERENCES_FILENAME, conferences);
		
		// load submissions
		readFileClassVector<Submission>(DATABASE_SUBMISSIONS_FILENAME, submissions);
	}
	else
	{
		setupFiles();
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
		create << "test" << std::endl;
		create.close();
		std::ifstream ifs;
		ifs.open(fullFilename.str().c_str());
		if (ifs.good())
		{
			std::cout << "worked" << std::endl;
		}
		else
		{
			std::cout << "didn't work" << std::endl;
		}
		ifs.close();
		fout.open(fullFilename.str().c_str(), std::ios::app);
	}
	fout << aEvent << std::endl;
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

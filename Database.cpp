#include "Database.h"

Database::Database()
{
	load();
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

void Database::save()
{
	// save accounts
	writeFileClassVector<Account>(DATABASE_ACCOUNTS_FILENAME, accounts);
	
	// load conferences
	writeFileClassVector<Conference>(DATABASE_CONFERENCES_FILENAME, conferences);
	
	// load submissions
	writeFileClassVector<Submission>(DATABASE_SUBMISSIONS_FILENAME, submissions);
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

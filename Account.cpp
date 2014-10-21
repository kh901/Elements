#include "Account.h"

std::string encrypt(const std::string &str)
{
	std::string result(str);
	int sum = 0;
	for (int i = 0; i < (int)result.length(); ++i)
	{
		sum += static_cast<int>(result[i]) + (i+1);
	}
	//sum = sum % 128;
	//sum = sum % 4096;
	//std::cout << "Sum: " << sum << std::endl;
	for (int i = 0; i < (int)result.length(); ++i)
	{
		result[i] = result[i] ^ static_cast<char>(sum);
	}
	return result;
}

Account::Account()
{
	accountType = Account_User;
	loggedIn = false;
	uniqueId = generateId();
}
Account::Account(const Account &anAcc)
{
	accountType = Account_User;
	loggedIn = false;
	uniqueId = generateId();
}
Account::Account(const std::string &anId, const AccountType accType, const bool loginStatus)
{
	accountType = accType;
	loggedIn = loginStatus;
	uniqueId = anId;
}

bool Account::matchPassword(const std::string &str, const bool encryptValue)
{
	if (encryptValue)
	{
		return password == encrypt(str);
	}
	return password == str;
}

std::string Account::generateId()
{
	std::ostringstream os;
	srand(time(NULL));
	for (int i = 0; i < ACCOUNT_MAX_ID_LEN; ++i)
	{
		if (i != 4)
		{
			switch(rand()%3)
			{
				// lower case
				case 0:
					os << static_cast<char>((rand()%26) + 97);
				break;
				// upper case
				case 1:
					os << static_cast<char>((rand()%26) + 65);
				break;
				// digits
				case 2:
					os << rand()%10;
				break;
			}
		}
		else
		{
			os << ':';
		}
	}
	std::cout << "Gen id:" << os.str() << std::endl;
	return os.str();
}
std::string Account::startSession()
{
	sessionId = generateId();
	loggedIn = true;
	return sessionId;
}
void Account::endSession()
{
	sessionId.clear();
	loggedIn = false;
}
bool Account::addAccess(const std::string &conferenceId, const AccessLevel permissions)
{
	std::pair< std::map<std::string, AccessLevel>::iterator, bool > ret;
	ret = accessMap.insert( std::pair<std::string, AccessLevel>(conferenceId, permissions) );
	return ret.second;
}
void Account::changeAccess(const std::string &conferenceId, const AccessLevel permissions)
{
	// alternatively:
	//accessMap[conferenceId] = permissions;
	std::map<std::string, AccessLevel>::iterator it;
	// update the row in the access map if it exists
	it = accessMap.find(conferenceId);
	if (it != accessMap.end())
	{
		it->second = permissions;
	}
	// if it does not exist, create one
	else
	{
		addAccess(conferenceId, permissions);
	}
}
Account::AccessLevel Account::getAccess(const std::string &conferenceId)
{
	std::map<std::string, AccessLevel>::iterator it;
	AccessLevel recordedLevel = Access_None;
	it = accessMap.find(conferenceId);
	if (it != accessMap.end())
	{
		recordedLevel = it->second;
	}
	return recordedLevel;
}
bool Account::hasAccess(const std::string &conferenceId)
{
	return (accessMap.find(conferenceId) != accessMap.end());
}
void Account::printAccess()
{
	std::cout << "User " << username << " has access to: " << std::endl;
	std::map<std::string, AccessLevel>::iterator it;
	for (it = accessMap.begin(); it != accessMap.end(); ++it)
	{
		std::cout << "Conference: " << it->first << " - Access Level: ";
		switch(it->second)
		{
			case Access_None:
				std::cout << "None";
			break;
			case Access_Author:
				std::cout << "Author";
			break;
			case Access_Reviewer:
				std::cout << "Reviewer";
			break;
			case Access_Chairman:
				std::cout << "Chairman";
			break;
			case Access_Admin:
				std::cout << "Admin";
			break;
		}
		std::cout << std::endl;
	}
}
void Account::setUsername(const std::string &aUser)
{
	username = aUser;
}
void Account::setPassword(const std::string &aPass)
{
	password = encrypt(aPass);
}

using namespace std;

int main ()
{
	//Clear and reset cursor
	cout << "\033[2J";
	cout << "\x1B[1;1f";

	Account myAccount;
	myAccount.setUsername("lol");
	myAccount.setPassword("3server5me");
	/*
	string test = ".asdhj$%^&*", pass = "?asdhj$%^&*";
	string enc = encrypt(test), passEnc = encrypt(pass);
	cout << "Encrypted: " << enc << endl;
	cout << "Pass: " << passEnc << endl;
	cout << (enc == passEnc ? "MATCHES" : "DOES NOT MATCH") << endl;
	*/
	
	//Clear and reset cursor
	cout << "\033[2J";
	
	string accOpt[3] = {
		"Login",
		"Register",
		"Exit"
	}; 
	
	Menu accMenu;
	accMenu.setOptions("Welcome", accOpt, 3);
	accMenu.setShowControls();
	
	string loginOpt[4] = {
		"Username: ",
		"Password: ",
		"Sign in",
		"Cancel"
	};
	
	Menu loginMenu;
	loginMenu.setOptions("Welcome > Login", loginOpt, 4);
	
	int accRes = -2, loginRes = -2;
	string tmpUser, tmpPass;
	while (accMenu.notExited(accRes))
	{
		accRes = accMenu.doMenu();
		switch(accRes)
		{
			// Logging in
			case 0:
				loginRes = -2;
				while (loginMenu.notExited(loginRes))
				{
					loginRes = loginMenu.doMenu();
					switch(loginRes)
					{
						// Enter username
						case 0:
							cout << "Enter username: ";
							getline(cin, tmpUser);
							loginOpt[0] = string("Username: ") + tmpUser;
							cout << "\033[2J";
						break;
						// Enter password
						case 1:
						{
							cout << "Enter password: ";
							getline(cin, tmpPass);
							string hidePass;
							hidePass.append(tmpPass.length(), '*');
							loginOpt[1] = string("Password: ") + hidePass;
							cout << "\033[2J";
						}
						break;
						case 2:
							cout << "Signing you in...";
							cin.ignore(1,'\n');
							if (myAccount.matchUsername(tmpUser) && myAccount.matchPassword(tmpPass, true))
							{
								cout << "Success!" << endl;
								cout << "Welcome, " << myAccount.getUsername() << endl;
								cin.ignore(1,'\n');
								loginRes = accRes = -1;
							}
							else
							{
								cout << "Failure!" << endl;
								cin.ignore(1, '\n');
							}
							cout << "\033[2J";
						break; 
						case 3: case -1:
						break;
					}
				}
			break;
			
			// Registering a new account
			case 1:
			{
				// put register menu here 
				Account newAccount;
				cin.ignore(1,'\n');
				cout << "\033[2J";
			}
			break;
			
			// Exiting
			case 2: case -1:
			break;
		}
	}
	
	//Clear and reset cursor
	cout << "\033[2J";
	cout << "\x1B[1;1f";
	
}

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
		result[i] = result[i] ^ static_cast<char>(sum * i ^ result[i] ^ result.length());
	}
	return result;
}

Account::Account()
{
	accountType = Account_User;
	loggedIn = false;
}
Account::Account(const Account &anAcc)
{
	username = anAcc.username;
	password = anAcc.password;
	loggedIn = anAcc.loggedIn;
	accessMap = anAcc.accessMap;
	accountType = anAcc.accountType;
	firstName = anAcc.firstName;
	lastName = anAcc.lastName;
	email = anAcc.email;
	university = anAcc.university;
	keywords = anAcc.keywords;
	allocatedMap = anAcc.allocatedMap;
}
Account::Account(const AccountType accType, const bool loginStatus)
{
	accountType = accType;
	loggedIn = loginStatus;
}

bool Account::matchPassword(const std::string &str, const bool doEncryption)
{
	if (doEncryption)
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
	return os.str();
}
void Account::startSession()
{
	loggedIn = true;
}
void Account::endSession()
{
	loggedIn = false;
}
// returns true if under the max
bool Account::checkAllocation(const std::string &id, const int max)
{
	std::map<std::string, int>::iterator it;
	it = allocatedMap.find(id);
	if (it == allocatedMap.end())
	{
		allocatedMap[id] = 0;
		return true;
	}
	return allocatedMap[id] < max;
}

bool Account::incrementAllocated(const std::string &id, const int max)
{
	if (checkAllocation(id, max))
	{
		allocatedMap[id] += 1;
		return true;
	}
	return false;
}
bool Account::addAccess(const std::string &conferenceId, const AccessLevel permissions)
{
	std::pair< std::map<std::string, AccessLevel>::iterator, bool > ret;
	ret = accessMap.insert( std::pair<std::string, AccessLevel>(conferenceId, permissions) );
	return ret.second;
}
void Account::changeAccess(const std::string &conferenceId, const AccessLevel permissions)
{
	if (permissions != Access_None)
	{
		accessMap[conferenceId] = permissions;
	}
	else
	{
		std::map<std::string, AccessLevel>::iterator it;	
		it = accessMap.find(conferenceId);
		if (it != accessMap.end())
		{
			accessMap.erase(it);
		}
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
void Account::getConferences(std::vector<std::string> &list)
{
	std::map<std::string, AccessLevel>::const_iterator it;
	for (it = accessMap.begin(); it != accessMap.end(); ++it)
	{
		list.push_back(it->first);
	}
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
void Account::clearAccess()
{
	accessMap.clear();
}
void Account::setUsername(const std::string &aUser)
{
	username = aUser;
}
void Account::setPassword(const std::string &aPass)
{
	password = encrypt(aPass);
}

// get/set details
void Account::setFirstName(const std::string &aFirst)
{
	firstName = aFirst;
}
std::string Account::getFirstName() const
{
	return firstName;
}
void Account::setLastName(const std::string &aLast)
{
	lastName = aLast;
}
std::string Account::getLastName() const
{
	return lastName;
}
void Account::setEmail (const std::string &anEmail) 
{ 
	email = anEmail; 
}
std::string Account::getEmail() const
{ 
	return email; 
}
void Account::setUniversity(const std::string &aUni) 
{ 
	university = aUni; 
}
std::string Account::getUniversity() const
{ 
	return university;
}
void Account::addKeyword(const std::string &word) 
{ 
	keywords.push_back(word); 
}
bool Account::findKeyword(const std::string &word) 
{ 
	return (std::find(keywords.begin(), keywords.end(), word) != keywords.end()); 
}
bool Account::matchKeywordsForSubmission(const Submission &sub)
{
	std::vector<std::string>::iterator it;
	for (it = keywords.begin(); it != keywords.end(); ++it)
	{
		if (sub.hasKeyword(*it))
		{
			return true;
		}
	}
	return false;
}
void Account::clearKeywords() 
{ 
	keywords.clear(); 
}
void Account::displayKeywords()
{
	std::vector<std::string>::iterator it;
	for (it = keywords.begin(); it != keywords.end(); ++it)
	{
		std::cout << "Keyword: " << *it << std::endl;
	}
}

Account & Account::operator=(const Account &acc)
{
	username = acc.username;
	password = acc.password;
	loggedIn = acc.loggedIn;
	accessMap = acc.accessMap;
	accountType = acc.accountType;
	firstName = acc.firstName;
	lastName = acc.lastName;
	email = acc.email;
	university = acc.university;
	keywords = acc.keywords;
	allocatedMap = acc.allocatedMap;
	return *this;
}

void Account::writeFile(std::ofstream &ofs) const
{
	appendString(ofs, this->username);
	appendString(ofs, this->password);
	appendString(ofs, this->firstName);
	appendString(ofs, this->lastName);
	appendString(ofs, this->email);
	appendString(ofs, this->university);
	appendData<bool>(ofs, this->loggedIn);
	appendData<AccountType>(ofs, this->accountType);
	appendStringVector(ofs, this->keywords);
	appendStringKeyMap<AccessLevel>(ofs, this->accessMap);
	appendStringKeyMap<int>(ofs, this->allocatedMap);
}
void Account::readFile(std::ifstream &ifs)
{
	readString(ifs, this->username);
	readString(ifs, this->password);
	readString(ifs, this->firstName);
	readString(ifs, this->lastName);
	readString(ifs, this->email);
	readString(ifs, this->university);
	readData<bool>(ifs, this->loggedIn);
	readData<AccountType>(ifs, this->accountType);
	readStringVector(ifs, this->keywords);
	readStringKeyMap<AccessLevel>(ifs, this->accessMap);
	readStringKeyMap<int>(ifs, this->allocatedMap);
}

sf::Packet & operator<<(sf::Packet &packet, const Account::AccessLevel &level)
{
	return packet << static_cast<int>(level);
}

sf::Packet & operator>>(sf::Packet &packet, Account::AccessLevel &level)
{
	int tmp;
	packet >> tmp;
	level = static_cast<Account::AccessLevel>(tmp);
	return packet;
}


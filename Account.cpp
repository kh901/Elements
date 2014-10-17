#include "Account.h"

template<class T>
T Max(const T &a, const T &b)
{
	return (a > b ? a : b);
}

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
	generateId();
}

bool Account::matchPassword(const std::string &str)
{
	return password == str;
}

void Account::generateId()
{
	std::ostringstream os;
	srand(time(NULL));
	for (int i = 0; i < 9; ++i)
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
	this->uniqueId = os.str();
}

using namespace std;

int main ()
{
	//Clear and reset cursor
	cout << "\033[2J";
	cout << "\x1B[1;1f";

	
	string test = ".asdhj$%^&*", pass = "?asdhj$%^&*";
	string enc = encrypt(test), passEnc = encrypt(pass);
	cout << "Encrypted: " << enc << endl;
	cout << "Pass: " << passEnc << endl;
	cout << (enc == passEnc ? "MATCHES" : "DOES NOT MATCH") << endl;
	
	cin.ignore(1, '\n');
	
	//Clear and reset cursor
	cout << "\033[2J";
	
	string accOpt[3] = {
		"Login",
		"Register",
		"Exit"
	}; 
	
	Menu accMenu;
	accMenu.setOptions("Welcome", accOpt, 3);
	
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
							for (int i = 0; i < (int)tmpPass.length(); ++i)
							{
								hidePass += '*';
							}
							loginOpt[1] = string("Password: ") + hidePass;
							cout << "\033[2J";
						}
						break;
						case 2:
							cout << "Signing you in...";
							cin.ignore(1,'\n');
							if (encrypt(tmpPass) == encrypt(string("passyo")))
							{
								cout << "Success!" << endl;
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


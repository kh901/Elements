#include<iostream>
#include<SFML/Network.hpp>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

void level1();
void level2();
void level3();
void level4();
void level5();

int main(){

	Packet login;
	Packet validate;
	size_t received;
	string username;
	string password;
	int value = 0;
	bool valid = false;

	srand(time(NULL));

	sf::TcpListener listener;
	
	if(listener.listen(60000)!=sf::Socket::Done){
	
		cout<<"error"<<endl;
		
	}
	
	sf::TcpSocket client;

	if(listener.accept(client)== sf::Socket::Done){
		client.receive(login);
		login >> username >> password;
		value = rand() % 1;
		if (value == 0)
		{
			valid = false;
		}
		else if (value == 1)
		{
			valid = true;
		}
		else
		{
			return -1;
		}
		
		validate << valid;
		client.send(validate);

		/*switch (level)
		{
			case 1: level1();
				break;
			case 2: level2();
				break;
			case 3: level3();
				break;
			case 4: level4();
				break;
			case 5: level5();
				break;
			default: break;
		}*/
	
	}
	
	return 0;
}

void level1()
{
	cout << "The User has account permission level 1" << endl;
}

void level2()
{
	cout << "The User has account permission level 2" << endl;
}

void level3()
{
	cout << "The User has account permission level 3" << endl;
}

void level4()
{
	cout << "The User has account permission level 4" << endl;
}

void level5()
{
	cout << "The User has account permission level 5" << endl;
}

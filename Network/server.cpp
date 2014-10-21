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

	Packet login,validate;
	string username,password;
	int value = 0;
	bool valid = false;
	size_t received;

	srand(time(NULL));

	sf::TcpListener listener;
	sf::TcpSocket client;
	
	if(listener.listen(60000)!= Socket::Done)
		cout<<"error"<<endl;
	


	if(listener.accept(client)== sf::Socket::Done){
	
		while(valid==false){
		
			client.receive(login);
			login >> username >> password;
			value = rand() % 1 + 2;
			cout<<"Value: "<<value<<endl;
			if (value == 1)
				valid = false;
			else if (value == 2)
				valid = true;
		
			validate << valid;
		
			client.send(validate);
		}
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

#include<iostream>
#include<SFML/Network.hpp>

using namespace std;

void level1();
void level2();
void level3();
void level4();
void level5();

int main(){

	size_t received;
	string username;
	string password;
	int level;

	sf::TcpListener listener;
	
	if(listener.listen(60000)!=sf::Socket::Done){
	
		cout<<"error"<<endl;
		
	}
	
	sf::TcpSocket client;

	//char data[100] = "hello world";
	if(listener.accept(client)== sf::Socket::Done){
		char data[100] = "Username: ";
		client.send(data,100);
		client.receive(data,100,received);
		data[100] = "Password: ";
		client.send(data,100);
		client.receive(data,100,received);
		data[100] = "Level: ";
		client.send(data,100);
		client.receive(login);
		login >> username >> password >> level;
		switch (level)
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
		}
	
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

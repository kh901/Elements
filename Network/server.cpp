#include<iostream>
#include<SFML/Network.hpp>

using namespace std;

void function1();
void function2();
void function3();
void function4();
void function5();

int main(){

	int option[1];
	size_t received;

	sf::TcpListener listener;
	
	if(listener.listen(60000)!=sf::Socket::Done){
	
		cout<<"error"<<endl;
		
	}
	
	sf::TcpSocket client;

	//char data[100] = "hello world";
	char data[100] = "Please choose a number between 1 and 5";
	if(listener.accept(client)== sf::Socket::Done){
	
		client.send(data,100);
		client.receive(option, 1, received);
		int option_temp = option[0];
		switch (option_temp)
		{
			case 1: function1();
				break;
			case 2: function2();
				break;
			case 3: function3();
				break;
			case 4: function4();
				break;
			case 5: function5();
				break;
			default: break;
		}
	
	}
	
	return 0;
}

void function1()
{
	cout << "Called Function 1" << endl;
}

void function2()
{
	cout << "Called Function 2" << endl;
}

void function3()
{
	cout << "Called Function 3" << endl;
}

void function4()
{
	cout << "Called Function 4" << endl;
}

void function5()
{
	cout << "Called Function 5" << endl;
}

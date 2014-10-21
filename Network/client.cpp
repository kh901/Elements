#include<iostream>
#include<SFML/Network.hpp>
#include<cstring>
#include<SFML/System/Time.hpp>

using namespace std;
using namespace sf;	

bool loginHandler();

int main(){

	Packet protocol,login;
	string key;

	string username;
	string password;
	
	int option;

	TcpSocket socket;
	
	Time time = seconds(7);
	
	Socket::Status status = socket.connect("localhost",60000,time);
	
	if(status == Socket::Done){

		cout << "Login (1)" << endl;
		cout << "Register (2)" << endl;
		cout << "Exit (3)" << endl;
		cin >> option;

		if (option == 1)
		{
			key = "LOGIN";
			protocol << key;
			socket.send(protocol);
			loginHandler();
		}
		else if (option == 2)
		{
			key = "REGISTER";
			protocol << key;
			socket.send(protocol);
		}
		else
		{
			cout << "TOPLEL" << endl;
			return -1;
		}
			

	}
	
	return 0;
}

bool loginHandler(){

	bool valid = false;
	Packet response;
	
	while(valid==false){
	
		cout<<"Username: ";
		cin>>username;

		cout<<"Password: ";
		cin>>password;
	
		login << username << password;
		socket.send(login);
		socket.receive(response);
		response >> valid;
	}

}



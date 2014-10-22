#include<iostream>
#include<vector>
#include<SFML/Network.hpp>
#include<cstring>
#include<SFML/System/Time.hpp>

using namespace std;
using namespace sf;	

void loginHandler(TcpSocket &, Packet &);
void handleRegister(TcpSocket &, Packet &);

int main(){

	Packet request;
	string protocol;
	
	int option;

	TcpSocket socket;
	
	Time time = seconds(7);
	
	Socket::Status status = socket.connect("localhost",60000,time);
	
	if(status == Socket::Done){

		cout << "Login (1)" << endl;
		cout << "Register (2)" << endl;
		cout << "Exit (3)" << endl;
		cin >> option;
		cin.ignore(1, '\n');

		if (option == 1)
		{
			protocol = "LOGIN";
			request << protocol;
			loginHandler(socket, request);
		}
		else if (option == 2)
		{
			protocol = "REGISTER";
			request << protocol;
			handleRegister(socket, request);
		}
		else
		{
			cout << "TOPLEL" << endl;
			return -1;
		}
	}
	
	return 0;
}

void handleRegister(TcpSocket &socket, Packet &request)
{
	Packet response;
	bool exists = true;
	string protocol = "REGISTER";	
	
	string username, password;
	string firstname, lastname;
	string email, uni;
	vector<string> keywords;
	
	string tmp;
	
	while(exists == true)
	{
		cout << "Username: ";
		cin >> username;
		cin.ignore(1, '\n');
		
		cout << "Password: ";
		cin >> password;
		cin.ignore(1, '\n');
		
		cout << "Firstname: ";
		cin >> firstname;
		cin.ignore(1, '\n');
		
		cout << "Lastname: ";
		cin >> lastname;
		cin.ignore(1, '\n');
		
		cout << "Email: ";
		cin >> email;
		cin.ignore(1, '\n');
		
		cout << "Uni: ";
		cin >> uni;
		cin.ignore(1, '\n');
		
		cout << "Keywords (when finished, type 'stop'): " << endl;
		while(getline(cin, tmp), tmp != "stop")
		{
			keywords.push_back(tmp);
		}
		
		request << username << password;
		request << firstname << lastname;
		request << email << uni;
		request << (int) keywords.size();
		for (int i = 0; i < (int)keywords.size(); ++i)
		{
			request << keywords[i];
		}
		
		socket.send(request);
		socket.receive(response);
		response >> exists;
		
		if (exists)
		{
			cout << "That account already exists" << endl;
			request.clear();
			request << protocol;
		}
		keywords.clear();
	}
	cout << "You have been registered" << endl;
}

void loginHandler(TcpSocket &socket, Packet &request){
	Packet response;
	bool valid = false;
	
	string username;
	string password;
	string protocol = "LOGIN";
	
	while(valid==false){
	
		cout<<"Username: ";
		cin>>username;
		cin.ignore(1, '\n');
		
		cout<<"Password: ";
		cin>>password;
		cin.ignore(1, '\n');
	
		request << username << password;
		socket.send(request);
		socket.receive(response);
		response >> valid;
		if (valid == false)
		{
			cout << "Invalid details" << endl;
		}
		request.clear();
		request << protocol;
	}
}



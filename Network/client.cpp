#include<iostream>
#include<vector>
#include<SFML/Network.hpp>
#include<SFML/System/Time.hpp>
#include "../Account.h"
#include "../Submission.h"

using namespace std;
using namespace sf;	

string loginHandler(TcpSocket &, Packet &);
string handleRegister(TcpSocket &, Packet &);
string handlePickConference(TcpSocket &, const std::string &, Account::AccessLevel &);
void handleSubmissions(TcpSocket &,Packet &);
void showFirstScreen(TcpSocket&,string &);
void submitHandler(TcpSocket &, Packet &);

int main(){

	Packet request;
	string protocol;
	
	string loggedInUser, conference;
	Account::AccessLevel level;
	
	int option;
	TcpSocket socket;
	
	Time time = seconds(7);
	
	string serverAddress;
	cout << "Enter server address: ";
	getline(cin, serverAddress);
	
	Socket::Status status = socket.connect(serverAddress.c_str(),60000,time);
	
	if(status == Socket::Done)
	{
		bool exit = false;

		showFirstScreen(socket,loggedInUser);
	
		conference = handlePickConference(socket, loggedInUser, level);
		if (conference != "None")
		{
			cout << "You're in " << conference << endl;
			cout << "Your level is: " << level << endl;
		}

		cout << "Main Menu" << endl;
		cout << "Submit report(1)" <<endl;
		cout << "View Submissions(2)" << endl;
		cin >> option;
		cin.ignore(1, '\n');

		request.clear();

		if (option == 1)
		{
			protocol = "SUBMIT_PAPER";
			request << protocol;
			submitHandler(socket, request);
		}
		else if (option == 2)
		{
			protocol = "VIEW_SUBMISSIONS";
			request << protocol << loggedInUser;
			handleSubmissions(socket,request);
		}
	}
	return 0;
}

void submitHandler(TcpSocket &socket, Packet &request)
{
	Packet response;
	Submission submission;
	string response_title;
	submission.submit();
	request << submission;
	
	socket.send(request);
}

void showFirstScreen(TcpSocket& socket,string &loggedInUser)
{
	cout << "Login (1)" << endl;
	cout << "Register (2)" << endl;
	cout << "Exit (3)" << endl;
	int option;
	string protocol;
	Packet request;
	cin >> option;
	cin.ignore(1, '\n');

	if (option == 1)
	{
		protocol = "LOGIN";
		request << protocol;
		loggedInUser = loginHandler(socket, request);
	}
	else if (option == 2)
	{
		protocol = "REGISTER";
		request << protocol;
		loggedInUser = handleRegister(socket, request);
	}
	else
	{
		cout << "TOPLEL" << endl;
		return;
	}
}

void handleSubmissions(TcpSocket &socket, Packet &request)
{
	socket.send(request);
	Packet response;
	socket.receive(response);		//recieve the submissons + number of submissions
	
	int amountOfSubmissions = 0;
	response >> amountOfSubmissions;
	string temp;
	vector<string> subList;
	
	if(amountOfSubmissions==0)
	{
		cout<< "You have not submitted a paper yet!" <<endl; 
	}
	else
	{
		for(int i=0;i<amountOfSubmissions;i++)
		{
			response >> temp;
			subList.push_back(temp);
			cout << i+1 << ". " << temp << endl;
		}	
	}
	
	cout<< "Press enter to go back to Submission Menu" <<endl;
	cin.ignore(1, '\n');
}

string handlePickConference(TcpSocket &socket, const std::string &user, Account::AccessLevel &access)
{
	Packet request, response;
	string requestProtocol = "CONFERENCE_LIST";
	request << requestProtocol << user;
	
	vector<string> conferences;
	int conferenceSize = 0;
	
	// send that off to the server
	socket.send(request);
	
	// wait for the reply
	socket.receive(response);
	response >> conferenceSize;
	string tmpconference;
	for (int i = 0; i < conferenceSize; ++i)
	{
		response >> tmpconference;
		conferences.push_back(tmpconference);
	}
	
	// display!
	// let the user pick a conference
	// save that conference so we can use it later in the conference menu
	if (conferenceSize == 0) 
	{
		cout << "No conferences!" << endl;
		return "None";
	}
	int pick = 0;
	do 
	{
		cout << "Choose a conference: " << endl;
		for (int c = 0; c < conferenceSize; c++)  // :)
		{
			cout << c+1 << ". " << conferences[c] << endl;
		}
		cin >> pick;
		cin.ignore(1, '\n');
	} while (pick < 1 || pick > conferenceSize);
	
	// get the user's access level from the server
	Packet accessPacket;
	requestProtocol = "CONFERENCE_ACCESS";
	accessPacket << requestProtocol << user << conferences[pick-1];
	socket.send(accessPacket);
	response.clear();
	socket.receive(response);
	response >> access;
	
	return conferences[pick-1];
}

string handleRegister(TcpSocket &socket, Packet &request)
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
	return username;
}

string loginHandler(TcpSocket &socket, Packet &request)
{
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
	return username;
}



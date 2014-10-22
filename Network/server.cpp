#include<iostream>
#include<SFML/Network.hpp>
#include"../Account.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>

using namespace std;
using namespace sf;

void level1();
void level2();
void level3();
void level4();
void level5();
int checkAccount(vector<Account> &,string,string, bool usernameOnly = false);
void loginAccount(Packet&, TcpSocket&,vector<Account>&);
bool registerAccount(Packet&, TcpSocket&,vector<Account>&);
void processClient(Packet&, TcpSocket&, vector<Account>&);
void getConferences(Packet&, TcpSocket&, vector<Account>&);
void getAccess(Packet&, TcpSocket&, vector<Account>&);
void loadFalseAccounts(vector<Account> &acc){
	Account author;
	author.setUsername("Adam");
	author.setPassword("pass");
	author.addAccess("AIDS conference", Account::Access_Author);
	author.addAccess("Zombie apocalypse conference", Account::Access_Admin);
	
	Account reviewer;
	reviewer.setUsername("Jonathan");
	reviewer.setPassword("pass");
	reviewer.addAccess("AIDS conference", Account::Access_Reviewer);
	
	Account admin;
	admin.setUsername("Kieran");
	admin.setPassword("pass");
	admin.addAccess("AIDS conference", Account::Access_Admin);
		
	acc.push_back(author);
	acc.push_back(reviewer);
	acc.push_back(admin);
}

int main(){
	
	vector<Account> accounts;
	loadFalseAccounts(accounts);
	
	TcpListener listener;
	TcpSocket client;
	
	list<TcpSocket*> clients;
	
	SocketSelector selector;
	
	if(listener.listen(60000)!= Socket::Done)
		cout<<"error"<<endl;
		
	selector.add(listener);
	
	for(;;){
	
		 if (selector.wait()){
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				TcpSocket* client = new TcpSocket;
				if (listener.accept(*client) == Socket::Done)
				{
					cout << "New connection from " << client->getRemoteAddress().toString() << endl;
					// Add the new client to the clients list
					clients.push_back(client);
					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*client);
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					delete client;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				std::list<sf::TcpSocket*>::iterator end = clients.end();
				for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != end; ++it)
				{
					sf::TcpSocket& client = **it;
					if (selector.isReady(client))
					{
					// The client has sent some data, we can receive it
						sf::Packet packet;
						sf::Socket::Status status;
						status = client.receive(packet);
						if (status == sf::Socket::Done)
						{
							cout << "Processing client" << endl;
							processClient(packet, client, accounts);
							packet.clear();
						}
						else if (status == sf::Socket::Disconnected)
						{
							cout << "Client disconnected" << endl;
							packet.clear();
							
							sf::TcpSocket * del = *it;
							clients.erase(it);
							selector.remove(**it);
							delete del;
							it = clients.begin();
							end = clients.end();
						}			
					}
				}
			}
		}
	}
	
	return 0;
}

void processClient(Packet &packet, TcpSocket &client, vector<Account> &accounts)
{
	string protocol;
		
	packet >> protocol;
		
	cout << "Protocol " << protocol << endl;
		
	if(protocol=="LOGIN"){
		loginAccount(packet, client,accounts);
	}
	else if(protocol=="REGISTER"){
		registerAccount(packet, client, accounts);
	}
	else if(protocol=="CONFERENCE_LIST"){
		getConferences(packet, client, accounts);
	}
	else if(protocol=="CONFERENCE_ACCESS"){
		getAccess(packet, client, accounts);
	}
	else if(protocol=="STUB3"){


	}
	else if(protocol=="STUB4"){


	}
	else if(protocol=="BYE"){
		

	}
}

void getAccess(Packet &packet, TcpSocket &socket, vector<Account> &accounts)
{
	Packet accessDetails;
	string user, tmp = "test", conference;
	packet >> user >> conference;
	
	int findIndex = checkAccount(accounts, user, tmp, true);
	Account::AccessLevel level;
	if (findIndex != -1)
	{
		level = accounts[findIndex].getAccess(conference);
		accessDetails << level;
		socket.send(accessDetails);
	}
}

void getConferences(Packet &packet, TcpSocket &socket, vector<Account> &accounts)
{
	Packet list;
	string user, tmp = "test";
	packet >> user;
	
	int findIndex = checkAccount(accounts, user, tmp, true);
	vector<string> results;
	if (findIndex != -1)
	{
		accounts[findIndex].getConferences(results);
		list << (int) results.size();
		for (int i = 0; i < (int)results.size(); ++i)
		{
			list << results[i];
		}
		socket.send(list);
	}
}

int checkAccount(vector<Account> &accounts,string username,string password, bool usernameOnly){
	for(int i=0;i<accounts.size();i++){
		if(accounts[i].matchUsername(username))
		{
			if (usernameOnly || accounts[i].matchPassword(password))
			{
				return i;
			}
		}
	}
	return -1;
}

void loginAccount(Packet &packet, TcpSocket &client,vector<Account> &accounts){

	Packet validate;
	string username,password;
	
	bool valid = false;
	
	packet >> username >> password;
	int findIndex = checkAccount(accounts,username,password);
	
	if (findIndex != -1) 
	{
		valid = true;
	}
	
	if (valid)
	{
		// set the user as logged in
		accounts[findIndex].startSession();
	}
	validate << valid;
	client.send(validate);
}

bool registerAccount(Packet &packet, TcpSocket &client,vector<Account> &accounts){
	Packet existsPacket;
	string username,password,email,university, firstname, lastname;
	vector<string> keywords;
	int keywordSize = 0;

	packet >> username >> password >> email >> university >> keywordSize;
	string tmpkeyword;
	for (int i = 0; i < keywordSize; ++i)
	{
		packet >> tmpkeyword;
		keywords.push_back(tmpkeyword);
	}
	
	bool exists = false;
	int index = checkAccount(accounts, username, password, true);
	if (index == -1)
	{
		cout << "New user registered! Welcome " << username << endl;
		Account tmp;
		tmp.setUsername(username);
		tmp.setPassword(password);
		tmp.setFirstName(firstname);
		tmp.setLastName(lastname);
		tmp.setEmail(email);
		tmp.setUniversity(university);
		for (int i = 0; i < keywordSize; ++i)
		{
			tmp.addKeyword(keywords[i]);
		}
		// registered users start logged in
		tmp.startSession();			
		accounts.push_back(tmp);
	}
	else
	{
		exists = true;
	}
	existsPacket << exists;
	client.send(existsPacket);
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

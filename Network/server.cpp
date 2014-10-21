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
bool checkAccount(vector<Account>,string,string);
bool loginAccount(TcpSocket&,vector<Account>);
bool registerAccount();
void processClient(TcpSocket&, vector<Account>);


vector<Account> loadFalseAccounts(){
	vector<Account> temp;
	
	Account author;
	author.setUsername("Adam");
	author.setPassword("pass");
	author.addAccess("AIDS conference", Account::Access_Author);
	
	
	Account reviewer;
	reviewer.setUsername("Jonthan");
	reviewer.setPassword("pass");
	reviewer.addAccess("AIDS conference", Account::Access_Reviewer);
	
	
	Account admin;
	admin.setUsername("Kieran");
	admin.setPassword("pass");
	admin.addAccess("AIDS conference", Account::Access_Admin);
	
	
	temp.push_back(author);
	temp.push_back(reviewer);
	temp.push_back(admin);



	return temp;
}



int main(){


	

	
	vector<Account> accounts = loadFalseAccounts();
	
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
				for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					sf::TcpSocket& client = **it;
					if (selector.isReady(client))
					{
					// The client has sent some data, we can receive it
						sf::Packet packet;
						if (client.receive(packet) == sf::Socket::Done)
						{
							processClient(client, accounts);
						}
						
					
					}
				}
			}
		}
	}


	
	
	return 0;
}

void processClient(TcpSocket &client, vector<Account> accounts)
{
	Packet entryPacket;
	string protocol;
	client.receive(entryPacket);
		
	entryPacket >> protocol;
		
	if(protocol=="LOGIN"){
		loginAccount(client,accounts);
	}
	else if(protocol=="REGISTER"){
		registerAccount();
	}
	else if(protocol=="STUB1"){


	}
	else if(protocol=="STUB2"){


	}
	else if(protocol=="STUB3"){


	}
	else if(protocol=="STUB4"){


	}
	else if(protocol=="BYE"){
		

	}
}

bool checkAccount(vector<Account> accounts,string username,string password){
	
	for(int i=0;i<accounts.size();i++){
		if(accounts[i].matchUsername(username) && accounts[i].matchPassword(password))
			return true;
	}
	return false;
}

bool loginAccount(TcpSocket&client,vector<Account> accounts){

	Packet login,validate;
	string username,password;
	
	bool valid = false;

	while(valid==false){

		client.receive(login);
		login >> username >> password;
		valid = checkAccount(accounts,username,password);

		validate << valid;

		client.send(validate);
	}

}

bool registerAccount(){

	string username,password,email,university;
	

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

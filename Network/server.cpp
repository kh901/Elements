#include<iostream>
#include<SFML/Network.hpp>
#include"../Account.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
using namespace sf;

void level1();
void level2();
void level3();
void level4();
void level5();
bool checkAccount(vector<Account>,string,string);
bool loginAccount();


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


	Packet entryPacket;
	string protocol;
	
	vector<Account> accounts;
	accounts = loadFalseAccounts();
	
	sf::TcpListener listener;
	
	
	if(listener.listen(60000)!= Socket::Done)
		cout<<"error"<<endl;
		
	sf::TcpSocket client;
	if(listener.accept(client)== sf::Socket::Done){
	
		client.receive(entryPacket);
		
		entryPacket >> protocol;
		
		if(protocol=="LOGIN"){
			loginAccount();
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
		else if(protocol=="STUB5"){
		
		
		}
	


	}
	
	return 0;
}

bool checkAccount(vector<Account> accounts,string username,string password){
	
	for(int i=0;i<accounts.size();i++){
		if(accounts[i].matchUsername(username) && accounts[i].matchPassword(password))
			return true;
	}
	return false;
}

bool loginAccount(){

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

	string username,password,email,university
	

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

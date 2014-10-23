#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "SFML/Network.hpp"
#include "Account.h"
#include "Submission.h"

#ifndef SERVER_CONTROLLER
#define SERVER_CONTROLLER

class ServerController
{
	public:
		ServerController();//
		void run();//
		void loadFalseAccounts();//
		void loadFalseSubmissions();//
		int checkAccount(std::string, std::string = "#USERNAMEONLY");//
		void loginAccount(sf::Packet&, sf::TcpSocket&);//
		bool registerAccount(sf::Packet&, sf::TcpSocket&);//
		void processClient(sf::Packet&, sf::TcpSocket&);//
		void getConferences(sf::Packet&, sf::TcpSocket&);//
		void getAccess(sf::Packet&, sf::TcpSocket&);//
		void getSubmissions(sf::Packet&, sf::TcpSocket&);//
	private:
		std::list<sf::TcpSocket*> clients;
		sf::SocketSelector selector;
		std::vector<Account> accounts;
		std::vector<Submission> submissions;
};

#endif

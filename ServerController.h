#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "SFML/Network.hpp"
#include "Account.h"
#include "Conference.h"
#include "Submission.h"

#ifndef SERVER_CONTROLLER
#define SERVER_CONTROLLER

class ServerController
{
	public:
		ServerController();//
		void run();//
	private:
		std::list<sf::TcpSocket*> clients;
		sf::SocketSelector selector;
		std::vector<Account> accounts;
		std::vector<Submission> submissions;
		std::vector<Conference> conferences;
	
		void loadFalseAccounts();//
		//void loadFalseSubmissions();//
		void loginAccount(sf::Packet&, sf::TcpSocket&);//
		bool registerAccount(sf::Packet&, sf::TcpSocket&);//
		void processClient(sf::Packet&, sf::TcpSocket&);//
		void getConferences(sf::Packet&, sf::TcpSocket&);//
		void getAccess(sf::Packet&, sf::TcpSocket&);//
		void getSubmissions(sf::Packet&, sf::TcpSocket&);//
		void paperSubmission(sf::Packet&, sf::TcpSocket&);
		void getAdminStatus(sf::Packet&, sf::TcpSocket&);
		void createConference(sf::Packet&, sf::TcpSocket&);
		void logoutUser(sf::Packet&, sf::TcpSocket&);
		int checkAccount(std::string, std::string = "#USERNAMEONLY");//
		bool conferenceExists(const std::string &);
};

#endif

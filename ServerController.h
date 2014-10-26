#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include "SFML/Network.hpp"
#include "Account.h"
#include "Conference.h"
#include "Submission.h"
#include "common.h"

#ifndef SERVER_CONTROLLER
#define SERVER_CONTROLLER

typedef std::map< std::string, std::deque<std::string> > NotifyMap;

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
		std::vector<std::string> deadlineSubmissions;
		bool deadlineSet;
		time_t deadline;
		NotifyMap notifications;
	
		void loadFalseAccounts();//
		void loadFalseConferences();
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
		void getNotifications(sf::Packet&, sf::TcpSocket&);
		void advancePhase(sf::Packet&, sf::TcpSocket&);
		void bidList(sf::Packet&, sf::TcpSocket&);
		
		void addNotification(const std::string &user, const std::string &str);
		void clearNotifications(const std::string &user);
		
		int checkSubmission(const std::string &, const std::string &);
		int checkAccount(std::string, std::string = "#USERNAMEONLY");//
		bool conferenceExists(const std::string &);
		void checkPhase(sf::Packet&, sf::TcpSocket&);
		int checkConference(std::string);
		void bidPaper(sf::Packet&, sf::TcpSocket&);
		void notifyConference(const std::string &, const std::string &);
		void autoAllocate();
		void allocate(const std::string &);
		void getAllocations(sf::Packet&, sf::TcpSocket&);
};

#endif

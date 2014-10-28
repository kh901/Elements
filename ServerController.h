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
#include "Review.h"

#ifndef SERVER_CONTROLLER
#define SERVER_CONTROLLER

#define MAXIMUM_USEABLEINT_LIMIT 256000

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
		std::vector<Review> reviews;
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
		void getConferenceSubs(sf::Packet&, sf::TcpSocket&);
		void getReviewList(sf::Packet&, sf::TcpSocket&);
		void sendSubDetail(sf::Packet&, sf::TcpSocket&);
		void addMember(sf::Packet&, sf::TcpSocket&, Account::AccessLevel);
		void getAccountName(sf::Packet&, sf::TcpSocket&);
		void getReview(sf::Packet&, sf::TcpSocket&);
		void changeLimit(sf::Packet&, sf::TcpSocket&, const std::string &);
		void getLimit(sf::Packet&, sf::TcpSocket&, const std::string &);
		void checkNotifyCount(sf::Packet&, sf::TcpSocket&);
		
		void addNotification(const std::string &user, const std::string &str);
		int getNotificationCount(const std::string &user);
		void clearNotifications(const std::string &user);
		
		int checkSubmission(const std::string &, const std::string &);
		int checkAccount(std::string, std::string = "#USERNAMEONLY");//
		bool conferenceExists(const std::string &);
		void checkPhase(sf::Packet&, sf::TcpSocket&);
		int checkConference(std::string);
		void bidPaper(sf::Packet&, sf::TcpSocket&);
		void notifyConference(const std::string &, const std::string &, Account::AccessLevel = Account::Access_Author);
		void autoAllocate();
		void allocate(const std::string &);
		void getAllocations(sf::Packet&, sf::TcpSocket&);
		void submitReview(sf::Packet&, sf::TcpSocket&);
		void getComments(sf::Packet&, sf::TcpSocket&);
		void sendComments(sf::Packet&, sf::TcpSocket&);
};

#endif

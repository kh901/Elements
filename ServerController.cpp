#include "ServerController.h"

int main()
{
	ServerController servercontroller;
	servercontroller.run();
	return 0;
}

ServerController::ServerController()
{

}

void ServerController::paperSubmission(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	Submission submission;
	std::string username;
	
	packet >> username;
	packet >> submission;
	submissions.push_back(submission);
	
	std::cout << "Submitted paper: " << submission.getTitle() << " by " << username << std::endl;
}

void ServerController::run()
{
	loadFalseAccounts();
	//loadFalseSubmissions();
	
	sf::TcpListener listener;
	
	if(listener.listen(60000)!= sf::Socket::Done)
		std::cout<<"error"<<std::endl;
		
	selector.add(listener);
	
	for(;;){
	
		 if (selector.wait()){
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done)
				{
					std::cout << "New connection from " << client->getRemoteAddress().toString() << std::endl;
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
							std::cout << "Processing client" << std::endl;
							processClient(packet, client);
							packet.clear();
						}
						// The client has disconnected, stop listening to them
						else if (status == sf::Socket::Disconnected)
						{
							std::cout << "Client disconnected" << std::endl;
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
}

void ServerController::loadFalseAccounts(){
	Account author;
	author.setUsername("Adam");
	author.setPassword("pass");
	author.setFirstName("Adam");
	author.setLastName("Dahler");
	author.addAccess("AIDS conference", Account::Access_Author);
	author.addAccess("Zombie apocalypse conference", Account::Access_Admin);
	
	Account reviewer;
	reviewer.setUsername("Jonathan");
	reviewer.setPassword("pass");
	reviewer.setFirstName("Jonathan");
	reviewer.setLastName("Yip");
	reviewer.addAccess("AIDS conference", Account::Access_Reviewer);
	addNotification("Jonathan", "Hai, u da one");
	addNotification("Jonathan", "oh no, you get the second?");
	addNotification("Jonathan", "aww, now you have 3. :{D");
	addNotification("Jonathan", "spam");
	
	Account admin;
	admin.setUsername("Kieran");
	admin.setPassword("pass");
	admin.setFirstName("Kieran");
	admin.setLastName("Haavisto");
	admin.setSystemAdmin();
	admin.addAccess("AIDS conference", Account::Access_Admin);
		
	accounts.push_back(author);
	accounts.push_back(reviewer);
	accounts.push_back(admin);
}

/*void ServerController::loadFalseSubmissions(){
	Submission temp1,temp2,temp3;
	temp1.submit();
	temp2.submit();
	temp3.submit();
	
	submissions.push_back(temp1);
	submissions.push_back(temp2);
	submissions.push_back(temp3);
}*/

int ServerController::checkConference(std::string conference)
{
	for (int i = 0; i < (int)conferences.size(); i++)
	{
		if (conferences[i].getName() == conference)
		{
			return i;
		}
	}
	return -1;
}

int ServerController::checkAccount(std::string username, std::string password){
	for(int i=0;i<accounts.size();i++){
		if(accounts[i].matchUsername(username))
		{
			if (password=="#USERNAMEONLY" || accounts[i].matchPassword(password))
			{
				return i;
			}
		}
	}
	return -1;
}

void ServerController::loginAccount(sf::Packet &packet, sf::TcpSocket &client){

	sf::Packet validate;
	std::string username, password;
	
	bool valid = false;
	
	packet >> username >> password;
	int findIndex = checkAccount(username,password);
	
	if (findIndex != -1) 
	{
		valid = true;
	}
	
	if (valid)
	{
		// set the user as logged in
		accounts[findIndex].startSession();
		std::cout << username << " has logged in" << std::endl;
	}
	validate << valid;
	client.send(validate);
}

bool ServerController::registerAccount(sf::Packet &packet, sf::TcpSocket &client){
	sf::Packet existsPacket;
	std::string username,password,email,university, firstname, lastname;
	std::vector<std::string> keywords;
	int keywordSize = 0;

	packet >> username >> password >> firstname >> lastname >> email >> university >> keywordSize;
	std::string tmpkeyword;
	for (int i = 0; i < keywordSize; ++i)
	{
		packet >> tmpkeyword;
		keywords.push_back(tmpkeyword);
	}
	
	bool exists = false;
	int index = checkAccount(username);
	if (index == -1)
	{
		std::cout << "New user registered! Welcome " << username << std::endl;
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

void ServerController::processClient(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string protocol;
		
	packet >> protocol;
		
	std::cout << "Protocol " << protocol << std::endl;
		
	if(protocol=="LOGIN"){
		loginAccount(packet, client);
	}
	else if(protocol=="REGISTER"){
		registerAccount(packet, client);
	}
	else if(protocol=="CONFERENCE_LIST"){
		getConferences(packet, client);
	}
	else if(protocol=="CONFERENCE_ACCESS"){
		getAccess(packet, client);
	}
	else if(protocol=="VIEW_SUBMISSIONS"){
		getSubmissions(packet, client);
	}
	else if(protocol=="SUBMIT_PAPER"){
		paperSubmission(packet, client);
	}
	else if (protocol=="ADMIN_STATUS"){
		getAdminStatus(packet, client);
	}
	else if (protocol=="CREATE_CONFERENCE"){
		createConference(packet, client);
	}
	else if (protocol=="GET_NOTIFICATIONS"){
		getNotifications(packet, client);
	}
	else if (protocol=="CHECK_PHASE"){
		checkPhase(packet, client);
	}
	else if (protocol=="BID_PAPER"){
		bidPaper(packet, client);
	}
	else if(protocol=="BYE"){
		logoutUser(packet, client);
	}
	else {
		std::cout << "Unrecognised protocol" << std::endl;
	}
}

void ServerController::getNotifications(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username;
	packet >> username;
	int findIndex = checkAccount(username);		//get Account index
	if (findIndex == -1)
	{
		return;		// ignore request if user is not found
	}
	
	int size = notifications[username].size();
	response << size;
	if (!notifications[username].empty())
	{
		std::deque<std::string>::iterator it;
		for (it = notifications[username].begin(); it != notifications[username].end(); ++it)
		{
			response << *it;
		} 
	}
	client.send(response);
	clearNotifications(username);
}

void ServerController::addNotification(const std::string &username, const std::string &str)
{
	notifications[username].push_back(str);
}

void ServerController::clearNotifications(const std::string &username)
{
	while (!notifications[username].empty())
	{
		notifications[username].pop_front();
	} 
}

void ServerController::logoutUser(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username;
	packet >> username;
	int findIndex = checkAccount(username);		//get Account index
	if (findIndex == -1)
	{
		return;		// ignore request if user is not found
	}
	std::cout << username << " logged out" << std::endl;
	accounts[findIndex].endSession();	// log out found user
}

void ServerController::createConference(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username;
	Conference addConf;
	bool exists = false;
	// check user is an admin
	packet >> username;
	int findIndex = checkAccount(username);		//get Account index
	if (findIndex == -1)
	{
		return;		// ignore request if user is not found
	}
	bool isAdmin = accounts[findIndex].isSystemAdmin();
	if (!isAdmin)
	{
		return;		// ignore request if user is not an admin
	}
	// check conference doesn't already exist before adding
	packet >> addConf;
	exists = conferenceExists(addConf.getName());
	if (!exists)
	{
		std::cout << "Admin user " << username;
		std::cout << " created conference " << addConf.getName() << std::endl;
		conferences.push_back(addConf);
		// add conference to user access map
		accounts[findIndex].addAccess(addConf.getName(), Account::Access_Admin);
	}
	// send response
	response << exists;
	client.send(response);
}

bool ServerController::conferenceExists(const std::string &title)
{
	std::vector<Conference>::iterator it;
	for (it = conferences.begin(); it != conferences.end(); ++it)
	{
		if (it->getName() == title)
		{
			return true;
		}
	}
	return false;
}

void ServerController::getAdminStatus(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet adminPacket;
	std::string username;
	packet >> username;
	
	int findIndex = checkAccount(username);		//get Account index
	if (findIndex == -1)
	{
		adminPacket << false;
		client.send(adminPacket);
		return;
	}
	bool isAdmin = accounts[findIndex].isSystemAdmin();
	std::cout << "User is " << (isAdmin ? "" : "not ") << "an admin" << std::endl;
	adminPacket << isAdmin;
	client.send(adminPacket);
}

void ServerController::getSubmissions(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet subPacket;
	std::string username;
	packet >> username;
	std::string firstname, lastname, fullname;
	
	int findIndex = checkAccount(username);		//get Account index
	
	if (findIndex == -1)
	{
		subPacket << 0;
		client.send(subPacket);
		return;
	}
	firstname = accounts[findIndex].getFirstName();
	lastname = accounts[findIndex].getLastName();		//get the fullname for the account
	
	std::vector<std::string> sub;
	int count=0;
	
	for(int i=0;i<submissions.size();i++)
	{
		if(submissions[i].isAuthorIncluded(firstname, lastname))
		{
			count++;
			sub.push_back(submissions[i].getTitle());
		}
	}
	
	subPacket << count;			//add the amount of submissons the author is involved in
	
	for(int i=0;i<sub.size();i++){
		subPacket<< sub[i];				//pack all the submissions in the packet
	}
	client.send(subPacket);
}

void ServerController::getAccess(sf::Packet &packet, sf::TcpSocket &socket)
{
	sf::Packet accessDetails;
	std::string user, conference;
	packet >> user >> conference;
	
	int findIndex = checkAccount(user);
	Account::AccessLevel level;
	if (findIndex != -1)
	{
		level = accounts[findIndex].getAccess(conference);
		accessDetails << level;
		socket.send(accessDetails);
	}
}

void ServerController::getConferences(sf::Packet &packet, sf::TcpSocket &socket)
{
	sf::Packet list;
	std::string user;
	packet >> user;
	
	int findIndex = checkAccount(user);
	std::vector<std::string> results;
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

void ServerController::checkPhase(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username;
	std::string conference;
	std::string currentPhase;
	sf::Packet response;
	Account::AccessLevel level;
	
	packet >> username >> conference;
	
	int findIndex = checkAccount(username);
	int confIndex = checkConference(conference);
	
	if (confIndex != -1)
	{
		level = accounts[findIndex].getAccess(conference);
		currentPhase = conferences[confIndex].getCurrentPhase();
	
		response << level << currentPhase;
		client.send(response);
	}
}

void ServerController::bidPaper(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username;
	std::string conference;
	std::string subTitle;
	
	packet >> username >> conference >> subTitle;
	
	int findIndex = checkAccount(username);
	int confIndex = checkConference(conference);
	
	if (confIndex != -1)
	{
		if (conferences[confIndex].getCurrentPhase() == "Allocation")
		{
			for (int i = 0; i < (int)submissions.size(); i++)
			{
				if (conference == submissions[i].getConference())
				{
					if (subTitle == submissions[i].getTitle())
					{
						submissions[i].addReviewer(username);
					}
				}
			}
		}			
	}
}

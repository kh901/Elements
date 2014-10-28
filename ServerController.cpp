#include "ServerController.h"

ServerController::ServerController()
{

}

void ServerController::autoAllocate()
{

	std::cout << "Allocating..." << std::endl;
	int numOfReviewers = 0;
	int maxReviewers = 0;
	int spotsLeft = 0;
	bool verdict = false;
	bool reviewerAlready = false;
	std::string username;
	for (int i = 0; i < (int)deadlineSubmissions.size(); i++)
	{
		for (int j = 0; j <(int)submissions.size(); j++)
		{
			if (deadlineSubmissions[i] == submissions[j].getTitle())
			{
				numOfReviewers = submissions[j].getReviewerCount();
				for (int k = 0; k < (int)conferences.size(); k++)
				{
					if (submissions[j].getConference() == conferences[k].getName())
					{
						maxReviewers = conferences[k].getMaxPaperReviewers();
					}
				}
				spotsLeft = maxReviewers - numOfReviewers;
				if (spotsLeft == 0)
				{
					deadlineSubmissions.erase(deadlineSubmissions.begin()+i);
				}
				else
				{
					for (int k = 0; k < (int)accounts.size() || verdict == true || spotsLeft > 0; k++)
					{
						verdict = accounts[k].matchKeywordsForSubmission(submissions[j]);
						if (verdict == true)
						{
							username = accounts[k].getUsername();
							if (submissions[j].hasReviewer(username))
							{
								verdict = false;
							}
							else
							{
								submissions[j].addReviewer(username);
								spotsLeft--;
							}
						}
					}
				}
			}
		}
	}
}

void ServerController::paperSubmission(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	Submission sub;
	bool exists = false;
	std::string username, conference, title;
	
	packet >> username;
	int findIndex = checkAccount(username);		//get Account index
	if (findIndex == -1)
	{
		return;		// ignore request if user is not found
	}
	packet >> sub;
	conference = sub.getConference();
	title = sub.getTitle();
	int confIndex = checkConference(conference);
	if (confIndex == -1)
	{
		return; 	// ignore request if conference is not found
	}
	std::cout << "Conference found" << std::endl;
	if (accounts[findIndex].getAccess(conference) < Account::Access_Author)
	{
		return;		// ignore request if user is not at least an author of that conference
	}
	std::cout << "User is at least an author" << std::endl;
	
	// check conference is in submission phase
	if (conferences[confIndex].getCurrentPhase() != "Submission")
	{
		return;		// ignore request if conference is not accepting submissions
	}
	
	// check that the paper does not already exist
	if (checkSubmission(title, conference) == -1)
	{
		// set the papers university to the submitting author
		sub.setUniversity(accounts[findIndex].getUniversity());
		// add the submitting author as an author
		std::string firstname = accounts[findIndex].getFirstName();
		std::string lastname = accounts[findIndex].getLastName();
		sub.addAuthor(firstname, lastname);
		submissions.push_back(sub);
		std::cout << "Submitted paper: " << title << " by " << username << std::endl;
	}
	else
	{
		exists = true;
	}
	response << exists;
	client.send(response);
}

void ServerController::run()
{
	loadFalseAccounts();
	//loadFalseSubmissions();
	loadFalseConferences();
	
	sf::TcpListener listener;
	
	if(listener.listen(60000)!= sf::Socket::Done)
		std::cout<<"error"<<std::endl;
		
	selector.add(listener);
	
	for(;;){
		/*
		time_t current = getTimeValue();
		//check if current time is past deadline
		if (current > deadline)
		{
			autoAllocate();
		}
		*/
		
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
	author.setUniversity("University Of Wollongong");
	author.addAccess("Medical Conference", Account::Access_Author);
	author.addAccess("Information Technology Talk", Account::Access_Admin);
	
	Account reviewer;
	reviewer.setUsername("Jonathan");
	reviewer.setPassword("pass");
	reviewer.setFirstName("Jonathan");
	reviewer.setLastName("Yip");
	reviewer.setUniversity("University Of Sydney");
	reviewer.addKeyword("medicine");
	reviewer.addKeyword("phones");
	reviewer.addAccess("Medical Conference", Account::Access_Reviewer);
	addNotification("Jonathan", "Your emails have reached maximum storage.");
	addNotification("Jonathan", "Your request for more storage has been denied");
	addNotification("Jonathan", "Welcome to the system");
	
	Account admin;
	admin.setUsername("Kieran");
	admin.setPassword("pass");
	admin.setFirstName("Kieran");
	admin.setLastName("Haavisto");
	admin.setSystemAdmin();
	admin.setUniversity("University Of Technology Sydney");
	admin.addKeyword("medicine");
	admin.addKeyword("technology");
	admin.addAccess("Medical Conference", Account::Access_Admin);
		
	accounts.push_back(author);
	accounts.push_back(reviewer);
	accounts.push_back(admin);
}

void ServerController::loadFalseConferences(){
	Conference con, con2, con3;
	
	con.setName("Medical Conference");
	con2.setName("Information Technology Talk");
	con3.setName("Telecommunications Conference");
	
	con.setDate("30/10/2014");
	con2.setDate("4/11/2014");
	con3.setDate("28/11/2014");
	
	con.setLocation("University Hall");
	con2.setLocation("SMART Building");
	con3.setLocation("Telstra Headquarters");
	
	con.setMaxReviewedPapers(5);
	con2.setMaxReviewedPapers(5);
	con3.setMaxReviewedPapers(3);
	
	con.setMaxPaperReviewers(4);
	con2.setMaxPaperReviewers(3);
	con3.setMaxPaperReviewers(2);
	
	con.addReviewer("Kieran");
	con2.addReviewer("Jonathan");
	con3.addReviewer("Adam");
	
	con.setChairman("Omar");
	con2.setChairman("Joshua");
	con3.setChairman("Jason");
	
	con.addSubchair("Kieran");
	con2.addSubchair("Jonathan");
	con3.addSubchair("Adam");
	
	conferences.push_back(con);
	conferences.push_back(con2);
	conferences.push_back(con3);
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

int ServerController::checkSubmission(const std::string &title, const std::string &conf)
{
	for (int i = 0; i < (int)submissions.size(); i++)
	{
		if (submissions[i].getTitle() == title && submissions[i].getConference() == conf)
		{
			return i;
		}
	}
	return -1;
}

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
	else if (protocol=="BID_LIST"){
		bidList(packet, client);
	}
	else if (protocol=="BID_PAPER"){
		bidPaper(packet, client);
	}
	else if (protocol=="ADVANCE_PHASE"){
		advancePhase(packet, client);
	}
	else if(protocol=="BYE"){
		logoutUser(packet, client);
	}
	else if(protocol=="GET_ALLOCATIONS"){
		getAllocations(packet, client);
	}
	else if(protocol=="CONFERENCE_SUBMISSIONS"){
		getConferenceSubs(packet, client);
	}
	else if(protocol=="REVIEW_LIST"){
		getReviewList(packet, client);
	}
	else if (protocol=="SUB_DETAIL"){
		sendSubDetail(packet, client);
	}
	else if (protocol=="SUBMIT_REVIEW"){
		submitReview(packet, client);
	}
	else if (protocol=="GET_COMMENTS"){
		getComments(packet, client);
	}
	else if (protocol=="SEND_COMMENT"){
		sendComments(packet, client);
	}
	else if (protocol=="ADD_REVIEWER"){
		addMember(packet, client, Account::Access_Reviewer);
	}
	else if (protocol=="ADD_AUTHOR"){
		addMember(packet, client, Account::Access_Author);
	}
	else if (protocol=="CHANGE_MAX_ALLOCATED_CONF"){
		changeLimit(packet, client, "ALLOCATED");
	}
	else if (protocol=="CHANGE_MAX_PAPERREVIEWERS_CONF"){
		changeLimit(packet, client, "PAPERREV");
	}
	else if (protocol=="GET_MAX_ALLOCATED_CONF"){
		getLimit(packet, client, "ALLOCATED");
	}
	else if (protocol=="GET_MAX_PAPERREVIEWERS_CONF"){
		getLimit(packet, client, "PAPERREV");
	}
	else if (protocol=="GET_FULLNAME"){
		getAccountName(packet, client);
	}
	else if (protocol=="VIEW_REVIEW"){
		getReview(packet, client);
	}
	else if (protocol=="NOTIFY_COUNT"){
		checkNotifyCount(packet, client);
	}
	else {
		std::cout << "Unrecognised protocol" << std::endl;
	}
}

void ServerController::getLimit(sf::Packet &packet, sf::TcpSocket &client, const std::string &mode)
{
	sf::Packet response;
	std::string username, conference;
	sf::Int16 limit;
	packet >> username >> conference >> limit;
	// authenticate request
	int accIndex = checkAccount(username);
	if (accIndex == -1)
	{
		return;
	}
	// authenticate conference
	int confIndex = checkConference(conference);
	if (confIndex == -1)
	{
		return;
	}
	if (mode == "ALLOCATED")
	{
		limit = conferences[confIndex].getMaxReviewedPapers();
	}
	else if (mode == "PAPERREV")
	{
		limit = conferences[confIndex].getMaxPaperReviewers();
	}
	response << limit;
	client.send(response);
}

void ServerController::changeLimit(sf::Packet &packet, sf::TcpSocket &client, const std::string &mode)
{
	std::string username, conference;
	sf::Int16 val;
	packet >> username >> conference >> val;
	int setVal = val;
	// authenticate request
	int accIndex = checkAccount(username);
	if (accIndex == -1)
	{
		return;
	}
	// authenticate conference
	int confIndex = checkConference(conference);
	if (confIndex == -1)
	{
		return;
	}
	// authenticate privileges
	if (accounts[accIndex].getAccess(conference) < Account::Access_Chairman)
	{
		return;
	}
	if (val > 0 && val < MAXIMUM_USEABLEINT_LIMIT)
	{
		if (mode == "ALLOCATED")
		{
			conferences[confIndex].setMaxReviewedPapers(setVal);
		}
		else if (mode == "PAPERREV")
		{
			conferences[confIndex].setMaxPaperReviewers(setVal);
		}
	}
}

void ServerController::getAccountName(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username, target, first, last;
	bool exists = false;
	packet >> username >> target;
	std::cout << "Attempting to find full name for User " << target << std::endl;
	// authenticate request
	if (checkAccount(username) == -1)
	{
		return;
	}
	int targetIndex = checkAccount(target);
	if (targetIndex != -1)
	{
		std::cout << "User does exist" << std::endl;
		first = accounts[targetIndex].getFirstName();
		last = accounts[targetIndex].getLastName();
		exists = true;
		response << exists << first << last;
		client.send(response);
		return;
	}
	std::cout << "User does not exist" << std::endl;
	response << exists;
	client.send(response);
}
void ServerController::getReview(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username, conf, id;
	Review aReview;
	bool found = false;
	packet >> username >> conf >> id;
	// authenticate request
	if (checkAccount(username) == -1)
	{
		return;
	}
	// authenticate conference
	if (checkConference(conf) == -1)
	{
		return;
	}
	// find review with id
	std::vector<Review>::iterator it;
	for (it = reviews.begin(); it != reviews.end(); ++it)
	{
		if (it->getReviewID() == id)
		{
			aReview = *it;
			found = true;
			break;
		}
	}
	response << found;
	if (found)
	{
		response << aReview;
	}
	client.send(response);
}
		

void ServerController::addMember(sf::Packet &packet, sf::TcpSocket &client, Account::AccessLevel level)
{
	sf::Packet response;
	std::string username, conference, targetUser;
	packet >> username >> conference >> targetUser;
	
	bool success = false;
	
	int findIndex = checkAccount(username);
	int targetIndex = checkAccount(targetUser);
	int confIndex = checkConference(conference);
	if (findIndex == -1 || targetIndex == -1 || confIndex == -1)
	{
		success = false;
		response << success;
		client.send(response);
		return;
	}
	// add access to the conference in the target user's accessmap
	accounts[targetIndex].addAccess(conference, level);
	// add welcome notification to the user
	addNotification(targetUser, "Welcome to " + conference + "!");
	success = true;
	response << success;
	client.send(response);
}

void ServerController::sendComments(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username, conference, subTitle, newComment;
	
	packet >> username >> conference >> subTitle >> newComment;
	
	for (int i = 0; i < (int)submissions.size(); i++)
	{
		if (submissions[i].getConference() == conference)
		{
			if (submissions[i].getTitle() == subTitle)
			{
				if (submissions[i].hasReviewer(username))
				{
					submissions[i].addComment(username, newComment);
				}
			}
		}
	}
}

void ServerController::getComments(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string conference, paperTitle;
	sf::Packet response;
	
	packet >> conference >> paperTitle;
	std::vector<Comment> comments;
	
	for (int i = 0; i < (int)submissions.size(); i++)
	{
		if (submissions[i].getConference() == conference)
		{
			if (submissions[i].getTitle() == paperTitle)
			{
				submissions[i].getComments(comments);
			}
		}
	}
	
	response << (int)comments.size();
	for (int i = 0; i < (int)comments.size(); i++)
	{
		response << comments[i];
	}
	client.send(response);
}

void ServerController::submitReview(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username, conference, paperTitle;
	Review review;
	std::string firstname, lastname;
	
	packet >> username >> conference >> paperTitle;
	packet >> review;
	
	int findIndex = checkAccount(username);
	if (findIndex == -1)
	{
		return;		// ignore request if user does not exist
	}
	firstname = accounts[findIndex].getFirstName();
	lastname = accounts[findIndex].getLastName();
	
	review.setPCMember(firstname + " " + lastname);
	
	for (int i = 0; i < (int)submissions.size(); i++)
	{
		if (submissions[i].getConference() == conference)
		{
			if (submissions[i].getTitle() == paperTitle)
			{
				if (submissions[i].hasReviewer(username))
				{
					std::cout << "Review submitted in Conference " << conference << " for Paper " << paperTitle << " by " << username << std::endl;
					submissions[i].setReviewed();
					reviews.push_back(review);
					break;
				}
			}
		}
	}
}

void ServerController::sendSubDetail(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string conf, paperTitle;
	packet >> conf >> paperTitle;
	
	int confIndex = checkConference(conf);
	if (confIndex == -1)
	{
		return; 	// ignore request if conference is not found
	}
	
	int subIndex = checkSubmission(paperTitle, conf);
	if (subIndex == -1)
	{
		return;		// ignore request if paper is not found
	}
	
	Submission result = submissions[subIndex]; 
	response << result;
	client.send(response);
}

void ServerController::getConferenceSubs(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string conf;
	packet >> conf;
	
	// add all matching submissions with this conference name
	std::vector<std::string> results;
	std::vector<Submission>::iterator it;
	for (it = submissions.begin(); it != submissions.end(); ++it)
	{
		if (it->getConference() == conf)
		{
			results.push_back(it->getTitle());
		}
	}
	
	// pack into the response packet
	response << (int)results.size();
	for (int i = 0; i < (int)results.size(); ++i)
	{
		response << results[i];
	}	
	client.send(response);
}
void ServerController::getReviewList(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string conference, paperTitle;
	packet >> conference >> paperTitle;
	
	std::vector<std::string> reviewIDs;
	for (int i = 0; i < (int)reviews.size(); i++)
	{
		if (reviews[i].getConference() == conference)
		{
			if (reviews[i].getTitle() == paperTitle)
			{
				reviewIDs.push_back(reviews[i].getReviewID());
			}
		}
	}
	
	response << (int)reviewIDs.size();
	for (int i = 0; i < (int)reviewIDs.size(); i++)
	{
		response << reviewIDs[i];
	}
	
	client.send(response);
}

void ServerController::getAllocations(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username, conference;
	int allocatedSize = 0;
	std::vector<std::string> allocatedPapers;
	std::string temp;
	
	packet >> username >> conference;
	
	for (int i = 0; i < (int)submissions.size(); i++)
	{
		if (submissions[i].getConference() == conference)
		{
			if (submissions[i].hasReviewer(username))
			{
				allocatedPapers.push_back(submissions[i].getTitle());
				allocatedSize++;
			}
		}
	}
	
	response << allocatedSize;
	
	for (int i = 0; i < allocatedSize; i++)
	{
		temp = allocatedPapers[i];
		response << temp;
	}
	
	client.send(response);
}

void ServerController::bidList(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	// generate a list of submissions that:
	// have reviewer slots left
	// are not already bid on by the reviewer
	// do not create a conflict of interest
	std::string username, conf;
	packet >> username >> conf;
	
	int findIndex = checkAccount(username);		//get Account index
	if (findIndex == -1)
	{
		return;		// ignore request if user is not found
	}
	int confIndex = checkConference(conf);
	if (confIndex == -1)
	{
		return; 	// ignore request if conference is not found
	}
	if (accounts[findIndex].getAccess(conf) < Account::Access_Reviewer)
	{
		return;		// ignore request if user is not at least a reviewer of that conference
	}
	
	std::cout << "Bid list for Reviewer " << username << " in Conference " << conf << std::endl;
	
	std::string firstname = accounts[findIndex].getFirstName();
	std::string lastname = accounts[findIndex].getLastName();
	
	std::vector<std::string> results;
	for (int a = 0; a < (int)submissions.size(); ++a)
	{
		std::cout << "Considering sub: " << submissions[a].getTitle() << " conf: " << submissions[a].getConference() << std::endl;
		// match conference
		if (submissions[a].getConference() == conf)
		{
			// check not already bid for paper and avoid conflicts of interest
			if (!(submissions[a].hasReviewer(username)))
			{
				if (submissions[a].getUniversity() != accounts[findIndex].getUniversity() && 
						!submissions[a].isAuthorIncluded(firstname, lastname))
				{
					int numReviewers = submissions[a].getReviewerCount();
					int maxPaperReviewers = conferences[confIndex].getMaxPaperReviewers();
					// check reviewer slots are free
					if (numReviewers < maxPaperReviewers)
					{
						results.push_back(submissions[a].getTitle());
					}
					else
					{
						std::cout << "Not enough reviewer slots left: " << numReviewers << " / " << maxPaperReviewers << std::endl;
					}
				}
				else
				{
					std::cout << "Reviewer has a conflict of interest with this submission" << std::endl;
				}
			}
			else
			{
				std::cout << "Reviewer already bid for this submission" << std::endl;
			}
		}
		else
		{
			std::cout << "Wrong conference" << std::endl;
		}
	}
	
	std::cout << "Generated bid list of size: " << results.size() << std::endl;
	
	// send the results
	response << (int)results.size();
	for (int i = 0; i < (int)results.size(); ++i)
	{	
		response << results[i];
	}
	client.send(response);
}

void ServerController::advancePhase(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username, conference;
	bool valid = false;
	packet >> username >> conference;

	int findIndex = checkAccount(username);		//get Account index
	if (findIndex == -1)
	{
		return;		// ignore request if user is not found
	}
	int confIndex = checkConference(conference);
	if (confIndex == -1)
	{
		return; 	// ignore request if conference is not found
	}
	
	// check that they have admin rights to the conference
	Account::AccessLevel access = accounts[findIndex].getAccess(conference);
	
	if (access >= Account::Access_Chairman)
	{
		if (conferences[confIndex].getCurrentPhase() == "Allocation")
		{
			std::cout << "Semi auto allocation for conference " << conference << std::endl;
			allocate(conference);
			std::cout << "Finished allocation" << std::endl;
		}
	
		std::cout << username << " moved Conference " << conference << " from " << conferences[confIndex].getCurrentPhase();
		conferences[confIndex].advancePhase();
		std::cout << " to " << conferences[confIndex].getCurrentPhase() << std::endl;
		notifyConference(conference, conference + " is now in " + conferences[confIndex].getCurrentPhase());
	}
}

void ServerController::allocate(const std::string &conference)
{
	int reviewerCount = 0;
	int maxReviewer = 0;
	int maxPapers = 0;
	int spotsLeft = 0;
	std::string username, firstname, lastname;
	bool verdict = false;
	
	int confIndex = checkConference(conference);
	if (confIndex == -1)
	{
		return;
	}
	
	for (int i = 0; i < (int)submissions.size(); i++)
	{
		if (submissions[i].getConference() == conference)
		{
			std::cout << "Allocating reviewers for Paper " << submissions[i].getTitle() << std::endl;
			reviewerCount = submissions[i].getReviewerCount();
			maxReviewer = conferences[confIndex].getMaxPaperReviewers();
			maxPapers = conferences[confIndex].getMaxReviewedPapers();
			spotsLeft = maxReviewer - reviewerCount;
			if (spotsLeft != 0)
			{
				for (int j = 0; j < (int)accounts.size(); j++)
				{
					firstname = accounts[j].getFirstName();
					lastname = accounts[j].getLastName();
					verdict = accounts[j].matchKeywordsForSubmission(submissions[i]);
					if (verdict == true && spotsLeft > 0)
					{
						username = accounts[j].getUsername();
						if (!submissions[i].hasReviewer(username) && 
								!submissions[i].isAuthorIncluded(firstname, lastname) &&
									accounts[j].incrementAllocated(conference, maxPapers) )
						{
							std::cout << "Allocated Reviewer " << username << " to Paper " << submissions[i].getTitle() << std::endl;
							submissions[i].addReviewer(username);
							spotsLeft--;
						}
					}
				}
			}
			reviewerCount = submissions[i].getReviewerCount();
			if (reviewerCount == 0)
			{
				std::cout << "This paper has no reviewers assigned to it!" << std::endl;
			}
		}
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

void ServerController::checkNotifyCount(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username;
	packet >> username;
	int findIndex = checkAccount(username);		//get Account index
	if (findIndex == -1)
	{
		return;		// ignore request if user is not found
	}
	sf::Int16 notifyCount = getNotificationCount(username);
	response << notifyCount;
	client.send(response);
}

void ServerController::notifyConference(const std::string &conf, const std::string &msg)
{
	for (int i = 0; i < (int)accounts.size(); ++i)
	{
		if (accounts[i].hasAccess(conf))
		{
			this->addNotification(accounts[i].getUsername(), msg);
		}
	}
}

void ServerController::addNotification(const std::string &username, const std::string &str)
{
	notifications[username].push_back(str);
}

int ServerController::getNotificationCount(const std::string &user)
{
	return notifications[user].size();
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
		addNotification(username, "Welcome to " + addConf.getName() + "!");
		// add conference to reviewers access map
		std::vector<std::string> revs;
		addConf.getReviewers(revs);
		for (int i = 0; i < (int)revs.size(); ++i)
		{
			int getIndex = checkAccount(revs[i]);
			if (getIndex != -1)
			{
				accounts[getIndex].addAccess(addConf.getName(), Account::Access_Reviewer);
				addNotification(revs[i], "Welcome to " + addConf.getName() + "!");
			}
		}
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
	//std::string username;
	std::string conference;
	std::string currentPhase;
	sf::Packet response;
	//Account::AccessLevel level;
	
	//packet >> username >> conference;
	packet >> conference;
	
	//int findIndex = checkAccount(username);
	int confIndex = checkConference(conference);
	
	if (confIndex != -1)
	{
		//level = accounts[findIndex].getAccess(conference);
		currentPhase = conferences[confIndex].getCurrentPhase();
	
		//response << level << currentPhase;
		response << currentPhase;
		client.send(response);
	}
}

void ServerController::bidPaper(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username;
	std::string conf;
	std::string subTitle;
	
	packet >> username >> conf >> subTitle;
	
	int findIndex = checkAccount(username);
	int confIndex = checkConference(conf);
	
	if (confIndex != -1)
	{
		if (conferences[confIndex].getCurrentPhase() == "Allocation")
		{
			for (int i = 0; i < (int)submissions.size(); i++)
			{
				if (conf == submissions[i].getConference() && subTitle == submissions[i].getTitle())
				{
					int reviewerCount = submissions[i].getReviewerCount();
					int maxPaperReviewers = conferences[confIndex].getMaxPaperReviewers();
					if (reviewerCount < maxPaperReviewers)
					{
						submissions[i].addReviewer(username);
						break;
					}
				}
			}
		}			
	}
}

#include "ServerController.h"

ServerController::ServerController()
{

}

void ServerController::autoAllocate()
{
	std::cout << "Allocating..." << std::endl;
	/*
		go through each conference that is currently on the allocation phase
		check if meeting or past the conference's deadline for that phase
		call allocate on that conference
		move the conference to the next phase
	*/
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
	if (data.accounts[findIndex].getAccess(conference) < Account::Access_Author)
	{
		return;		// ignore request if user is not at least an author of that conference
	}
	
	// check conference is in submission phase
	if (data.conferences[confIndex].getCurrentPhase() != "Submission")
	{
		return;		// ignore request if conference is not accepting submissions
	}
	
	// check that the paper does not already exist
	if (checkSubmission(title, conference) == -1)
	{
		// set the papers university to the submitting author
		sub.setUniversity(data.accounts[findIndex].getUniversity());
		// add the submitting author as an author
		std::string firstname = data.accounts[findIndex].getFirstName();
		std::string lastname = data.accounts[findIndex].getLastName();
		
		sub.addAuthor(firstname, lastname);
		data.submissions.push_back(sub);
		data.saveSubmissions();
		
		data.addLog("Paper was submitted: " + title + " by " + username + " for conference " + conference);
		addNotification(username, "You submitted a paper '" + title + "' to " + conference);
		notifyConference(conference, 
			username + " submitted a paper '" + title + "' to " + conference, 
				Account::Access_Chairman);
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
	sf::TcpListener listener;
	
	if(listener.listen(60000)!= sf::Socket::Done)
	{
		data.addLog("Could not bind server to the defined network port (60000).");
		return;
	}
		
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
					data.addLog("New connection from " + client->getRemoteAddress().toString());
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
							processClient(packet, client);
							packet.clear();
						}
						// The client has disconnected, stop listening to them
						else if (status == sf::Socket::Disconnected)
						{
							data.addLog("Client (" + client.getRemoteAddress().toString() 
								+ ") disconnected");
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

int ServerController::checkSubmission(const std::string &title, const std::string &conf)
{
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getTitle() == title && data.submissions[i].getConference() == conf)
		{
			return i;
		}
	}
	return -1;
}

int ServerController::checkConference(std::string conference)
{
	for (int i = 0; i < (int)data.conferences.size(); i++)
	{
		if (data.conferences[i].getName() == conference)
		{
			return i;
		}
	}
	return -1;
}

int ServerController::checkAccount(std::string username, std::string password){
	for(int i = 0; i < (int)data.accounts.size();i++){
		if(data.accounts[i].matchUsername(username))
		{
			if (password=="#USERNAMEONLY" || data.accounts[i].matchPassword(password, false))
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
		data.accounts[findIndex].startSession();
		data.addLog(username + " has logged in.");
		data.saveAccounts();
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
		data.addLog("New user registered! Welcome " + username);
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
		data.accounts.push_back(tmp);
		data.saveAccounts();
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
		
	std::cout << "Interpreting Packet Protocol: " << protocol << std::endl;
		
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
		getReviewList(packet, client, true);
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
	else if (protocol=="APPROVE_PAPER"){
		decidePaper(packet, client, true);
	}
	else if (protocol=="REJECT_PAPER"){
		decidePaper(packet, client, false);
	}
	else if (protocol=="MY_REVIEWS"){
		getReviewList(packet, client, false);
	}
	else if (protocol=="DID_REVIEW"){
		checkReviewed(packet, client);
	}
	else if (protocol=="FINAL_REVIEW"){
		getFinalReview(packet, client);
	}
	else if (protocol=="CONF_REVIEWERS"){
		getReviewers(packet, client);
	}
	else if (protocol=="CONF_SUBMISSIONS"){
		getConfSubmissions(packet, client);
	}
	else if (protocol=="FILLED_ALLOCATION"){
		checkPaperAlloc(packet, client);
	}
	else if (protocol=="GET_FREE_REVIEWERS"){
		getFreeReviewers(packet, client);
	}
	else if (protocol=="ASSIGN_REVIEWER"){
		assignReviewer(packet, client);
	}
	else if (protocol=="CHANGE_PASSWORD"){
		changePassword(packet, client);
	}
	else {
		std::cout << "Unrecognised protocol" << std::endl;
	}
}

void ServerController::changePassword(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username, password;
	packet >> username >> password;
	
	int findIndex = checkAccount(username);
	
	data.accounts[findIndex].setPassword(password);
	data.saveAccounts();
}

void ServerController::assignReviewer(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string conference, paperTitle, targetUsername;
	std::string firstname, lastname;
	
	packet >> conference >> paperTitle >> targetUsername;
	int findIndex = checkAccount(targetUsername);
	firstname = data.accounts[findIndex].getFirstName();
	lastname = data.accounts[findIndex].getLastName();
	int confIndex = checkConference(conference);
	int maxReviewers = data.conferences[confIndex].getMaxPaperReviewers();
	
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getConference() == conference);
		{
			if (data.submissions[i].getTitle() == paperTitle)
			{
				if (!data.submissions[i].isAuthorIncluded(firstname, lastname)
					&& !data.submissions[i].hasReviewer(targetUsername))
				{
					data.submissions[i].addReviewer(targetUsername);
					data.accounts[findIndex].incrementAllocated(conference, maxReviewers);
				}
			}
		}
	}	
	data.saveAccounts();
	data.saveSubmissions();			
}

void ServerController::getFreeReviewers(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string conference;
	std::vector<std::string> reviewerList;
	std::vector<std::string> freeList;
	
	packet >> conference;
	
	int confIndex = checkConference(conference);
	data.conferences[confIndex].getReviewers(reviewerList);
	int maxReviewers = data.conferences[confIndex].getMaxPaperReviewers();
	
	std::cout << "Getting a list of free reviewers for conference " << conference << std::endl;
	for (int i = 0; i < (int)reviewerList.size(); i++)
	{
		int findIndex = checkAccount(reviewerList[i]);
		std::cout << "Displaying Reviewers: " << reviewerList[i] << std::endl;
		if (data.accounts[findIndex].checkAllocation(conference, maxReviewers))
		{
			std::cout << "Pushing" << std::endl;
			freeList.push_back(reviewerList[i]);
		}
	}
	
	response << (int)freeList.size();
	for (int i = 0; i < (int)freeList.size(); i++)
	{
		response << freeList[i];
	}
	
	client.send(response);
} 

void ServerController::checkPaperAlloc(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string conference, paperTitle;
	int rev_count;
	bool result = false;
	
	packet >> conference >> paperTitle;
	
	int confIndex = checkConference(conference);
	int max_rev = data.conferences[confIndex].getMaxPaperReviewers();
	
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getConference() == conference)
		{
			if(data.submissions[i].getTitle() == paperTitle)
			{
				rev_count = data.submissions[i].getReviewerCount();
				if (rev_count < max_rev)
				{
					result = true;
					break;
				}
			}
		}
	}
	response << result;
	client.send(response);	
}

void ServerController::getConfSubmissions(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string conference;
	std::vector<std::string> submission;
	
	packet >> conference;
	
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if(data.submissions[i].getConference() == conference)
		{
			submission.push_back(data.submissions[i].getTitle());
		}
	}
	
	response << (int)submission.size();
	
	for (int i = 0; i < (int)submission.size(); i++)
	{
		response << submission[i];
	}
	
	client.send(response);
}

void ServerController::getReviewers(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username, conference;
	std::vector<std::string> reviewer;
	
	packet >> username >> conference;	

	int confIndex = checkConference(conference);
	data.conferences[confIndex].getReviewers(reviewer);
	
	response << (int)reviewer.size();
	
	for (int i = 0; i < (int)reviewer.size(); i++)
	{
		response << reviewer[i];
	}
	
	client.send(response);
}

void ServerController::getFinalReview(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username, conference, paper;
	std::string finalRev;
	bool exists = false;
	packet >> username >> conference >> paper;
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
	if (data.accounts[accIndex].getAccess(conference))
	{
		for (int i = 0; i < (int)data.reviews.size(); ++i)
		{
			if (data.reviews[i].getConference() == conference &&
				data.reviews[i].getTitle() == paper &&
					data.reviews[i].getFinal())
			{
				finalRev = data.reviews[i].getReviewID();
				exists = true;
				break;
			}
		}
		response << exists << finalRev;
	}
	else
	{
		response << exists;
	}
	client.send(response);
}

void ServerController::checkReviewed(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string user, conference, paper, first, last;
	packet >> user >> conference >> paper;
	
	// authenticate request
	int	findIndex = checkAccount(user);
	if (findIndex != -1)
	{
		first = data.accounts[findIndex].getFirstName();
		last = data.accounts[findIndex].getLastName();
	}
	
	bool hasReviewed = false;
	for (int i = 0; i < (int)data.reviews.size(); ++i)
	{
		if (data.reviews[i].getTitle() == paper && data.reviews[i].getConference() == conference)
		{
			if (data.reviews[i].getPCMember() == (first + " " + last))
			{
				hasReviewed = true;
				break;
			}
		}
	}
	response << hasReviewed;
	client.send(response);
}

void ServerController::decidePaper(sf::Packet &packet, sf::TcpSocket &client, const bool approved)
{
	std::string username, conference, paper;
	packet >> username >> conference >> paper;
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
	if (data.accounts[accIndex].getAccess(conference) < Account::Access_Chairman)
	{
		return;
	}
	// authenticate paper
	int paperIndex = checkSubmission(paper, conference);
	if (paperIndex == -1)
	{
		return;		// ignore request if submission does not exist
	}
	if (approved)
	{
		data.submissions[paperIndex].accept();
	}
	else
	{
		data.submissions[paperIndex].reject();
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
		limit = data.conferences[confIndex].getMaxReviewedPapers();
	}
	else if (mode == "PAPERREV")
	{
		limit = data.conferences[confIndex].getMaxPaperReviewers();
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
	if (data.accounts[accIndex].getAccess(conference) < Account::Access_Chairman)
	{
		return;
	}
	if (val > 0 && val < MAXIMUM_USEABLEINT_LIMIT)
	{
		if (mode == "ALLOCATED")
		{
			data.conferences[confIndex].setMaxReviewedPapers(setVal);
			notifyConference(conference, 
				username + " has changed the max papers per reviewer limit for " + conference, 
					Account::Access_Chairman);
		}
		else if (mode == "PAPERREV")
		{
			data.conferences[confIndex].setMaxPaperReviewers(setVal);
			notifyConference(conference, 
				username + " has changed the max reviewers per paper limit for " + conference, 
					Account::Access_Chairman);
		}
		data.saveConferences();
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
		first = data.accounts[targetIndex].getFirstName();
		last = data.accounts[targetIndex].getLastName();
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
	for (it = data.reviews.begin(); it != data.reviews.end(); ++it)
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
	data.accounts[targetIndex].addAccess(conference, level);
	if (level == Account::Access_Reviewer)
	{
		data.conferences[confIndex].addReviewer(targetUser);
	}
	// add welcome notification to the user
	addNotification(targetUser, "Welcome to " + conference + "!");
	success = true;
	response << success;
	client.send(response);
	data.saveAccounts();
	data.saveConferences();
}

void ServerController::sendComments(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string username, conference, subTitle, newComment;
	
	packet >> username >> conference >> subTitle >> newComment;
	
	int subIndex = -1;
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getConference() == conference)
		{
			if (data.submissions[i].getTitle() == subTitle)
			{
				if (data.submissions[i].hasReviewer(username))
				{
					data.submissions[i].addComment(username, newComment);
					subIndex = i;
				}
			}
		}
	}
	if (subIndex != -1)
	{
		std::vector<std::string> revList;
		data.submissions[subIndex].getReviewerList(revList);
		for (int r = 0; r < (int)revList.size(); ++r)
		{
			if (revList[r] != username)
			{
				addNotification(revList[r], username + " has commented for Paper " + subTitle);
			}
		}
	}
	data.saveSubmissions();
}

void ServerController::getComments(sf::Packet &packet, sf::TcpSocket &client)
{
	std::string conference, paperTitle;
	sf::Packet response;
	
	packet >> conference >> paperTitle;
	std::vector<Comment> comments;
	
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getConference() == conference)
		{
			if (data.submissions[i].getTitle() == paperTitle)
			{
				data.submissions[i].getComments(comments);
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
	firstname = data.accounts[findIndex].getFirstName();
	lastname = data.accounts[findIndex].getLastName();
	
	review.setPCMember(firstname + " " + lastname);
	
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getConference() == conference)
		{
			if (data.submissions[i].getTitle() == paperTitle)
			{
				if (data.submissions[i].hasReviewer(username))
				{					
					// overwrite the old review by this reviewer
					bool exists = false;
					for (int q = 0; q < (int)data.reviews.size(); ++q)
					{
						if (data.reviews[q].getConference() == conference &&
							data.reviews[q].getTitle() == paperTitle &&
								data.reviews[q].getPCMember() == (firstname + " " + lastname))
						{
							data.reviews[q] = review;		// overwrite the old review
							exists = true;
							break;
						}
					}
					if (!exists)
					{
						data.addLog("Review submitted in Conference " + conference + 
							" for Paper " + paperTitle + " by " + username);
						data.reviews.push_back(review);
						data.saveReviews();
					}
					else
					{
						data.addLog("Review resubmitted in Conference " + conference +
							" for Paper " + paperTitle + " by " + username);
					}
					if (review.getFinal())
					{
						data.addLog("Submitted review is final");
						data.submissions[i].setReviewed();
						data.saveSubmissions();
					}
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
	
	Submission result = data.submissions[subIndex]; 
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
	for (it = data.submissions.begin(); it != data.submissions.end(); ++it)
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
void ServerController::getReviewList(sf::Packet &packet, sf::TcpSocket &client, const bool allReviewers)
{
	sf::Packet response;
	std::string conference, paperTitle, user, first, last;
	packet >> conference >> paperTitle;
	int findIndex = -1;
	if (!allReviewers)
	{
		packet >> user;
		findIndex = checkAccount(user);
		if (findIndex != -1)
		{
			first = data.accounts[findIndex].getFirstName();
			last = data.accounts[findIndex].getLastName();
		}
	}
	
	std::vector<std::string> reviewIDs;
	for (int i = 0; i < (int)data.reviews.size(); i++)
	{
		if (data.reviews[i].getConference() == conference)
		{
			bool isOwnPaper = (data.reviews[i].getPCMember() == first + " " + last);
			if (data.reviews[i].getTitle() == paperTitle && 
					(allReviewers || !allReviewers && isOwnPaper))
			{
				reviewIDs.push_back(data.reviews[i].getReviewID());
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
	
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getConference() == conference)
		{
			if (data.submissions[i].hasReviewer(username))
			{
				allocatedPapers.push_back(data.submissions[i].getTitle());
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
	if (data.accounts[findIndex].getAccess(conf) < Account::Access_Reviewer)
	{
		return;		// ignore request if user is not at least a reviewer of that conference
	}
	
	std::cout << "Bid list for Reviewer " << username << " in Conference " << conf << std::endl;
	
	std::string firstname = data.accounts[findIndex].getFirstName();
	std::string lastname = data.accounts[findIndex].getLastName();
	
	std::vector<std::string> results;
	for (int a = 0; a < (int)data.submissions.size(); ++a)
	{
		std::cout << "Considering sub: " << data.submissions[a].getTitle() << " conf: " << data.submissions[a].getConference() << std::endl;
		// match conference
		if (data.submissions[a].getConference() == conf)
		{
			// check not already bid for paper and avoid conflicts of interest
			if (!(data.submissions[a].hasReviewer(username)))
			{
				if (data.submissions[a].getUniversity() != data.accounts[findIndex].getUniversity() && 
						!data.submissions[a].isAuthorIncluded(firstname, lastname))
				{
					int numReviewers = data.submissions[a].getReviewerCount();
					int maxPaperReviewers = data.conferences[confIndex].getMaxPaperReviewers();
					// check reviewer slots are free
					if (numReviewers < maxPaperReviewers)
					{
						results.push_back(data.submissions[a].getTitle());
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

bool ServerController::checkAllFinalised(const std::string &conference)
{
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getConference() == conference)
		{
			if (!data.submissions[i].getReviewed())
			{
				return false;
			}
		}
	}
	
	return true;
}

void ServerController::advancePhase(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet response;
	std::string username, conference;
	bool valid = false;
	bool result = true;
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
	Account::AccessLevel access = data.accounts[findIndex].getAccess(conference);
	
	if (access >= Account::Access_Chairman)
	{
		std::string reason;
		std::string oldAdvancePhase = data.conferences[confIndex].getCurrentPhase();
		if (oldAdvancePhase == "Allocation")
		{
			data.addLog("Attempting semi auto allocation for conference " + conference);
			result = allocate(conference);
			if (!result)
			{	
				data.addLog("Auto allocation could not allocate some papers. Manual allocation recommended");
				reason = "Auto Allocation failed. Please manually allocate to rectify.";
			}
			else
			{
				data.addLog("Finished allocation");
			}
		}
		// auto reject all papers in a conference that have not been approved
		else if (oldAdvancePhase == "Finalising")
		{
			result = checkAllFinalised(conference);
			if (!result)
			{
				reason = "Final reviews missing. Please complete the final reviews on all papers.";
			}
			autoRejectPapers(conference);
		}
		else if (oldAdvancePhase == "Completed")
		{
			result = true;		// ignore this request
		}
	
		response << result;
		
		if (!result)
		{
			response << reason;
			std::cout << conference << " could not be moved from " << oldAdvancePhase << std::endl;
		}
		else
		{
			data.conferences[confIndex].advancePhase();
			data.addLog(username + " moved Conference " + conference + " from " + oldAdvancePhase
				+ " to " + data.conferences[confIndex].getCurrentPhase());
		}
		notifyConference(conference, conference + " is now in " + data.conferences[confIndex].getCurrentPhase());
		client.send(response);
		data.saveConferences();
	}
}

void ServerController::autoNotifyAuthors(const Submission &sub)
{
	std::vector<Fullname> list;
	std::string paper = sub.getTitle();
	std::string status = sub.getStatus();
	sub.getAuthors(list);
	for (int e = 0; e < (int)list.size(); ++e)
	{
		std::string authFirst = list[e].firstname;
		std::string authLast = list[e].surname;
		for (int w = 0; w < (int)data.accounts.size(); ++w)
		{
			std::string accountFirst = data.accounts[w].getFirstName();
			std::string accountLast = data.accounts[w].getLastName();
			if (accountFirst == authFirst && accountLast == authLast &&
					data.accounts[w].hasAccess(sub.getConference()))
			{
				addNotification(data.accounts[w].getUsername(), paper + " has been " + status);
			}
		}
	}
}

void ServerController::autoRejectPapers(const std::string &conference)
{
	for (int i = 0; i < (int)data.submissions.size(); ++i)
	{
		std::string status = data.submissions[i].getStatus();
		if (data.submissions[i].getConference() == conference && 
			status == "Pending")
		{
			data.submissions[i].reject();
		}
		autoNotifyAuthors(data.submissions[i]);
	}
	data.saveSubmissions();
}

bool ServerController::allocate(const std::string &conference)
{
	int reviewerCount = 0;
	int maxReviewer = 0;
	int maxPapers = 0;
	int spotsLeft = 0;
	std::string username, firstname, lastname;
	bool verdict = false;
	bool allAllocated = true;
	
	int confIndex = checkConference(conference);
	if (confIndex == -1)
	{
		return false;
	}
	
	for (int i = 0; i < (int)data.submissions.size(); i++)
	{
		if (data.submissions[i].getConference() == conference)
		{
			data.addLog("Allocating reviewers by keywords for Paper " + data.submissions[i].getTitle());
			reviewerCount = data.submissions[i].getReviewerCount();
			maxReviewer = data.conferences[confIndex].getMaxPaperReviewers();
			maxPapers = data.conferences[confIndex].getMaxReviewedPapers();
			spotsLeft = maxReviewer - reviewerCount;
			if (spotsLeft != 0)
			{
				for (int j = 0; j < (int)data.accounts.size(); j++)
				{
					firstname = data.accounts[j].getFirstName();
					lastname = data.accounts[j].getLastName();
					verdict = data.accounts[j].matchKeywordsForSubmission(data.submissions[i]);
					if (verdict == true && spotsLeft > 0)
					{
						username = data.accounts[j].getUsername();
						if (!data.submissions[i].hasReviewer(username) &&
								!data.submissions[i].isAuthorIncluded(firstname, lastname) &&
									data.accounts[j].incrementAllocated(conference, maxPapers) )
						{
							data.addLog("Allocated Reviewer " + username + 
								" to Paper " + data.submissions[i].getTitle());
							data.submissions[i].addReviewer(username);
							spotsLeft--;
						}
					}
				}
			}
			reviewerCount = data.submissions[i].getReviewerCount();
			if (reviewerCount == 0)
			{
				data.addLog("Could not allocate reviewers to paper " + data.submissions[i].getTitle());
				allAllocated = false;
			}
		}
	}
	data.saveAccounts();
	data.saveSubmissions();
	
	return allAllocated;
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

void ServerController::notifyConference(const std::string &conf, const std::string &msg, Account::AccessLevel minAccess)
{
	for (int i = 0; i < (int)data.accounts.size(); ++i)
	{
		if (data.accounts[i].hasAccess(conf) && data.accounts[i].getAccess(conf) >= minAccess)
		{
			this->addNotification(data.accounts[i].getUsername(), msg);
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
	data.addLog(username + " logged out");
	data.accounts[findIndex].endSession();	// log out found user
	data.saveAccounts();
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
	bool isAdmin = data.accounts[findIndex].isSystemAdmin();
	if (!isAdmin)
	{
		return;		// ignore request if user is not an admin
	}
	// check conference doesn't already exist before adding
	packet >> addConf;
	exists = conferenceExists(addConf.getName());
	if (!exists)
	{
		data.addLog("Admin user " + username +
			" created conference " + addConf.getName());
		data.conferences.push_back(addConf);
		
		// add conference to user access map
		data.accounts[findIndex].addAccess(addConf.getName(), Account::Access_Admin);
		addNotification(username, "Welcome to " + addConf.getName() + "!");
		
		// add conference to reviewers access map
		std::vector<std::string> revs;
		addConf.getReviewers(revs);
		for (int i = 0; i < (int)revs.size(); ++i)
		{
			int getIndex = checkAccount(revs[i]);
			if (getIndex != -1)
			{
				data.accounts[getIndex].addAccess(addConf.getName(), Account::Access_Reviewer);
				addNotification(revs[i], "Welcome to " + addConf.getName() + "!");
			}
		}
		data.saveConferences();
		data.saveAccounts();
	}
	// send response
	response << exists;
	client.send(response);
}

bool ServerController::conferenceExists(const std::string &title)
{
	std::vector<Conference>::iterator it;
	for (it = data.conferences.begin(); it != data.conferences.end(); ++it)
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
	bool isAdmin = data.accounts[findIndex].isSystemAdmin();
	std::cout << "User is " << (isAdmin ? "" : "not ") << "an admin" << std::endl;
	adminPacket << isAdmin;
	client.send(adminPacket);
}

void ServerController::getSubmissions(sf::Packet &packet, sf::TcpSocket &client)
{
	sf::Packet subPacket;
	std::string username, conference;
	packet >> username >> conference;
	std::string firstname, lastname, fullname;
	
	int findIndex = checkAccount(username);		//get Account index
	
	if (findIndex == -1)
	{
		subPacket << 0;
		client.send(subPacket);
		return;
	}
	firstname = data.accounts[findIndex].getFirstName();
	lastname = data.accounts[findIndex].getLastName();		//get the fullname for the account
	
	std::vector<std::string> sub;
	int count=0;
	
	for(int i=0;i<(int)data.submissions.size();i++)
	{
		if (data.submissions[i].getConference() == conference)
		{
			if(data.submissions[i].isAuthorIncluded(firstname, lastname))
			{
				count++;
				sub.push_back(data.submissions[i].getTitle());
			}
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
		level = data.accounts[findIndex].getAccess(conference);
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
		data.accounts[findIndex].getConferences(results);
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
		//level = data.accounts[findIndex].getAccess(conference);
		currentPhase = data.conferences[confIndex].getCurrentPhase();
	
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
	
	if (confIndex != -1 && findIndex != -1)
	{
		if (data.conferences[confIndex].getCurrentPhase() == "Allocation")
		{
			for (int i = 0; i < (int)data.submissions.size(); i++)
			{
				if (conf == data.submissions[i].getConference() && subTitle == data.submissions[i].getTitle())
				{
					int reviewerCount = data.submissions[i].getReviewerCount();
					int maxPaperReviewers = data.conferences[confIndex].getMaxPaperReviewers();
					if (data.accounts[findIndex].incrementAllocated(conf, maxPaperReviewers))
					{
						data.submissions[i].addReviewer(username);
						break;
					}
				}
			}
		}			
	}
	data.saveAccounts();
	data.saveSubmissions();
}

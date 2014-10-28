#include "Submission.h"

Submission::Submission()
{
    reviewed = false;
    status = "Pending";
    deadline = getTimeValue();
}

Submission::Submission(const Submission &other)
{
	filename = other.filename;
	title = other.title;
	description = other.description;
	reviewed = other.reviewed;
	authors = other.authors;
	keywords = other.keywords;
	comments = other.comments;
	conference = other.conference;
	status = other.status;
	reviewers = other.reviewers;
	uni = other.uni;
	deadline = other.deadline;
}
Submission & Submission::operator=(const Submission &other)
{
	filename = other.filename;
	title = other.title;
	description = other.description;
	reviewed = other.reviewed;
	authors = other.authors;
	keywords = other.keywords;
	comments = other.comments;
	conference = other.conference;
	status = other.status;
	reviewers = other.reviewers;
	uni = other.uni;
	deadline = other.deadline;
	return *this;
}

void Submission::setReviewed(){
	reviewed = true;
}

void Submission::accept()
{
	status = "Accepted";
}
void Submission::reject()
{
	status = "Rejected";
}

std::string Submission::getStatus() const
{
	return status;
}
void Submission::setConference(const std::string &aConf)
{
	conference = aConf;
}
std::string Submission::getConference() const
{
	return conference;
}
std::string Submission::getTitle() const
{
	return title;
}
void Submission::setUniversity(const std::string &aUni)
{
	uni = aUni;
}
std::string Submission::getUniversity() const
{
	return uni;
}

void Submission::adjustDeadlineDays(const int num)
{
	if (num > 0)
	{
		deadline = getDaysAhead(getTimeValue(), num);
	}
}
bool Submission::hasDeadlinePassed(const time_t cur)
{
	return (cur >= deadline);
}

bool Submission::isAuthorIncluded(const std::string &aFirst, const std::string &aLast){
	std::vector<Fullname>::iterator it;
	for (it = authors.begin(); it != authors.end(); ++it)
	{
		if (it->firstname == aFirst && it->surname == aLast) 
		{
			return true;
		}
	}
	return false;
}


void Submission::submit()
{
    std::cout << "Enter paper name: ";
    getline(std::cin, filename);
    Menu::eraseLine("Enter paper name: " + filename);
    
    std::cout << "Enter title: ";
    getline(std::cin, title);
    Menu::eraseLine("Enter title: " + title);
    
    Fullname author;
    std::string cmd;
    do
    {
        do
        {
            std::cout << "Do you want to add another author to this paper? (y/n): ";
            getline(std::cin, cmd);
            Menu::eraseLine("Do you want to add another author to this paper? (y/n): " + cmd);
            if(cmd != "y" && cmd != "n")
            {
                std::cout << "Invalid option, please try again.";
                std::cin.ignore(1, '\n');
                Menu::eraseLine("Invalid option, please try again.");
            }
        }while(cmd != "y" && cmd != "n");
        if (cmd == "y")
        {
		    std::cout << "Enter authors firstname: ";
		    getline(std::cin, author.firstname);
		    Menu::eraseLine("Enter authors firstname: " + author.firstname);
		    
		    std::cout << "Enter authors lastname: ";
		    getline(std::cin, author.surname);
		    Menu::eraseLine("Enter authors lastname: " + author.surname);
		    
		    authors.push_back(author);
		}
    }while(cmd != "n");
    
    std::cout << "Enter description: ";
    getline(std::cin, description);
    Menu::eraseLine("Enter description: " + description);
    
    std::string keyword;
    std::cout << "Enter keywords (when finished, type 'stop'): " << std::endl;
    
    while (getline(std::cin, keyword), keyword != "stop")
    {
        keywords.push_back(keyword);
    }
    Menu::eraseLine("stop");
	for (int i = (int)(keywords.size()-1); i >= 0; i--)
	{
		Menu::eraseLine(keywords[i]);
	} 
	Menu::eraseLine("Enter keywords (when finished, type 'stop'): ");
}

void Submission::view()
{    
    std::cout << "Title: " << title << std::endl;
    std::cout << "Authors: " << std::endl;
    for(int i = 0; i < (int)authors.size(); i++)
        std::cout << authors[i].firstname << " " << authors[i].surname << std::endl;
    std::cout << "Description: " << description;
    std::cout << "Keywords: ";
    std::ostringstream ks;
    for (int w = 0; w < (int)keywords.size(); ++w)
    {
    	ks << keywords[w] << (w != (int)(keywords.size()-1) ? ", " : "");
    }
    std::cout << ks.str();
    std::cout << "Status: " << status;
    std::cin.ignore(1, '\n');
    
    // erase display
    Menu::eraseLine("Status: " + status);
    Menu::eraseLine("Keywords: " + ks.str());
    Menu::eraseLine("Description: " + description);
    for (int e = (int)(authors.size()-1); e >= 0; e--)
    {
    	Menu::eraseLine(authors[e].firstname + " " + authors[e].surname);
    }
    Menu::eraseLine("Authors: ");
    Menu::eraseLine("Title: " + title);
}

void Submission::withdraw()
{
    std::cout << "Main Menu < Submissions < Withdraw" << std::endl << std::endl;
    
    // function call to database to delete the file? maybe?


}
void Submission::addReviewer(const std::string &user)
{
	reviewers.push_back(user);
}
int Submission::getReviewerCount() const
{
	return reviewers.size();
}		
bool Submission::hasReviewer(const std::string &user)
{
	// return (std::find(reviewers.begin(), reviewers.end(), user) != reviewers.end());
	for (int i = 0; i < (int)reviewers.size(); ++i)
	{
		if (reviewers[i] == user)
		{
			return true;
		}
	}
	return false;
}
void Submission::removeReviewer(const std::string &user)
{
	if (hasReviewer(user))
	{
		reviewers.erase(std::find(reviewers.begin(), reviewers.end(), user));
	}
}

bool Submission::hasKeyword(const std::string &word) const
{
	return (std::find(keywords.begin(), keywords.end(), word) != keywords.end()); 
}

void Submission::addAuthor(const std::string &first, const std::string &last)
{
	Fullname tmp;
	tmp.firstname = first;
	tmp.surname = last;
	authors.push_back(tmp);
}

void Submission::addComment(const std::string &user, const std::string &msg)
{
    Comment com;
    com.comment = msg;
    com.username = user;
    comments.push_back(com);
}

void Submission::getComments(std::vector<Comment> &list)
{
	list = comments;
}

void Submission::displayComments()
{
    for(int i = 0; i < (int)comments.size(); i++)
        std::cout << comments[i].username << ": " << comments[i].comment << std::endl;
}

sf::Packet & operator<<(sf::Packet &packet, const Submission &sub)
{
	packet << sub.reviewed << sub.filename << sub.title << sub.description;
	packet << sub.conference << sub.status << sub.uni;
	sf::Int32 val = sub.deadline;
	packet << val;
	packet << (int)sub.authors.size();
	for (int i = 0; i < (int)sub.authors.size(); ++i)
	{
		packet << sub.authors[i].firstname << sub.authors[i].surname;
	}
	packet << (int)sub.keywords.size();
	for (int j = 0; j < (int)sub.keywords.size(); ++j)
	{
		packet << sub.keywords[j];
	}
	packet << (int)sub.comments.size();
	for (int k = 0; k < (int)sub.comments.size(); ++k)
	{
		packet << sub.comments[k].username << sub.comments[k].comment;
	}
	packet << (int)sub.reviewers.size();
	for (int o = 0; o < (int)sub.reviewers.size(); ++o)
	{
		packet << sub.reviewers[o];
	}
	return packet;
}
sf::Packet & operator>>(sf::Packet &packet, Submission &sub)
{
	sub.authors.clear();
	sub.keywords.clear();
	sub.comments.clear();
	
	packet >> sub.reviewed >> sub.filename >> sub.title >> sub.description;
	packet >> sub.conference >> sub.status >> sub.uni;
	sf::Int32 val;
	packet >> val;
	sub.deadline = val;
	int authorSize = 0;
	packet >> authorSize;
	for (int a = 0; a < authorSize; ++a)
	{
		Fullname tmpAuthor;
		packet >> tmpAuthor.firstname >> tmpAuthor.surname;
		sub.authors.push_back(tmpAuthor);
	}
	int keywordSize = 0;
	packet >> keywordSize;
	for (int b = 0; b < keywordSize; ++b)
	{
		std::string tmpWord;
		packet >> tmpWord;
		sub.keywords.push_back(tmpWord);
	}
	int commentSize = 0;
	packet >> commentSize;
	for (int c = 0; c < commentSize; ++c)
	{
		Comment tmpComment;
		packet >> tmpComment.username >> tmpComment.comment;
		sub.comments.push_back(tmpComment);
	}
	int reviewersSize = 0;
	packet >> reviewersSize;
	for (int d = 0; d < reviewersSize; ++d)
	{
		std::string tmpReviewer;
		packet >> tmpReviewer;
		sub.reviewers.push_back(tmpReviewer);
	}
	return packet;
}

void Submission::writeFile(std::ofstream &ofs) const
{
	appendData<bool>(ofs, this->reviewed);
	appendString(ofs, this->filename);
	appendString(ofs, this->title);
	appendString(ofs, this->description);
	appendString(ofs, this->conference);
	appendString(ofs, this->status);
	appendString(ofs, this->uni);
	appendData<time_t>(ofs, this->deadline);
	appendClassVector<Fullname>(ofs, this->authors);
	appendStringVector(ofs, this->keywords);
	appendClassVector<Comment>(ofs, this->comments);
	appendStringVector(ofs, this->reviewers);
}
void Submission::readFile(std::ifstream &ifs)
{
	readData<bool>(ifs, this->reviewed);
	readString(ifs, this->filename);
	readString(ifs, this->title);
	readString(ifs, this->description);
	readString(ifs, this->conference);
	readString(ifs, this->status);
	readString(ifs, this->uni);
	readData<time_t>(ifs, this->deadline);
	readClassVector<Fullname>(ifs, this->authors);
	readStringVector(ifs, this->keywords);
	readClassVector<Comment>(ifs, this->comments);	
	readStringVector(ifs, this->reviewers);
}

Comment::Comment(const Comment &other)
{
	username = other.username;
	comment = other.comment;
}	
Comment & Comment::operator=(const Comment &other)
{
	username = other.username;
	comment = other.comment;
	return *this;
}
void Comment::writeFile(std::ofstream &ofs) const
{
	appendString(ofs, this->username);
	appendString(ofs, this->comment);
}
void Comment::readFile(std::ifstream &ifs)
{
	readString(ifs, this->username);
	readString(ifs, this->comment);	
}

sf::Packet & operator<<(sf::Packet &packet, const Comment &com)
{
	packet << com.username << com.comment;
	return packet;
}
sf::Packet & operator>>(sf::Packet &packet, Comment &com)
{
	packet >> com.username >> com.comment;
	return packet;
}

Fullname::Fullname(const Fullname &other)
{
	firstname = other.firstname;
	surname = other.surname;
}
void Fullname::writeFile(std::ofstream &ofs) const
{
	appendString(ofs, this->firstname);
	appendString(ofs, this->surname);
}
void Fullname::readFile(std::ifstream &ifs)
{
	readString(ifs, this->firstname);
	readString(ifs, this->surname);	
}
Fullname & Fullname::operator=(const Fullname &other)
{
	firstname = other.firstname;
	surname = other.surname;
	return *this;
}

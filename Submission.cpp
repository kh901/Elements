#include "Submission.h"

Submission::Submission()
{
    reviewed = false;
    status = "Pending";
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
    
    std::cout << "Enter title: ";
    getline(std::cin, title);
    
    Fullname author;
    std::string cmd;
    do
    {
        std::cout << "Enter authors firstname: ";
        getline(std::cin, author.firstname);
        std::cout << "Enter authors lastname: ";
        getline(std::cin, author.surname);
        
        authors.push_back(author);
      
        do
        {
            std::cout << "Do you want to add another author to this paper? (y/n)";
            getline(std::cin, cmd);
            if(cmd != "y" && cmd != "n")
                std::cout << "Invalid option, please try again" << std::endl;
        }while(cmd != "y" && cmd != "n");
    }while(cmd != "n");
    
    std::cout << "Enter description: ";
    getline(std::cin, description);
    
    std::string keyword;
    std::cout << "Enter keywords (when finished, type 'stop'): " << std::endl;
    
    while (getline(std::cin, keyword), keyword != "stop")
    {
        keywords.push_back(keyword);
    }
}

void Submission::view()
{    
    std::cout << "Title: " << title << std::endl;
    std::cout << "Authors: ";
    for(int i = 0; i < (int)authors.size(); i++)
        std::cout << authors[i].firstname << " " << authors[i].surname << ", ";
    std::cout << std::endl << "Description: " << description << std::endl;
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
int Submission::getReviewerCount()
{
	return reviewers.size();
}		

void Submission::addAuthor(const std::string &first, const std::string &last)
{
	Fullname tmp;
	tmp.firstname = first;
	tmp.surname = last;
	authors.push_back(tmp);
}

void Submission::addComment(Account account)
{
    Comment com;
    std::cout << "Add comment about this paper: ";
    getline(std::cin, com.comment);
    com.username = account.getUsername();
    comments.push_back(com);
    
    // send submission back to server controller
}

void Submission::displayComments()
{
    for(int i = 0; i < (int)comments.size(); i++)
        std::cout << comments[i].username << ": " << comments[i].comment << std::endl;
}

sf::Packet & operator<<(sf::Packet &packet, const Submission &sub)
{
	packet << sub.reviewed << sub.filename << sub.title << sub.description;
	packet << sub.conference << sub.status;
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
	packet >> sub.conference >> sub.status;
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

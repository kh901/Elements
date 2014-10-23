#include "Submission.h"

Submission::Submission()
{
    filename = "";
    title = "";
    description = "";
    reviewed = false;
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
	return *this;
}

void Submission::setReviewed(){
	reviewed = true;
}

std::string Submission::getTitle(){
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
    std::cout << "Main Menu > Submissions > Submit a paper" << std::endl << std::endl;
    
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
        
    std::cout << "Do you want to submit? (y/n): ";
    std::string option;
    getline(std::cin, option);
    
}

void Submission::view()
{    
    std::cout << "Title: " << title << std::endl;
    std::cout << "Authors: ";
    for(int i = 0; i < authors.size(); i++)
        std::cout << authors[i].firstname << " " << authors[i].surname << ", ";
    std::cout << std::endl << "Description: " << description << std::endl;
}

void Submission::withdraw()
{
    std::cout << "Main Menu < Submissions < Withdraw" << std::endl << std::endl;
    
    // function call to database to delete the file? maybe?

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
    for(int i = 0; i < comments.size(); i++)
        std::cout << comments[i].username << ": " << comments[i].comment << std::endl;
}

void Submission::writeFile(std::ofstream &ofs) const
{
	appendData<bool>(ofs, this->reviewed);
	appendString(ofs, this->filename);
	appendString(ofs, this->title);
	appendString(ofs, this->description);
	appendClassVector(ofs, this->authors);
	appendStringVector(ofs, this->keywords);
	appendClassVector<Comment>(ofs, this->comments);
}
void Submission::readFile(std::ifstream &ifs)
{
	readData<bool>(ifs, this->reviewed);
	readString(ifs, this->filename);
	readString(ifs, this->title);
	readString(ifs, this->description);
	readClassVector(ifs, this->authors);
	readStringVector(ifs, this->keywords);
	readClassVector<Comment>(ifs, this->comments);	
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

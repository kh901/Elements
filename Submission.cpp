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
<<<<<<< HEAD
void Submission::setReviewed()
=======

vector<std::string> getAuthors(){
	return authors;
}


void Submission::setReviewed()
>>>>>>> 929a63bc8383f7910acc5d5acd59a705985072ff
{
    reviewed = true;
}

void Submission::submit()
{
    std::cout << "Main Menu > Submissions > Submit a paper" << std::endl << std::endl;
    
    std::cout << "Enter paper name: ";
    getline(std::cin, filename);
    
    std::cout << "Enter title: ";
    getline(std::cin, title);
    
    std::string author;
    do
    {
        std::cout << "Enter authors name: ";
        getline(std::cin, author);
        authors.push_back(author);
      
        do
        {
            std::cout << "Do you want to add another author to this paper? (y/n)";
            getline(std::cin, author);
            if(author != "y" && author != "n")
                std::cout << "Invalid option, please try again" << std::endl;
        }while(author != "y" && author != "n");
    }while(author != "n");
    
    std::cout << "Enter description: ";
    getline(std::cin, description);
    
    std::string keyword, str;
    std::cout << "Enter keywords (when finished, type 'stop'): " << std::endl;
    
    while (getline(std::cin, keyword), keyword != "stop")
    {
        keywords.push_back(str);
    }
        
    std::cout << "Do you want to submit? (y/n): ";
    std::string option;
    getline(std::cin, option);
    
    if(option[0] == 'n')
    {
        submitted = false;
    }
    else
    {
        // send paper through network to server controller
        submitted = true;
    }
    
}

void Submission::view()
{    
    std::cout << "Title: " << title << std::endl;
    std::cout << "Authors: ";
    for(int i = 0; i < authors.size(); i++)
        std::cout << authors[i] << ", ";
    std::cout << std::endl << "Description: " << description << std::endl;
}

void Submission::withdraw()
{
    std::cout << "Main Menu < Submissions < Withdraw" << std::endl << std::endl;
    
    // function call to database to delete the file? maybe?
    submitted = false;
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
	appendData<bool>(ofs, this->submitted);
	appendData<bool>(ofs, this->reviewed);
	appendString(ofs, this->filename);
	appendString(ofs, this->title);
	appendString(ofs, this->description);
	appendStringVector(ofs, this->authors);
	appendStringVector(ofs, this->keywords);
	appendClassVector<Comment>(ofs, this->comments);
}
void Submission::readFile(std::ifstream &ifs)
{
	readData<bool>(ifs, this->submitted);
	readData<bool>(ifs, this->reviewed);
	readString(ifs, this->filename);
	readString(ifs, this->title);
	readString(ifs, this->description);
	readStringVector(ifs, this->authors);
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

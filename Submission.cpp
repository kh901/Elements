#include "Submission.h"

Submission::Submission()
{
    submitted = false;
    filename = "";
    title = "";
    description = "";
    reviewed = false;
}

void Submission::setReviewed(bool reviewed)
{
    this->reviewed = reviewed;
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
    std::cout << "Enter keywords: ";
    getline(std::cin, keyword);
    while (std::cin >> str)
    {
        keywords.push_back(str);
    }
    std::cin.ignore(1, '\n');
        
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

void Submission::writeFile(std::ofstream &ofs)
{
	appendData<bool>(ofs, this->submitted);
	appendData<bool>(ofs, this->reviewed);
	appendData<std::string>(ofs, this->filename);
	appendData<std::string>(ofs, this->title);
	appendData<std::string>(ofs, this->description);
	appendVector<std::string>(ofs, this->authors);
	appendVector<std::string>(ofs, this->keywords);
	appendVector<Comment>(ofs, this->comments);
}
bool Submission::readFile(std::ifstream &ifs)
{
	readData<bool>(ifs, this->submitted);
	readData<bool>(ifs, this->reviewed);
	readData<std::string>(ifs, this->filename);
	readData<std::string>(ifs, this->title);
	readData<std::string>(ifs, this->description);
	readVector<std::string>(ifs, this->authors);
	readVector<std::string>(ifs, this->keywords);
	readVector<Comment>(ifs, this->comments);	
}

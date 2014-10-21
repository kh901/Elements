#include "Submission.h"

Submission::Submission()
{
    submitted = false;
    filename = "";
    title = "";
    description = "";
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
    
    std::cout << "Do you want to submit? (y/n): ";
    std::string option;
    getline(std::cin, option);
    
    if(option[0] == 'n')
    {
        submitted = false;
    }
    else
    {
        // send paper through network to store in database
        
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
}

void Submission::displayComments()
{
    for(int i = 0; i < comments.size(); i++)
        cout << comments[i].username << ": " << comments[i].comment << std::endl;
}

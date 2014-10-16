#include "Submission.h"

Submission::Submission()
{
    submitted = false;
    filename = "";
    title = "";
    author = "";
    description = "";
}

void Submission::submit()
{
    std::cout << "Main Menu > Submissions > Submit a paper" << std::endl << std::endl;
    
    std::cout << "Enter paper name: ";
    std::cin >> filename;
    
    std::cout << "Enter title: ";
    std::cin >> title;
    
    std::cout << "Enter author: ";
    std::cin >> author;
    
    std::cout << "Enter descritpion: ";
    std::cin >> description;
    
    std::cout << "Do you want to submit? (y/n): ";
    std::string option;
    std::cin >> option;
    
    if(option == "n")
    {
        submitted = false;
        return;
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
    std::cout << "Author: " << author << std::endl;
    std::cout << "Description: " << description << std::endl;
}

void Submission::withdraw()
{
    std::cout << "Main Menu < Submissions < Withdraw" << std::endl << std::endl;
    
    // function call to database to delete the file? maybe?
    submitted = false;
}


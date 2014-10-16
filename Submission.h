#include <iostream>
#include <fstream>

#ifndef SUBMISSION_H
#define SUBMISSION_H

class Submission
{
    public:
        Submission();
        void submit();
        void view();
        void withdraw();
        
    private:
        bool submitted;
        //std::fstream paper; might not need it, if filename is used as reference to database for paper
        std::string filename;
        
        std::string author;
        std::string title;
        std::string description;
    
};

#endif


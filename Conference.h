#include <string>
#include <iostream>
#include <fstream>
#include "File.h"

#ifndef CONFERENCE_H
#define CONFERENCE_H

class Conference : public FileIO
{
    public:
        Conference();
        
        
        enum phase {
            Phase_Submission,
            Phase_Allocation,
            Phase_Reviewing,
            Phase_Finalising,
            Phase_Completed
        };
        
        // write and read this class to a binary file stream
		void writeFile(std::ofstream &);
		bool readFile(std::ifstream &);
    private:
        std::string name;
        std::string date;
        std::string location;
        
};
#endif

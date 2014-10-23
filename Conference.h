#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "File.h"
#include "Phase.h"

#ifndef CONFERENCE_H
#define CONFERENCE_H

class Conference : public FileIO
{
    public:
        Conference();
        Conference(const Conference &);
        
		void setName(const std::string &);
		std::string getName();
        void setDate(const std::string &);
        std::string getDate();
        void setLocation(const std::string &);
        std::string getLocation();
        void setChairman(const std::string &);
        std::string getChairman();
        
        std::string getCurrentPhase();
        void advancePhase();
        
		void addReviewer(const std::string &);
		void printReviewers();
        void addSubchair(const std::string &);
        void printSubchairs();

        // write and read this class to a binary file stream
		void writeFile(std::ofstream &) const;
		void readFile(std::ifstream &);
        
    private:
        std::string name;
        std::string date;
        std::string location;
        Phase phase;
		std::vector<std::string> reviewers;
		std::vector<std::string> subchairs;
		std::string chairman;
};
#endif

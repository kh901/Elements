#include <string>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include "File.h"
#include "Submission.h"
#include "Phase.h"
#include "common.h"
#include <SFML/Network.hpp>

#ifndef CONFERENCE_H
#define CONFERENCE_H

#define CONFERENCE_DEFAULT_MAX_PAPERS_PER_REVIEWER 3 
#define CONFERENCE_DEFAULT_MAX_REVIEWERS_PER_PAPER 3

class Conference : public FileIO
{
    public:
        Conference();
        Conference(const Conference &);
        
		void setName(const std::string &);
		std::string getName() const;
        void setDate(const std::string &);
        std::string getDate() const;
        void setLocation(const std::string &);
        std::string getLocation() const;
        void setChairman(const std::string &);
        std::string getChairman() const;
        
        std::string getCurrentPhase();
        void advancePhase();
        void adjustDeadlineDays(const int);
        bool hasDeadlinePassed(const time_t);
        
        void setMaxReviewedPapers(const int);
        int getMaxReviewedPapers();
        void setMaxPaperReviewers(const int);
        int getMaxPaperReviewers();
        
		void addReviewer(const std::string &);
		void getReviewers(std::vector<std::string>&);
		void printReviewers();
        void addSubchair(const std::string &);
        void getSubchairs(std::vector<std::string>&);
        void printSubchairs();

		Conference & operator=(const Conference &);

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
		
		time_t deadline;
		
		// Conference settings
		int maxReviewersPerPaper;
		int maxPapersPerReviewer;
		
		friend sf::Packet & operator<<(sf::Packet &packet, const Conference &);
		friend sf::Packet & operator>>(sf::Packet &packet, Conference &);	
};
#endif

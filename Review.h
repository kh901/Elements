#ifndef REVIEW_H
#define REVIEW_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "File.h"
#include <SFML/Network.hpp>
#include <sstream>

#define REVIEW_MAX_ID_LEN 10

class Review : public FileIO
{
	public:
		Review();
		Review(const Review &);
		Review & operator=(const Review &);
		
		void generateReviewID();
		void setReviewID(const std::string&);
		std::string getReviewID();
		void setTitle(const std::string&);
		std::string getTitle();
		void setPCMember(const std::string&);
		std::string getPCMember();
		void setReviewer(const std::string&, const std::string&, const std::string&);
		std::string getReviewerFirst();
		std::string getReviewerLast();
		std::string getReviewerEmail();
		void setStrengths(const std::string&);
		std::string getStrengths();
		void setWeaknesses(const std::string&);
		std::string getWeaknesses();
		void setSuggestions(const std::string&);
		std::string getSuggestions();
		void setShortPaper(const std::string&);
		std::string getShortPaper();
		void setBestPaper(const std::string&);
		std::string getBestPaper();
		void setRemarks(const std::string&);
		std::string getRemarks();
		void setConference(const std::string &);
		std::string getConference();

		void setOverallEvaluation(int&);
		int getOverallEvaluation();
		void setReviewerConfidence(int&);
		int getReviewerConfidence();
		void setRelevance(int&);
		int getRelevance();
		void setOriginality(int&);
		int getOriginality();
		void setSignificance(int&);
		int getSignificance();
		void setPresentation(int&);
		int getPresentation();
		void setTechnicalQuality(int&);
		int getTechnicalQuality();
		void setEvaluation(int&);
		int getEvaluation();

		// write and read this class to a binary file stream
		void writeFile(std::ofstream &) const;
		void readFile(std::ifstream &);
	private:
		std::string reviewID;
		std::string conference;
		std::string title;
		std::string pcMember;
		std::string strengths;
		std::string weaknesses;
		std::string suggestions;
		std::string shortPaper;
		std::string bestPaper;
		std::string remarks;
		std::string reviewerFirstName;
		std::string reviewerLastName;
		std::string reviewerEmail;
		int overallEvaluation;
		int reviewerConfidence;
		int relevance;
		int originality;
		int significance;
		int presentation;
		int technicalQuality;
		int evaluation;
		
		friend sf::Packet & operator<<(sf::Packet &packet, const Review &);
		friend sf::Packet & operator>>(sf::Packet &packet, Review &);
};
#endif

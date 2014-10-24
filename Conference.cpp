#include "Conference.h"
Conference::Conference()
{
	maxReviewersPerPaper = CONFERENCE_DEFAULT_MAX_REVIEWERS_PER_PAPER;
	maxPapersPerReviewer = CONFERENCE_DEFAULT_MAX_PAPERS_PER_REVIEWER;
}
Conference::Conference(const Conference &other)
{
	name = other.name;
    date = other.date;
    location = other.location;
    reviewers = other.reviewers;
	subchairs = other.subchairs;
	chairman = other.chairman;
	maxReviewersPerPaper = other.maxReviewersPerPaper;
	maxPapersPerReviewer = other.maxPapersPerReviewer;
}

std::string Conference::getCurrentPhase()
{
    return phase.getCurrentPhase();
}

void Conference::advancePhase()
{
    phase.advancePhase();
}

void Conference::setName(const std::string &aName)
{
	name = aName;	
}
std::string Conference::getName()
{
	return name;
}

void Conference::setDate(const std::string &aDate)
{
    date = aDate;
}

std::string Conference::getDate()
{
    return date;
}

void Conference::setLocation(const std::string &aLocation)
{
    location = aLocation;
}

std::string Conference::getLocation()
{
    return location;
}

void Conference::setChairman(const std::string &aChairman)
{
    chairman = aChairman;
}

std::string Conference::getChairman()
{
    return chairman;
}

void Conference::setMaxReviewedPapers(const int max)
{
	if (max > 0)
	{
		maxPapersPerReviewer = max;
	}
}
void Conference::setMaxPaperReviewers(const int max)
{
	if (max > 0)
	{
		maxReviewersPerPaper = max;
	}
}

void Conference::addReviewer(const std::string &aReviewer)
{
	reviewers.push_back(aReviewer);
}

void Conference::printReviewers()
{
	std::vector<std::string>::iterator it;
	for (it = reviewers.begin(); it != reviewers.end(); ++it)
	{
		std::cout << "Reviewer: " << *it << std::endl;
	}
}

void Conference::addSubchair(const std::string &aSubchair)
{
	reviewers.push_back(aSubchair);
}

void Conference::printSubchairs()
{
    std::vector<std::string>::iterator it;
    for(it = subchairs.begin(); it != reviewers.end(); ++it)
    {
        std::cout << "Subchair: " << *it << std::endl;
    }
}

void Conference::writeFile(std::ofstream &ofs) const
{
	appendString(ofs, this->name);
	appendString(ofs, this->date);
	appendString(ofs, this->location);	
	appendClass<Phase>(ofs, this->phase);
	appendStringVector(ofs, this->reviewers);
	appendStringVector(ofs, this->subchairs);
	appendString(ofs, this->chairman);
	appendData<int>(ofs, this->maxReviewersPerPaper);
	appendData<int>(ofs, this->maxPapersPerReviewer);
}

void Conference::readFile(std::ifstream &ifs)
{
	readString(ifs, this->name);
	readString(ifs, this->date);
	readString(ifs, this->location);
	readClass<Phase>(ifs, this->phase);
	readStringVector(ifs, this->reviewers);
	readStringVector(ifs, this->subchairs);
	readString(ifs, this->chairman);
	readData<int>(ifs, this->maxReviewersPerPaper);
	readData<int>(ifs, this->maxPapersPerReviewer);
}

#include "Conference.h"
Conference::Conference()
{
	currentPhase = Phase_Submission;
}
Conference::Conference(const Conference &other)
{
	name = other.name;
    date = other.date;
    location = other.location;
    currentPhase = other.currentPhase;
    reviewers = other.reviewers;
	subchairs = other.subchairs;
	chairman = other.chairman;
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

void Conference::setCurrentPhase(const Phase &aPhase)
{
    currentPhase = aPhase;
}

Conference::Phase Conference::getCurrentPhase()
{
    return currentPhase;
}

void Conference::advancePhase()
{

}

void Conference::setChairman(const std::string &aChairman)
{
    chairman = aChairman;
}

std::string Conference::getChairman()
{
    return chairman;
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
	appendData<Phase>(ofs, this->currentPhase);
	appendStringVector(ofs, this->reviewers);
	appendStringVector(ofs, this->subchairs);
	appendString(ofs, this->chairman);
}

void Conference::readFile(std::ifstream &ifs)
{
	readString(ifs, this->name);
	readString(ifs, this->date);
	readString(ifs, this->location);
	readData<Phase>(ifs, this->currentPhase);
	readStringVector(ifs, this->reviewers);
	readStringVector(ifs, this->subchairs);
	readString(ifs, this->chairman);
}

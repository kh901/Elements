#include "Conference.h"

void Conference::setName(const std::string &aName)
{
	name = aName;	
}
std::string Conference::getName()
{
	return name;
}

void addReviewer(const std::string &aReviewer)
{
	reviewers.push_back(aReviewer);
}
void printReviewers()
{
	
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

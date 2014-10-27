#include "Conference.h"
Conference::Conference()
{
	maxReviewersPerPaper = CONFERENCE_DEFAULT_MAX_REVIEWERS_PER_PAPER;
	maxPapersPerReviewer = CONFERENCE_DEFAULT_MAX_PAPERS_PER_REVIEWER;
	deadline = getTimeValue();
}
Conference::Conference(const Conference &other)
{
	name = other.name;
    date = other.date;
    location = other.location;
    reviewers = other.reviewers;
	subchairs = other.subchairs;
	chairman = other.chairman;
	phase.setCurrentId(other.phase.getCurrentId());
	maxReviewersPerPaper = other.maxReviewersPerPaper;
	maxPapersPerReviewer = other.maxPapersPerReviewer;
	deadline = other.deadline;
}

Conference & Conference::operator=(const Conference &other)
{
	name = other.name;
    date = other.date;
    location = other.location;
    reviewers = other.reviewers;
	subchairs = other.subchairs;
	chairman = other.chairman;
	phase.setCurrentId(other.phase.getCurrentId());
	maxReviewersPerPaper = other.maxReviewersPerPaper;
	maxPapersPerReviewer = other.maxPapersPerReviewer;
	deadline = other.deadline;
	return *this;
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
std::string Conference::getName() const
{
	return name;
}

void Conference::setDate(const std::string &aDate)
{
    date = aDate;
}

std::string Conference::getDate() const
{
    return date;
}

void Conference::setLocation(const std::string &aLocation)
{
    location = aLocation;
}

std::string Conference::getLocation() const
{
    return location;
}

void Conference::setChairman(const std::string &aChairman)
{
    chairman = aChairman;
}

std::string Conference::getChairman() const
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
int Conference::getMaxReviewedPapers()
{
	return maxPapersPerReviewer;
}
int Conference::getMaxPaperReviewers()
{
	return maxReviewersPerPaper;
}
void Conference::adjustDeadlineDays(const int num)
{
	if (num > 0)
	{
		deadline = getDaysAhead(getTimeValue(), num);
	}
}
bool Conference::hasDeadlinePassed(const time_t cur)
{
	return (cur >= deadline);
}

void Conference::getReviewers(std::vector<std::string> &list)
{
	list = reviewers;
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
	appendData<time_t>(ofs, this->deadline);
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
	readData<time_t>(ifs, this->deadline);
}

sf::Packet & operator<<(sf::Packet &packet, const Conference &conf)
{
	packet << conf.name << conf.date << conf.location << conf.phase.getCurrentId();
	// send reviewers
	packet << (int)conf.reviewers.size();
	for (int i = 0; i < (int)conf.reviewers.size(); ++i)
	{
		packet << conf.reviewers[i];
	} 
	// send subchairs
	packet << (int)conf.subchairs.size();
	for (int i = 0; i < (int)conf.subchairs.size(); ++i)
	{
		packet << conf.subchairs[i];
	} 
	packet << conf.chairman;
	// send allocation parameters
	packet << conf.maxReviewersPerPaper;
	packet << conf.maxPapersPerReviewer;
	sf::Int32 val = conf.deadline;
	packet << val;
	return packet;
}
sf::Packet & operator>>(sf::Packet &packet, Conference &conf)
{
	conf.reviewers.clear();
	conf.subchairs.clear();
	int curPhase = 0;
	packet >> conf.name >> conf.date >> conf.location >> curPhase;
	conf.phase.setCurrentId(curPhase);
	// get reviewers
	int reviewerSize = 0;
	packet >> reviewerSize;
	for (int i = 0; i < reviewerSize; ++i)
	{
		std::string tmpReview;
		packet >> tmpReview;
		conf.reviewers.push_back(tmpReview);
	} 
	// get subchairs
	int subchairSize = 0;
	packet >> subchairSize;
	for (int i = 0; i < subchairSize; ++i)
	{
		std::string tmpSub;
		packet >> tmpSub;
		conf.subchairs.push_back(tmpSub);
	} 
	packet >> conf.chairman;
	// get allocation parameters
	packet >> conf.maxReviewersPerPaper;
	packet >> conf.maxPapersPerReviewer;
	sf::Int32 val;
	packet >> val;
	conf.deadline = val;
	return packet;
}

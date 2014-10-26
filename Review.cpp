#include "Review.h"

Review::Review()
{
	overallEvaluation = 0;
	reviewerConfidence = 0;
	relevance = 0;
	originality = 0;
	significance = 0;
	presentation = 0;
	technicalQuality = 0;
	evaluation = 0;
}

Review::Review(const Review &other)
{
	this->operator=(other);
}

Review & Review::operator=(const Review &other)
{
	reviewID = other.reviewID;
	title = other.title;
	pcMember = other.pcMember;
	reviewerFirstName = other.reviewerFirstName;
	reviewerLastName = other.reviewerLastName;
	reviewerEmail = other.reviewerEmail;
	strengths = other.strengths;
	weaknesses = other.weaknesses;
	suggestions = other.suggestions;
	shortPaper = other.shortPaper;
	bestPaper = other.bestPaper;
	remarks = other.remarks;
	overallEvaluation = other.overallEvaluation;
	reviewerConfidence = other.reviewerConfidence;
	relevance = other.relevance;
	originality = other.originality;
	significance = other.significance;
	presentation = other.presentation;
	technicalQuality = other.technicalQuality;
	evaluation = other.evaluation;
	return *this;
}

void Review::setReviewID(const std::string &aReviewID)
{
	reviewID = aReviewID;
}

void Review::setTitle(const std::string &aTitle)
{
	title = aTitle;
}

void Review::setPCMember(const std::string &aPCMember)
{
	pcMember = aPCMember;
}

void Review::setReviewer(const std::string &firstname, const std::string &lastname, const std::string &email)
{
	reviewerFirstName = firstname;
	reviewerLastName = lastname;
	reviewerEmail = email;
}

void Review::setStrengths(const std::string &aStrengths)
{
	strengths = aStrengths;
}

void Review::setWeaknesses(const std::string &aWeaknesses)
{
	weaknesses = aWeaknesses;
}

void Review::setSuggestions(const std::string &aSuggestions)
{
	suggestions = aSuggestions;
}

void Review::setShortPaper(const std::string &aShortPaper)
{
	shortPaper = aShortPaper;
}

void Review::setBestPaper(const std::string &aBestPaper)
{
	bestPaper = aBestPaper;
}

void Review::setRemarks(const std::string &aRemarks)
{
	remarks = aRemarks;
}

void Review::setOverallEvaluation(int &aOverall)
{
	overallEvaluation = aOverall;
}

void Review::setReviewerConfidence(int &aRevCon)
{
	reviewerConfidence = aRevCon;
}

void Review::setRelevance(int &aRelevance)
{
	relevance = aRelevance;
}

void Review::setSignificance(int &aSignificance)
{
	significance = aSignificance;
}

void Review::setOriginality(int &aOriginal)
{
	originality = aOriginal;
}

void Review::setPresentation(int &aPresent)
{
	presentation = aPresent;
}

void Review::setTechnicalQuality(int &aTech)
{
	technicalQuality = aTech;
}

void Review::setEvaluation(int &aEval)
{
	evaluation = aEval;
}

std::string Review::getReviewID()
{
	return reviewID;
}

std::string Review::getTitle()
{
	return title;
}

std::string Review::getStrengths()
{
	return strengths;
}

std::string Review::getWeaknesses()
{
	return weaknesses;
}

std::string Review::getSuggestions()
{
	return suggestions;
}

std::string Review::getShortPaper()
{
	return shortPaper;
}

std::string Review::getBestPaper()
{
	return bestPaper;
}

std::string Review::getRemarks()
{
	return remarks;
}

int Review::getOverallEvaluation()
{
	return overallEvaluation;
}

int Review::getReviewerConfidence()
{
	return reviewerConfidence;
}

int Review::getRelevance()
{
	return relevance;
}

int Review::getOriginality()
{
	return originality;
}

int Review::getSignificance()
{
	return significance;
}

int Review::getPresentation()
{
	return presentation;
}

int Review::getTechnicalQuality()
{
	return technicalQuality;
}

int Review::getEvaluation()
{
	return evaluation;
}

std::string Review::getReviewerFirst()
{
	return reviewerFirstName;
}

std::string Review::getReviewerLast()
{
	return reviewerLastName;
}

std::string Review::getReviewerEmail()
{
	return reviewerEmail;
}

void Review::writeFile(std::ofstream &ofs) const
{
	appendString(ofs, this->reviewID);
	appendString(ofs, this->title);
	appendString(ofs, this->pcMember);
	appendString(ofs, this->reviewerFirstName);
	appendString(ofs, this->reviewerLastName);
	appendString(ofs, this->reviewerEmail);
	appendString(ofs, this->strengths);
	appendString(ofs, this->weaknesses);
	appendString(ofs, this->suggestions);
	appendString(ofs, this->shortPaper);
	appendString(ofs, this->bestPaper);
	appendString(ofs, this->remarks);
	appendData<int>(ofs, this->overallEvaluation);
	appendData<int>(ofs, this->reviewerConfidence);
	appendData<int>(ofs, this->relevance);
	appendData<int>(ofs, this->originality);
	appendData<int>(ofs, this->significance);
	appendData<int>(ofs, this->presentation);
	appendData<int>(ofs, this->technicalQuality);
	appendData<int>(ofs, this->evaluation);
}
void Review::readFile(std::ifstream &ifs)
{
	readString(ifs, this->reviewID);
	readString(ifs, this->title);
	readString(ifs, this->pcMember);
	readString(ifs, this->reviewerFirstName);
	readString(ifs, this->reviewerLastName);
	readString(ifs, this->reviewerEmail);
	readString(ifs, this->strengths);
	readString(ifs, this->weaknesses);
	readString(ifs, this->suggestions);
	readString(ifs, this->shortPaper);
	readString(ifs, this->bestPaper);
	readString(ifs, this->remarks);
	readData<int>(ifs, this->overallEvaluation);
	readData<int>(ifs, this->reviewerConfidence);
	readData<int>(ifs, this->relevance);
	readData<int>(ifs, this->originality);
	readData<int>(ifs, this->significance);
	readData<int>(ifs, this->presentation);
	readData<int>(ifs, this->technicalQuality);
	readData<int>(ifs, this->evaluation);
}
sf::Packet & operator<<(sf::Packet &packet, const Review &rev)
{
	packet << rev.reviewID << rev.title << rev.pcMember << rev.strengths;
	packet << rev.weaknesses << rev.suggestions << rev.shortPaper;
	packet << rev.bestPaper << rev.remarks << rev.reviewerFirstName;	
	packet << rev.reviewerLastName << rev.reviewerEmail;
	packet << rev.overallEvaluation << rev.reviewerConfidence;
	packet << rev.relevance << rev.originality;
	packet << rev.significance << rev.presentation;
	packet << rev.technicalQuality << rev.evaluation;
	return packet;
}
sf::Packet & operator>>(sf::Packet &packet, Review &rev)
{
	packet >> rev.reviewID >> rev.title >> rev.pcMember >> rev.strengths;
	packet >> rev.weaknesses >> rev.suggestions >> rev.shortPaper;
	packet >> rev.bestPaper >> rev.remarks >> rev.reviewerFirstName;	
	packet >> rev.reviewerLastName >> rev.reviewerEmail;
	packet >> rev.overallEvaluation >> rev.reviewerConfidence;
	packet >> rev.relevance >> rev.originality;
	packet >> rev.significance >> rev.presentation;
	packet >> rev.technicalQuality >> rev.evaluation;
	return packet;
}
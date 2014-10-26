#include "Review.h"

Review::Review()
{
	reviewID = "";
	title = "";
	pcMember = "";
	reviewerFirstName = "";
	reviewerLastName = "";
	reviewerEmail = ""
	strengths = "";
	weaknesses = "";
	suggestions = "";
	shortPaper = "";
	bestPaper = "";
	remarks = "";
	overallEvaluation = 0;
	reviewerConfidence = 0;
	relevance = 0;
	originality = 0;
	significance = 0;
	presentation = 0;
	technicalQuality = 0;
	evaluation = 0;
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
	reviewer.Email = email;
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

void Review::setOverallEvalutaion(int &aOverall)
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

void Review::setTechnicalQulaity(int &aTech)
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

std::string Review::getReviewer()
{
	return reviewer;
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

std::string Review::remarks()
{
	return remarks;
}

int Review::getOverallEvaluation()
{
	return overallEvalution;
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
	appendData<int>(ofs, this ->significance);
	appendData<int>(ofs, this-> presentation);
	appedData<int>(ofs, this->technicalQuality);
	appendData<int>(ofs, this->evaluation);
}
void Review::readFile(std::ifstream &ifs)
{
	readString(ifs, this->reviewID);
	readString(ifs, this->title);
	readString(ifs, this->pcMember);
	readString(ifs, this->reviewerFirstName);
	readString(ifs, this->reviewerLastName);
	readstring(ifs, this->reviewerEmail);
	readstring(ifs, this->strengths);
	readstring(ifs, this->weaknesses);
	readstring(ifs, this->suggestions);
	readstring(ifs, this->shortPaper);
	readstring(ifs, this->bestPaper);
	readstring(ifs, this->remarks);
	readData<int>(ifs, this->overallEvaluation);
	readData<int>(ifs, this->reviewerConfidence);
	readData<int>(ifs, this->relevance);
	readData<int>(ifs, this->originality);
	readData<int>(ifs, this->significance);
	readData<int>(ifs, this->presentation);
	readData<int>(ifs, this->technicalQuality)
	readData<int>(ifs, this->evaluation);
}


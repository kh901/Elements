#include "Review.h"

Review::Review()
{
	overallEvaluation = 0;
	reviewerConfidence = 0;
	relevance = 3;
	originality = 3;
	significance = 3;
	presentation = 3;
	technicalQuality = 3;
	evaluation = 3;
	final = false;
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
	conference = other.conference;
	final = other.final;
	return *this;
}

void Review::generateReviewID()
{
	std::ostringstream os;
	srand(time(NULL));
	for (int i = 0; i < REVIEW_MAX_ID_LEN; ++i)
	{
		if (i != 4 && i != 5)
		{
			// random digits
			os << rand()%10;
		}
		else
		{
			os << ':';
		}
	}
	reviewID = os.str();
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

void Review::setConference(const std::string &conf)
{
	conference = conf;
}

void Review::setFinal()
{
	final = true;
}
bool Review::getFinal()
{
	return final;
}

std::string Review::getConference()
{
	return conference;
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
std::string Review::getPCMember()
{
	return pcMember;
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

void Review::view()
{
	bool hasSubReviewer = (reviewerFirstName.length() > 0 && reviewerLastName.length() > 0);
	if (final)
	{
		std::cout << "This is the final review for this paper." << std::endl;
	}
	std::cout << "Review ID: " << reviewID << std::endl;
	std::cout << "Conference: " << conference << std::endl;
	std::cout << "Paper: " << title << std::endl;
	std::cout << "Reviewer: " << pcMember << std::endl;
	std::cout << "Strengths: " << strengths << std::endl;
	std::cout << "Weaknesses: " << weaknesses << std::endl;
	std::cout << "Suggestions: " << suggestions << std::endl;
	std::cout << "Remark for short paper listing: " << shortPaper << std::endl;
	std::cout << "Remark for best paper award: " << bestPaper << std::endl;
	std::cout << "Remarks for PC members: " << remarks << std::endl;
	std::cout << std::endl;
	if (hasSubReviewer)
	{
		std::cout << "Sub reviewer name: " << reviewerFirstName << " " << reviewerLastName << std::endl;
		std::cout << "Sub reviewer email: " << reviewerEmail << std::endl;
		std::cout << std::endl;
	}
	std::ostringstream scores;
	scores << "Overall evaluation: " << overallEvaluation << '\n';
	scores << "Reviewer confidence: " << reviewerConfidence << '\n';
	scores << "Relevance: " << relevance << '\n';
	scores << "Originality: " << originality << '\n';
	scores << "Significance: " << significance << '\n';
	scores << "Presentation: " << presentation << '\n';
	scores << "Technical Quality: " << technicalQuality << '\n';
	scores << "Evaluation: " << evaluation;
	std::cout << scores.str();
	
	std::cin.ignore(1, '\n');
	// clean up display
	scores.str("");
	scores << overallEvaluation;
	Menu::eraseLine("Evaluation: " + scores.str());
	scores.str("");
	scores << technicalQuality;
	Menu::eraseLine("Technical Quality: " + scores.str());
	scores.str("");
	scores << presentation;
	Menu::eraseLine("Presentation: " + scores.str());
	scores.str("");
	scores << significance;
	Menu::eraseLine("Significance: " + scores.str());
	scores.str("");
	scores << originality;
	Menu::eraseLine("Originality: " + scores.str());
	scores.str("");
	scores << relevance;
	Menu::eraseLine("Relevance: " + scores.str());
	scores.str("");
	scores << reviewerConfidence;
	Menu::eraseLine("Reviewer confidence: " + scores.str());
	scores.str("");
	scores << overallEvaluation;
	Menu::eraseLine("Overall evaluation: " + scores.str());
	Menu::eraseLine(0);
	
	if (hasSubReviewer)
	{
		Menu::eraseLine("Sub reviewer email: " + reviewerEmail);
		Menu::eraseLine("Sub reviewer name: " + reviewerFirstName + " " + reviewerLastName);
		Menu::eraseLine(0);
	}
	
	Menu::eraseLine("Remarks for PC members: " + remarks);
	Menu::eraseLine("Remark for best paper award: " + bestPaper);
	Menu::eraseLine("Remark for short paper listing: " + shortPaper);
	Menu::eraseLine("Suggestions: " + suggestions);
	Menu::eraseLine("Weaknesses: " + weaknesses);
	Menu::eraseLine("Strengths: " + strengths);
	Menu::eraseLine("Reviewer: " + pcMember);
	Menu::eraseLine("Paper: " + title);
	Menu::eraseLine( "Conference: " + conference);
	Menu::eraseLine("Review ID: " + reviewID);
	
	if (final)
	{
		Menu::eraseLine("This is the final review for this paper.");
	}
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
	appendString(ofs, this->conference);
	appendData<int>(ofs, this->overallEvaluation);
	appendData<int>(ofs, this->reviewerConfidence);
	appendData<int>(ofs, this->relevance);
	appendData<int>(ofs, this->originality);
	appendData<int>(ofs, this->significance);
	appendData<int>(ofs, this->presentation);
	appendData<int>(ofs, this->technicalQuality);
	appendData<int>(ofs, this->evaluation);
	appendData<bool>(ofs, this->final);
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
	readString(ifs, this->conference);
	readData<int>(ifs, this->overallEvaluation);
	readData<int>(ifs, this->reviewerConfidence);
	readData<int>(ifs, this->relevance);
	readData<int>(ifs, this->originality);
	readData<int>(ifs, this->significance);
	readData<int>(ifs, this->presentation);
	readData<int>(ifs, this->technicalQuality);
	readData<int>(ifs, this->evaluation);
	readData<bool>(ifs, this->final);
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
	packet << rev.conference << rev.final;
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
	packet >> rev.conference >> rev.final;
	return packet;
}

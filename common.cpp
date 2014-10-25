#include "common.h"

// reference from stackoverflow - 
int Ceil(const int &a, const int &b)
{
	return (a/b) + (a % b != 0);
}

std::string getTimestamp()
{
	std::ostringstream os;
	time_t timeVal = time(NULL);
	struct tm * currentTime = localtime(&timeVal);
	int seconds = currentTime->tm_sec;
	int minutes = currentTime->tm_min;
	int hours = currentTime->tm_hour;
	os << std::setfill('0') << std::setw(2) << hours << ':';
	os << std::setfill('0') << std::setw(2) << minutes << ':';
	os << std::setfill('0') << std::setw(2) << seconds;
	return os.str();
}

std::string getDate()
{
	std::ostringstream os;
	time_t timeVal = time(NULL);
	struct tm * currentTime = localtime(&timeVal);
	int day = currentTime->tm_mday;
	int month = 1 + currentTime->tm_mon; 
	int year = 1900 + currentTime->tm_year; 
	os << std::setfill('0') << std::setw(2) << day << '-';
	os << std::setfill('0') << std::setw(2) << month << '-';
	os << std::setfill('0') << std::setw(2) << year;
	return os.str();
}


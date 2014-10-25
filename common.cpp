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
	os << hours << ':' << minutes << ':' << seconds;
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
	os << day << '-' << month << '-' << year;
	return os.str();
}


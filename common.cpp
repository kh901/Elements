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

std::string timeToString(time_t val)
{
	std::ostringstream os;
	struct tm * currentTime = localtime(&val);
	int seconds = currentTime->tm_sec;
	int minutes = currentTime->tm_min;
	int hours = currentTime->tm_hour;
	os << std::setfill('0') << std::setw(2) << hours << ':';
	os << std::setfill('0') << std::setw(2) << minutes << ':';
	os << std::setfill('0') << std::setw(2) << seconds;
	os << ' ';
	int day = currentTime->tm_mday;
	int month = 1 + currentTime->tm_mon; 
	int year = 1900 + currentTime->tm_year; 
	os << std::setfill('0') << std::setw(2) << day << '-';
	os << std::setfill('0') << std::setw(2) << month << '-';
	os << std::setfill('0') << std::setw(2) << year;
	return os.str();
}

time_t setWeekDeadlineFromNow()
{
	// get current time
	time_t val = time(NULL);
	// advance it a week
	val = getWeekAhead(val);
	// alter the time to be 12 midday
	struct tm * curTime = localtime(&val);
	curTime->tm_hour = 12;
	curTime->tm_min = 0;
	curTime->tm_sec = 0;
	return mktime(curTime);
}

time_t getTimeValue()
{
	return time(NULL);
}

time_t getDayAhead(time_t cur)
{
	// add a day
	static const long int DAY_TIME = 86400;
	cur += DAY_TIME;
	// set the time to be 12 midday
	struct tm * curTime = localtime(&cur);
	curTime->tm_hour = 12;
	curTime->tm_min = 0;
	curTime->tm_sec = 0;
	return mktime(curTime);
}
time_t getWeekAhead(time_t cur)
{
	// add a week
	static const long int WEEK_TIME = 604800;
	cur += WEEK_TIME;
	// set the time to be 12 midday
	struct tm * curTime = localtime(&cur);
	curTime->tm_hour = 12;
	curTime->tm_min = 0;
	curTime->tm_sec = 0;
	return mktime(curTime);
}


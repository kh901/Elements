#ifndef COMMON_H
#define COMMON_H

#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>

int Ceil(const int &, const int &);
// gets current time in HH:MM:SS format (24 hour time)
std::string getTimestamp();
// gets current date in DD-MM-YYYY format
std::string getDate();
// gets current time as a time_t (long int)
time_t getTimeValue();
// gets a day later at 12 midday from given value
time_t getDaysAhead(time_t, const int);
// gets a day later at 12 midday from given value
time_t getDayAhead(time_t);
// gets a week later at 12 midday from given value 
time_t getWeekAhead(time_t);
// converts time_t to show TIME DATE as format (HH:MM:SS DD-MM-YYYY)
std::string timeToString(time_t);
// gets a time value (long int) that is a week later from now at 12 midday
time_t setWeekDeadlineFromNow();

#endif

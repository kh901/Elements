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

#endif

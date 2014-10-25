#ifndef COMMON_H
#define COMMON_H

#include <ctime>
#include <string>
#include <sstream>

int Ceil(const int &, const int &);
// gets current time in HH:MM:SS format
std::string getTimestamp();
// gets current date in DD-MM-YYYY format
std::string getDate();

#endif

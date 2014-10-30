#include "File.h"

void appendString(std::ofstream &ofs, const std::string &strdata)
{
	// write size of string
	int size = strdata.length();
	ofs.write((char*)&size, sizeof(int));
	
	// write string data
	ofs.write(strdata.c_str(), size * sizeof(char));
}

void appendStringVector(std::ofstream &ofs, const std::vector<std::string> &strvec)
{
	// write size of vector
	int size = strvec.size();
	ofs.write((char*)&size, sizeof(int));
	
	// write vector data
	for (int i = 0; i < size; ++i)
	{
		appendString(ofs, strvec[i]);
	}
}

bool readString(std::ifstream &ifs, std::string &strdata)
{
	// read in size of string from file
	int size;
	ifs.read((char*)&size, sizeof(int));
	
	if (size <= 0)
	{
		return false;
	}
	strdata.clear();
	
	// read in string data from file
	// Note: could also make this a dynamic char array since size is given
	char tmp[MAX_STRING_LEN] = {""};
	ifs.read(tmp, size * sizeof(char));
	strdata = tmp;
	return true;
}

bool readStringVector(std::ifstream &ifs, std::vector<std::string> &strvec)
{
	// read in size of vector from file
	int size;
	ifs.read((char*)&size, sizeof(int));
	
	if (size <= 0)
	{
		return false;	
	}
	strvec.clear();
	strvec.reserve(size);
	
	// read in vector data from file
	std::string tmp;
	for (int i = 0; i < size; ++i)
	{
		tmp.clear();
		readString(ifs, tmp);
		strvec.push_back(tmp);
	}
	return true;
}


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

/*
void writeSubmissionsFile(const std::string &filename, const vector<Submission> &subs)
{
	std::ofstream fout;
	fout.open(filename.c_str(), ios::binary);
	
	// write size of vector
	int size = subs.size();
	fout.write((char*)&size, sizeof(int));
	
	// write data in vector
	for (int i = 0; i < size; ++i)
	{
		appendData<bool>(fout, subs[i].submitted);
		appendData<bool>(fout, subs[i].reviewed);
		appendData<std::string>(fout, subs[i].filename);
		appendData<std::string>(fout, subs[i].title);
		appendData<std::string>(fout, subs[i].description);
		appendVector<std::string>(fout, subs[i].authors);
		appendVector<std::string>(fout, subs[i].keywords);
		appendVector<Comment>(fout, subs[i].comments);
	}
	fout.close();
}

bool readSubmissionsFile(const std::string &filename, vector<Submission> &subs)
{
	std::ifstream fin;
    fin.open(filename.c_str(), std::ios::binary);
   
    // read in size of vector from file
    int size;
    fin.read((char*)&size, sizeof(int));
    
    if (size <= 0)
    {
    	fin.close();
    	return false;
    }
    subs.clear();
    subs.reserve(size);
    
    Submission tmp;
    // read in vector data from file
    for (int i = 0; i < size; ++i)
	{
		readData<bool>(fin, tmp.submitted);
		readData<bool>(fin, tmp.reviewed);
		readData<std::string>(fin, tmp.filename);
		readData<std::string>(fin, tmp.title);
		readData<std::string>(fin, tmp.description);
		readVector<std::string>(fin, tmp.authors);
		readVector<std::string>(fin, tmp.keywords);
		readVector<Comment>(fin, tmp.comments);
		subs.push_back(tmp);
	}
    fin.close();
    return true;
}
*/

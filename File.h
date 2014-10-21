#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

class FileIO
{
	public:
		virtual void writeFile(std::ofstream &) = 0;
		virtual bool readFile(std::ifstream &) = 0;
};

template<class A>
void appendData(std::ofstream &ofs, const A &data);

void appendString(std::ofstream &ofs, const std::string &strdata);

template<class H, class J>
void appendMap(std::ofstream &ofs, const std::map<H, J> &mapdata);

template <class B>
void appendVector(std::ofstream &ofs, const std::vector<B> &vecdata);

template<class T>
void writeFile(const std::string &filename, const std::vector<T> &myvec);

/*
	Each class that uses this function should inherit  
	and implement the interface described in the FileIO class.
	
	MyClass::writeFile(myFileStream) should use a combination of:
		appendData<datatype>(myFileStream, this->mydata);
		(and)
		appendVector<datatype>(myFileStream, this->mylist);
	
	in order to save their class's data to a binary file properly.
*/
template<class T>
void writeFileClass(const std::string &filename, const std::vector<T> &myvec);

template<class W>
void readData(std::ifstream &ifs, W &data);

bool readString(std::ifstream &ifs, std::string &strdata);

template<class Z>
bool readVector(std::ifstream &ifs, std::vector<Z> &vecdata);

template<class K, class L>
void readMap(std::ifstream &ifs, std::map<K, L> &mapdata);

template<class C>
bool readFile(const std::string &filename, std::vector<C> &myvec);

/*
	Each class that uses this function should inherit  
	and implement the interface described in the FileIO class.
	
	MyClass::readFile(myFileStream) should use a combination of:
		readData<datatype>(myFileStream, this->mydata);
		(and)
		readVector<datatype>(myFileStream, this->mylist);
	
	in order to read in their class's data from a vector saved in a binary file properly.
*/
template<class Y>
bool readFileClass(const std::string &filename, std::vector<Y> &myvec);

#endif

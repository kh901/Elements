#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#define MAX_STRING_LEN 256

class FileIO
{
	public:
		virtual void writeFile(std::ofstream &) const = 0;
		virtual void readFile(std::ifstream &) = 0;
};

template<class A>
void appendData(std::ofstream &ofs, const A &data)
{
	ofs.write((char*)&data, sizeof(A));
}

void appendString(std::ofstream &ofs, const std::string &strdata);
void appendStringVector(std::ofstream &ofs, const std::vector<std::string> &strvec);

template<class U>
void appendStringKeyMap(std::ofstream &ofs, const std::map<std::string, U> &mapdata)
{
	// write size of map
	int size = mapdata.size();
	ofs.write((char*)&size, sizeof(int));
	
	std::cout << "Size of map: " << size << std::endl;
	
	// write map data
	typename std::map<std::string, U>::const_iterator it;
	for (it = mapdata.begin(); it != mapdata.end(); ++it)
	{
		std::cout << "Write key: " << it->first << std::endl;
		// write the key
		appendString(ofs, it->first);
		std::cout << "Write value: " << it->second << std::endl;
		// write the value
		ofs.write((char*)&(it->second), sizeof(U));
	}
}

template<class H, class J>
void appendMap(std::ofstream &ofs, const std::map<H, J> &mapdata)
{
	// write size of map
	int size = mapdata.size();
	ofs.write((char*)&size, sizeof(int));
	
	std::cout << "Size of map: " << size << std::endl;
	
	// write map data
	typename std::map<H, J>::const_iterator it;
	for (it = mapdata.begin(); it != mapdata.end(); ++it)
	{
		std::cout << "Write key: " << it->first << std::endl;
		// write the key
		ofs.write((char*)&(it->first), sizeof(H));
		std::cout << "Write value: " << it->second << std::endl;
		// write the value
		ofs.write((char*)&(it->second), sizeof(J));
	}
}

template <class B>
void appendVector(std::ofstream &ofs, const std::vector<B> &vecdata)
{
	// write size of vector
	int size = vecdata.size();
	ofs.write((char*)&size, sizeof(int));
	
	// write vector data
	ofs.write((char*)&vecdata[0], vecdata.size() * sizeof(B));
}

template <class D>
void appendClass(std::ofstream &ofs, const D &data)
{
	data.writeFile(ofs); 	// use polymorphism to write each element
}

template<class T>
void writeFile(const std::string &filename, const std::vector<T> &myvec)
{
	std::ofstream fout;
	fout.open(filename.c_str(), std::ios::binary);
	
	appendVector<T>(fout, myvec);
	
	fout.close();
}

template<class R>
void appendClassVector(std::ofstream &ofs, const std::vector<R> &myvec)
{
	// write size of vector
	int size = myvec.size();
	ofs.write((char*)&size, sizeof(int));
	
	// write vector data
	for (int i = 0; i < size; ++i)
	{
		appendClass<R>(ofs, myvec[i]); 	// use polymorphism to write each element
	}
}

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
void writeFileClassVector(const std::string &filename, const std::vector<T> &myvec)
{
	std::ofstream fout;
	fout.open(filename.c_str(), std::ios::binary);
	
	appendClassVector<T>(fout, myvec);
	
	fout.close();
}

template<class W>
void readData(std::ifstream &ifs, W &data)
{
	ifs.read((char*)&data, sizeof(W));
}

bool readString(std::ifstream &ifs, std::string &strdata);
bool readStringVector(std::ifstream &ifs, std::vector<std::string> &strvec);

template<class Z>
bool readVector(std::ifstream &ifs, std::vector<Z> &vecdata)
{
	// read in size of vector from file
	int size;
	ifs.read((char*)&size, sizeof(int));
	
	if (size <= 0)
	{
		return false;	
	}
	vecdata.clear();
	vecdata.reserve(size);
	
	// read in vector data from file
	Z tmp;
	for (int i = 0; i < size; ++i)
	{
		ifs.read((char*)&tmp, sizeof(Z));
		vecdata.push_back(tmp);
	}
	return true;
}

template<class G>
bool readStringKeyMap(std::ifstream &ifs, std::map<std::string, G> &mapdata)
{
	// read in size of map from file
	int size;
	ifs.read((char*)&size, sizeof(int));
	
	if (size <= 0)
	{
		return false;	
	}
	mapdata.clear();
	
	// read in map data from file
	std::string tmpKey; 
	G tmpValue;
	for (int i = 0; i < size; ++i)
	{
		// read key
		readString(ifs, tmpKey);
		// read value
		ifs.read((char*)&tmpValue, sizeof(G));
		mapdata.insert(std::pair<std::string, G>(tmpKey, tmpValue));
	}
	return true;
}

template<class K, class L>
bool readMap(std::ifstream &ifs, std::map<K, L> &mapdata)
{
	// read in size of map from file
	int size;
	ifs.read((char*)&size, sizeof(int));
	
	if (size <= 0)
	{
		return false;	
	}
	mapdata.clear();
	
	// read in map data from file
	K tmpKey; 
	L tmpValue;
	for (int i = 0; i < size; ++i)
	{
		// read key
		ifs.read((char*)&tmpKey, sizeof(K));
		// read value
		ifs.read((char*)&tmpValue, sizeof(L));
		mapdata.insert(std::pair<K, L>(tmpKey, tmpValue));
	}
	return true;
}

template <class N>
bool readClass(std::ifstream &ifs, N &data)
{
	data.readFile(ifs);		// use polymorphism from classes which implement FileIO interface
	return true;		// hacky workaround!!
}

template<class C>
bool readFile(const std::string &filename, std::vector<C> &myvec) 
{
	std::ifstream fin;
    fin.open(filename.c_str(), std::ios::binary);
    
    bool readStatus = readVector<C>(fin, myvec);
 	
	fin.close();
	return readStatus;
}

template<class Q>
bool readClassVector(std::ifstream &ifs, std::vector<Q> &myvec)
{
	// read size of vector
    int size;
    ifs.read((char*)&size, sizeof(int));
    
    if (size <= 0)
    {
    	return false;
    }
    myvec.clear();
    myvec.reserve(size);
    
    // read contents of binary file into vector
    for (int i = 0; i < size; ++i)
    {
    	Q tmp;
    	readClass<Q>(ifs, tmp);		// use polymorphism to read in each element
    	myvec.push_back(tmp);
    }
	return true;
}

/*
	Each class that uses this function should inherit  
	and implement the interface described in the FileIO class.
	
	MyClass::readFile(myFileStream) should use a combination of:
		Simple data types: readData<datatype>(myFileStream, this->mydata);
		
		The above in a vector: readVector<datatype>(myFileStream, this->mylist);

		Strings: readString(myFileStream, this->mystring);

		String vectors: readStringVector(myFileStream, this->mystringvec);

		Classes which implement the FileIO interface:
			readClass<MyClass>(myFileStream, this->myclass);

		Vector of the above classes:
			readClassVector<MyClass>(myFileStream, this->myclassvec);

		Map of simple data types: readMap<Key, Value>(myFileStream, this->mymap);

		Special cases like:
			Map where the key is a string:
				readStringKeyMap<MyValue>(myFileStream, this->mystringkeymap);
	
	in order to read in their class's data from a vector saved in a binary file properly.
*/
template<class Y>
bool readFileClassVector(const std::string &filename, std::vector<Y> &myvec) 
{
	std::ifstream fin;
    fin.open(filename.c_str(), std::ios::binary);
    
	bool readStatus = readClassVector<Y>(fin, myvec);
    
	fin.close();
	return readStatus;
}

#endif

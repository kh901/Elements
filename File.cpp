#include "File.h"

template<class A>
void appendData(std::ofstream &ofs, const A &data)
{
	ofs.write((char*)&data, sizeof(A));
}

void appendString(std::ofstream &ofs, const std::string &strdata)
{
	// write size of string
	int size = strdata.length();
	ofs.write((char*)&size, sizeof(int));
	
	// write string data
	ofs.write(strdata.c_str(), size * sizeof(char));
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

template<class H, class J>
void appendMap(std::ofstream &ofs, const std::map<H, J> &mapdata)
{
	// write size of map
	int size = mapdata.size();
	ofs.write((char*)&size, sizeof(int));
	
	// write map data
	typename std::map<H, J>::iterator it;
	for (it = mapdata.begin(); it != mapdata.end(); ++it)
	{
		// write the key
		ofs.write((char*)&(it->first), sizeof(H));
		// write the value
		ofs.write((char*)&(it->second), sizeof(J));
	}
}

template<class T>
void writeFile(const std::string &filename, const std::vector<T> &myvec)
{
	std::ofstream fout;
	fout.open(filename.c_str(), std::ios::binary);
	
	appendVector<T>(fout, myvec);
	
	fout.close();
}

template<class W>
void readData(std::ifstream &ifs, W &data)
{
	ifs.read((char*)&data, sizeof(W));
}

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

template<class K, class L>
void readMap(std::ifstream &ifs, std::map<K, L> &mapdata)
{
	// read in size of map from file
	int size;
	ifs.read((char*)&size, sizeof(int));
	
	if (size <= 0)
	{
		return false;	
	}
	mapdata.clear();
	mapdata.reserve(size);
	
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
	std::string tmp;
	ifs.read((char*)&tmp, size * sizeof(char));
	strdata = tmp;
	return true;
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

template<class Y>
bool readFileClass(const std::string &filename, std::vector<Y> &myvec) 
{
	std::ifstream fin;
    fin.open(filename.c_str(), std::ios::binary);
    
    // read size of vector
    int size;
    fin.read((char*)&size, sizeof(int));
    
    if (size <= 0)
    {
    	fin.close();
    	return false;
    }
    myvec.clear();
    myvec.reserve(size);
    
    // read contents of binary file into vector
    for (int i = 0; i < size; ++i)
    {
    	Y tmp;
    	tmp.readFile(fin);		// use polymorphism to read in each element
    	myvec.push_back(tmp);
    }
	fin.close();
	return true;
}

template<class T>
void writeFileClass(const std::string &filename, const std::vector<T> &myvec)
{
	std::ofstream fout;
	fout.open(filename.c_str(), std::ios::binary);
	
	// write size of vector
	int size = myvec.size();
	fout.write((char*)&size, sizeof(int));
	
	// write vector data
	for (int i = 0; i < size; ++i)
	{
		myvec[i].writeFile(fout); 	// use polymorphism to write each element
	}
	fout.close();
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

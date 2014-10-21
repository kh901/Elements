#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <vector>

template<class A>
void appendData(std::ofstream &ofs, const A &data);

template <class B>
void appendVector(std::ofstream &ofs, const vector<B> &vecdata);

template<class T>
void writeFile(const std::string &filename, const vector<T> &myvec);

template<class W>
void readData(std::ifstream &ifs, W &data);

template<class Z>
bool readVector(std::ifstream &ifs, vector<Z> &vecdata);

template<class C>
bool readFile(const std::string &filename, vector<C> &myvec);

// Submissions
void writeSubmissionsFile(const std::string &filename, const vector<Submission> &subs);
bool readSubmissionsFile(const std::string &filename, vector<Submission> &subs)

#endif

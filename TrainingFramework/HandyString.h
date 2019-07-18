#pragma once
#include <string>
using namespace std;

static class HandyString
{
public:
	static int find_nth(string &s, size_t n);
	static string getTypeFolder(string &s);
	static string getFileExt(string &s);
	static string getFileName(string &s);
	static string getFileParent(string &s);
};


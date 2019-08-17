#include "stdafx.h"
#include "handyString.h"

int HandyString::find_nth(string & s, size_t n)
{
	int pos = 0;
	int found = -1;
	for (size_t i = 0; i < n; i++) {
		found = s.find("/", pos);
		pos = found + 1;
	}
	return found;
}

string HandyString::getTypeFolder(string & s)
{
	int i = find_nth(s, 2);
	int j = find_nth(s, 3);
	return s.substr(i + 1, j - i - 1);
}

string HandyString::getFileExt(string & s)
{
	size_t i = s.rfind('.', s.length());
	if (i != string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

string HandyString::getFileName(string & s)
{
	size_t i = s.rfind('/', s.length());
	size_t j = s.rfind('.', s.length());
	if (i == string::npos) {
		return(s.substr(0, j));
	}
	else if (j != string::npos) {
		return(s.substr(i + 1, j - i - 1));
	}

	return("");
}

string HandyString::getFileParent(string & s)
{
	int i = find_nth(s, 3);
	int j = find_nth(s, 4);
	return s.substr(i + 1, j - i - 1);
}

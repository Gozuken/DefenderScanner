#pragma once
#include <windows.h>

class Scanner
{
private:
	std::vector<char>* fileContent = nullptr;
public:
	DWORD scan(std::string path);

	int openFile(std::string path);

	int splitToFile(std::string fileName, int start, int end);
};
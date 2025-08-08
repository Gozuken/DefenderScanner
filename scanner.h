#pragma once
#include <windows.h>

class Scanner
{
private:
	
public:
	std::vector<char>* fileContent = nullptr;
	std::size_t fileSize = 0;

	DWORD scan(std::string path);

	int openFile(std::string path);

	int splitToFile(std::string fileName, int start, int end);
};
#pragma once
#include <windows.h>

class Scanner
{
private:
	std::vector<char>* fileContent = nullptr;
public:
	std::size_t fileSize = 0;
	DWORD scan(std::string path);

	int openFile(std::string path);

	int splitToFile(std::string fileName, int start, int end);
};
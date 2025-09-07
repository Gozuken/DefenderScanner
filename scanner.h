#pragma once
#include <windows.h>

class Scanner
{
private:
	
public:
	std::vector<char>* fileContent = nullptr;
	std::size_t fileSize = 0;

	DWORD scan(std::string path);

	bool openFile(std::string path);

	bool splitToFile(std::string fileName, int start, int end);
};
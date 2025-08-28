#pragma once
#include <windows.h>

class Scanner
{
private:
	
public:
<<<<<<< HEAD
	std::size_t fileSize = 0;
=======
	std::vector<char>* fileContent = nullptr;
	std::size_t fileSize = 0;

>>>>>>> 33e8358 (Full implementation, must be tested)
	DWORD scan(std::string path);

	int openFile(std::string path);

	int splitToFile(std::string fileName, int start, int end);
};
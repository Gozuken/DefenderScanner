#include <iostream>
#include <fstream>
#include <filesystem>
#include "scanner.h"


int main(int argc, char* argv[]) 	//check if defender is on before running!
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
		return 1;
	}
	std::string filePath = argv[1];

	Scanner scanner = Scanner();
	scanner.openFile(filePath);
	DWORD scanResult = scanner.scan(filePath);
	if (scanResult == 0)
	{
		std::cout << "[+] No bad signature found in file.";
		return 0;
	}

	std::size_t left = 0;
	std::size_t right = scanner.fileSize;
	std::size_t numberOfScans = 0;
	std::string fileToScan = {};

	// Heavy binary search
	while (left + 2048 < right) // 2048 is our window size, we could let user decide it for aggressiveness
	{
		std::size_t mid = left + (right - left) / 2;
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left, mid);
		scanResult = scanner.scan(fileToScan);

		if (scanResult == 2) // dirty, use left half
		{
			right = mid;
		}
		else if (scanResult == 0) // clean
		{
			numberOfScans++;
			fileToScan = std::format("{}.bin", numberOfScans);
			scanner.splitToFile(fileToScan, mid - 512, mid + 512);
			if (scanner.scan(fileToScan) == 2) // mid is near signature, we can continue precise scanning there
			{
				left = mid - 512;
				right = mid + 512;
				break;
			}
			else // we continue binary search
			{
				left = mid;
			}
		}
	}

	// Add seperate searching with multithreading for left and right pointers later
	// Precise window search
	while (true) // clean left
	{
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left + 10, right);
		
		if (scanner.scan(fileToScan) == 0)
		{
			break;
		}
		else
		{
			left += 10;
		}
	}
	while (true) // clean right
	{
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left , right - 10);

		if (scanner.scan(fileToScan) == 0)
		{
			break;
		}
		else
		{
			right -= 10;
		}
	}

	std::cout << "Left : " << left << std::endl << "Right : " << right;
	std::cin.get();
}
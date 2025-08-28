#include <iostream>
#include <fstream>
#include <filesystem>
#include "scanner.h"

<<<<<<< HEAD
void debug(std::size_t left, std::size_t right, DWORD scanResult)
{
	std::cout << "[-] LEFT : " << left << ", RIGHT : " << right << std::endl;
	std::cout << "Scan result : " << scanResult << std::endl;
	std::cin.get();
}

// We should show a hexdump and which part of the executable (.rdata, .text, etc.)
// maybe use vector<> and use push_back() to not create whole new arrays? dont know if that has any use case in this code.
int main(int argc, char* argv[]) 	// check if defender is on before running!
{									// add cleanup for temp files!!
	/*

=======

int main(int argc, char* argv[]) 	//check if defender is on before running!
{
>>>>>>> 33e8358 (Full implementation, must be tested)
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
		return 1;
	}
	std::string filePath = argv[1];
<<<<<<< HEAD

	Scanner scanner = Scanner();
	scanner.openFile(filePath);
	DWORD scanResult = scanner.scan(filePath);
	if (scanResult == 0)
	{
		std::cout << "[+] No bad signature found in file.";
		return 0;
	}
	const char* filePath = argv[1];
	*/
=======
>>>>>>> 33e8358 (Full implementation, must be tested)

	Scanner scanner = Scanner();
<<<<<<< HEAD
	DWORD scanResult = scanner.scan(filePath);
	std::size_t left = 0;
	std::size_t right = scanner.fileSize;
	std::size_t numberOfScans = 0;
	std::string fileToScan = {};

	debug(left, right, scanResult);
	// Heavy binary search
	while (left + 2048 < right) // 2048 is our window size, we could let user decide it for aggressiveness
	{
		std::size_t mid = left + (right - left) / 2;
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left, mid);
		scanResult = scanner.scan(fileToScan);

		if (scanResult == 2)
		{
			right = mid;
		}
		else if (scanResult == 0)
		{
			numberOfScans++;
			fileToScan = std::format("{}.bin", numberOfScans);
			scanner.splitToFile(fileToScan, mid - 512, mid + 512);
			if (scanner.scan(fileToScan) == 2) // mid is near signature, we can continue precise scanning there
			{
				left = mid - 512;
				right = mid + 512;
				// std::filesystem::remove(fileToScan);
				break;
			}
			else // we continue binary search
			{
				left = mid;
			}
		}

		debug(left, right, scanResult);

		// std::filesystem::remove(fileToScan); // lacking error handling, add later
	}

	// Add seperate searching with multithreading for left and right pointers later
	// Precise window search
	std::cout << "Binary search ended!";
	while (left < right) // clean left
	{
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left + 100, right);
		
=======
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

>>>>>>> 33e8358 (Full implementation, must be tested)
		if (scanner.scan(fileToScan) == 0)
		{
			break;
		}
		else
		{
<<<<<<< HEAD
			left += 100;
		}
		debug(left, right, scanResult);
	}
	while (right > left) // clean right
	{
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left , right - 100);

		if (scanner.scan(fileToScan) == 0)
		{
			break;
		}
		else
		{
			right -= 100;
		}
		debug(left, right, scanResult);
	}

	numberOfScans++;
	fileToScan = std::format("{}.bin", numberOfScans);
	scanner.splitToFile(fileToScan, left, right);
	if (scanner.scan(fileToScan) == 0)
	{
		std::cerr << "Failed to find bad string. This doesn't invalidate its existence.";
		return 1;
	}

	std::cout << "Left : " << left << std::endl << "Right : " << right;
	std::cin.get();
	return 0;
=======
			right -= 10;
		}
	}

	std::cout << "Left : " << left << std::endl << "Right : " << right;
	std::cin.get();
>>>>>>> 33e8358 (Full implementation, must be tested)
}
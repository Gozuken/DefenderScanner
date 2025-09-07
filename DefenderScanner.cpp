#include <iostream>
#include <fstream>
#include <filesystem>
#include <any>
#include "scanner.h"

template <typename T>
void debug(T value)
{
	std::cout << value << std::endl;
}

// We should show a hexdump and which part of the executable (.rdata, .text, etc.)
// maybe use vector<> and use push_back() to not create whole new arrays? dont know if that has any use case in this code.
// add cleanup for temp files!!
int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
		return 1;
	}

	Scanner scanner = Scanner();
	std::string filePath = argv[1];
	scanner.openFile(filePath);
	std::size_t left = 0;
	std::size_t right = scanner.fileSize;
	std::size_t numberOfScans = 0;
	std::string fileToScan = {};
	
	DWORD scanResult = scanner.scan(filePath);
	debug(scanner.fileSize);

	if (scanResult == 0)
	{
		std::cout << "[+] No bad signature found in file.";
		return 0;
	}

	// Heavy binary search
	while (left + 2048 < right) // 2048 is our window size, we could let user decide it for precision
	{
		debug("BINARY ENTERED");
		std::size_t mid = left + (right - left) / 2;
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left, mid);
		scanResult = scanner.scan(fileToScan);
		debug(scanResult);

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
		debug("CLEAN LEFT");
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left + 10, right);

		std::cout << "fileToScan : " << fileToScan << std::endl;
		std::cout << "left, right  : " << left << ", " << right << std::endl;
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
		debug("CLEAN RIGHT");
		numberOfScans++;
		fileToScan = std::format("{}.bin", numberOfScans);
		scanner.splitToFile(fileToScan, left, right - 10);

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
	}
}
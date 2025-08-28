#include <iostream>
#include <format>
#include <fstream>
#include <windows.h>
#include <boost/locale.hpp>
#include <random>
#include "scanner.h"

// --------------------[PIPE READING FUNCTION]-----------------------------
std::string readPipe(HANDLE hRead) {
    std::string output;
    const DWORD bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead;

    while (true) {
        BOOL success = ReadFile(hRead, buffer, bufferSize, &bytesRead, NULL);
        if (!success) {
            throw std::runtime_error("Could not read data from pipe!");
        }
        else if (bytesRead == 0) {
            break;
        }
        output.append(buffer, bytesRead);
    }

    return output;
}
// ------------------------------------------------------------------------

// convert path string to wchar and feed it to createprocess
DWORD Scanner::scan(std::string path)
{
    /*
    std::wstring wide_path = boost::locale::conv::to_utf<wchar_t>(path, "UTF-8");
    std::wstring cmdLine = std::format(
        L"C:\\Program Files\\Windows Defender\\MpCmdRun.exe -Scan -ScanType 3 -File \"{}\" -DisableRemediation -Trace -Level 0x10",
        wide_path
    );

    // --------------------[PIPE CREATION]-----------------------------
    HANDLE hWrite, hRead;
    SECURITY_ATTRIBUTES sa = {};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE; // inheritable handle
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        std::cout << "Failed to create pipe\n";
        return 1;
    }

    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);
    // ------------------------------------------------------------------

    // --------------------[PROCESS CREATION]-----------------------------
    wchar_t* cmd = &cmdLine[0];
    STARTUPINFO si = {};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;  // redirect stdout
    si.hStdError = hWrite;
    PROCESS_INFORMATION pi;

    BOOL success = CreateProcessW(
        NULL,   // Application name, NULL means use cmd line
        cmd,    // Command line
        NULL,   // Process security attributes
        NULL,   // Thread security attributes
        TRUE,   // Inherit handles 
        0,      // Creation flags
        NULL,   // Environment
        NULL,   // Current directory
        &si,    // Startup information
        &pi     // Process information
    );
    // ------------------------------------------------------------------

    if (success) {
        DWORD exitCode = 0;
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(hWrite); // close the only remaining handle so we can read (EOF)

        std::string result;
        try {
            result = readPipe(hRead);
            std::cout << "RESULT: " << result << std::endl;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Runtime error: " << e.what() << std::endl;
        }

        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return exitCode;
    }
    else {
        std::cerr << "Failed to start process. Error: " << GetLastError() << "\n";
        return GetLastError();
    }
    */

    std::random_device rd;                  // Seed
    std::mt19937 gen(rd());                 // Mersenne Twister RNG
    std::bernoulli_distribution dist(0.5);  // 50% true, 50% false

    bool randomBool = dist(gen);
    if (randomBool == true)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

int Scanner::openFile(std::string path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "[-] Could not open the file.";
        return 1;
    }

    // get filesize
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    fileContent = new std::vector<char>(file_size);
    file.read(fileContent->data(), file_size);
    fileSize = file_size;

<<<<<<< HEAD
    if (!file.good()) {
=======
    fileContent = new std::vector<char>(file_size);
    file.read(fileContent->data(), file_size);  // save filecontent
    fileSize = file_size;                       // save filesize 
                                                // in scanner for later use 
    if (!file.good())
    {
>>>>>>> 33e8358 (Full implementation, must be tested)
        std::cerr << "[-] Could not read file";
        return 1;
    }

    file.close();
    return 0;
}

// outFileName, startingByte, endingByte
int Scanner::splitToFile(std::string fileName, int start, int end)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "[-] Could not create a file.";
        return 1;
    }

    file.write(fileContent->data() + start, end - start);
    file.close();

    if (!file.good()) {
        std::cerr << "[-] Could not write to the file.";
        return 1;
    }

    return 0;
}

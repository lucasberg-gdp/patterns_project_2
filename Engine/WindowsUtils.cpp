#include "WindowsUtils.h"

#include <iostream>

bool WindowsUtils::GetAllFilesFromFolder(std::string path, std::vector<std::string>& fileNames)
{
    std::string searchPattern = path;
    searchPattern += "\\*";

    WIN32_FIND_DATAA fileData;
    HANDLE hFind = FindFirstFileA(searchPattern.c_str(), &fileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
            {
                fileNames.push_back(fileData.cFileName);
            }
        } while (FindNextFileA(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    else {
        //std::cerr << "Failed to open directory." << std::endl;
        return 1;
    }

    return true;
}

bool WindowsUtils::GetAllFoldersFromFolder(const std::string path, std::vector<std::string>& folderNames)
{
    std::string searchPattern = path;
    searchPattern += "\\*";

    WIN32_FIND_DATAA fileData;
    HANDLE hFind = FindFirstFileA(searchPattern.c_str(), &fileData);

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "Error opening directory.\n";
        return false;
    }

    do {
        if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::string folderName = fileData.cFileName;
            if (folderName != "." && folderName != "..") 
            {
                folderNames.push_back(folderName);
            }
        }
    } while (FindNextFileA(hFind, &fileData) != 0);

    FindClose(hFind);

    return true;
}

bool WindowsUtils::LoadPirateAssets(std::vector<std::string>& fileNames)
{
	const char* folderPath = "assets/models/PiratePack/Converted";

	if (!GetAllFilesFromFolder(folderPath, fileNames))
	{
		std::cout << "Pirates assets failed to load" << std::endl;

		return false;
	}

	return true;
}
